//
// Created by Thoh Testarossa on 2017/12/18.
//

#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#endif

#ifndef BMGR_H
#define BMGR_H

#include "BMgr.h"

#endif

#ifndef BCB_H
#define BCB_H

#include "BCB.h"

#endif

#ifndef DSMGR_H
#define DSMGR_H

#include "DSMgr.h"

#endif

BMgr::BMgr()
{
    for(int i = 0; i < DEFBUFSIZE; i++)
    {
        this->ftop[i] = PAGENULL;
        this->ptof[i] = BCB(PAGENULL, i);
        for(int j = 0; j < FRAMESIZE; j++)
            this->buffer[i].field[j] = (char)0;
    }
}

int BMgr::fixPage(int page_id, int prot)
{
    //Somehow to find if a page exists in some frame which has frame_id using page_id
    //What's the meaning of parameter prot?
    int ptof_id = this->hash(page_id);
    if(ptof_id != PTOF_NOT_FOUND)
        return this->ptof[ptof_id].getFrame_id();
    //If not, select a victim page and throw away if needed and load this requested page
    //Victim page is not latched
    else
    {
        int ptof_NeedToBeChanged = this->selectVictim(page_id);
        if(ptof_NeedToBeChanged != PTOF_NOT_FOUND)
        {
            //Get the frame id
            int frame_NeedToBeChanged = this->ptof[ptof_NeedToBeChanged].getFrame_id();
            //Write the frame's content out
            //Use a DSMgr to do this
            if(this->ptof[frame_NeedToBeChanged].getIfDirty() == DIRTY)
            {
                DSMgr ds_mgr = DSMgr();
                ds_mgr.openFile(FILE_NAME);
                ds_mgr.writePage(page_id, this->buffer[frame_NeedToBeChanged]);
                ds_mgr.closeFile();
            }
            //Clean the frame
            for(int i = 0; i < FRAMESIZE; i++)
                this->buffer[frame_NeedToBeChanged].field[i] = (char)0;
            //Clean the hash table and BCB
            this->ftop[frame_NeedToBeChanged] = PAGENULL;
            this->removeBCB(this->ptof, ptof_NeedToBeChanged);
            //Return this frame_id
            return frame_NeedToBeChanged;
        }
        else
            return FRAME_NOT_FOUND;
    }
}

void BMgr::fixNewPage()
{

}

//Maybe this is the function to decrement the specific page's age and check its latch status?
int BMgr::unfixPage(int page_id)
{
    int ptof_id = this->hash(page_id);
    if(ptof_id != PTOF_NOT_FOUND)
    {
        int newCount = this->ptof[ptof_id].getCount() - AGINGCOUNT;
        this->ptof[ptof_id].setCount(newCount > 0 ? newCount : 0);
        if(this->ptof[ptof_id].getCount() == 0) this->ptof[ptof_id].setLatched(UNLATCHED);
        return this->ptof[ptof_id].getFrame_id();
    }
    else return FRAME_NOT_FOUND;
}

//Where is the buffer? Inside/outside the BMgr?
int BMgr::numFreeFrames()
{
    int ans = 0;
    for(int i = 0; i < DEFBUFSIZE; i++)
    {
        if(this->ptof[i].getPage_id() == PAGENULL)
            ans++;
    }
    return ans;
}

BCB BMgr::getBCB(int ptof_id)
{
    return this->ptof[ptof_id];
}

bFrame BMgr::readFrame(int frame_id)
{
    return this->buffer[frame_id];
}

void BMgr::writeFrame(int frame_id, bFrame frm)
{
    this->buffer[frame_id] = frm;
}

int BMgr::selectVictim(int page_id)
{
    //Find the frame to be replaced just like the hash function
    //LRU is naturally applied with aging algorithm(decreasing BCB count)
    int ptof_id = PTOF_NOT_FOUND;
    if(this->ptof[page_id % DEFBUFSIZE].getIfLatched() == UNLATCHED)
        ptof_id = page_id % DEFBUFSIZE;
    else
    {
        for(int i = (page_id % DEFBUFSIZE + 1) % DEFBUFSIZE; i != page_id % DEFBUFSIZE; i = (i + 1) % DEFBUFSIZE)
        {
            if(this->ptof[i].getIfLatched() == UNLATCHED)
            {
                ptof_id = i;
                break;
            }
        }
    }
    //Part for dirty frame
    if(ptof_id != PTOF_NOT_FOUND && this->ptof[ptof_id].getIfDirty() == DIRTY)
    {
        //Write dirty frame/page into file
        //Use a instance of DSMgr to access files
        //Todo
    }
    return ptof_id;
}

//Use page_id to calculate ptof_id
int BMgr::hash(int page_id)
{
    int ptof_id = page_id % DEFBUFSIZE;
    //If current BCB shows that this frame has the specific page, return this ptof_id
    if(this->ptof[ptof_id].getPage_id() == page_id || this->ptof[ptof_id].getPage_id() == PAGENULL) return ptof_id;
    //Otherwise, search the whole BCB array to find if this ptof_id exists
    do
    {
        ptof_id = (ptof_id + 1) % DEFBUFSIZE;
    }while(this->ptof[ptof_id].getPage_id() != page_id &&
            this->ptof[ptof_id].getPage_id() != PAGENULL &&
            ptof_id != page_id % DEFBUFSIZE);
    //If still not found after a complete search in this BCB array, return a error value
    if(ptof_id == page_id % DEFBUFSIZE) ptof_id = PTOF_NOT_FOUND;
    return ptof_id;
}

//Remove the info in BCB
void BMgr::removeBCB(BCB *ptr, int ptof_id)
{
    //Remove the page information in this BCB
    if(ptof_id != PTOF_NOT_FOUND)
    {
        this->ptof[ptof_id].setPage_id(PAGENULL);
        this->ptof[ptof_id].setDirty(CLEAN);
        this->ptof[ptof_id].setLatched(UNLATCHED);
        this->ptof[ptof_id].setCount(0);
    }
    else;
}

void BMgr::removeLRUEle(int frame_id)
{

}

void BMgr::setDirty(int frame_id)
{
    if(this->ftop[frame_id] != PAGENULL)
        if(int ptof_id = this->hash(this->ftop[frame_id]) != PTOF_NOT_FOUND)
            this->ptof[ptof_id].setDirty(DIRTY);
}

void BMgr::unsetDirty(int frame_id)
{
    if(this->ftop[frame_id] != PAGENULL)
        if(int ptof_id = this->hash(this->ftop[frame_id]) != PTOF_NOT_FOUND)
            this->ptof[ptof_id].setDirty(CLEAN);
}

void BMgr::writeDirtys()
{

}

void BMgr::printFrame(int frame_id)
{

}

void BMgr::initBCB(int ptof_id, int page_id, int isDirty)
{
    this->ptof[ptof_id].setPage_id(page_id);
    this->ptof[ptof_id].setFrame_id(ptof_id);
    this->ptof[ptof_id].setLatched(LATCHED);
    this->ptof[ptof_id].setCount(INITAGE);
    this->ptof[ptof_id].setDirty(isDirty);
}

void BMgr::initFtop(int frame_id, int page_id)
{
    this->ftop[frame_id] = page_id;
}