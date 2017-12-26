//
// Created by Thoh Testarossa on 2017/12/17.
//

#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#endif

#ifndef BCB_H
#define BCB_H

#include "BCB.h"

#endif

#ifndef BFRAME_H
#define BFRAME_H

#include "bFrame.h"

#endif

#ifndef ADB_BUFFERSYSTEM_BMGR_H
#define ADB_BUFFERSYSTEM_BMGR_H

//Note: ptof_id has the same meaning as frame_id
//The difference between this two is just that we usually use ptof_id for BCB array and frame_id for buffer array
//The BCB which has ptof_id <----> The frame which has frame_id if ptof_id == frame_id
class BMgr
{
public:
    BMgr();
    //Interface functions
    //Return a frame_id which is relative to the page which page_id represents for
    int fixPage(int page_id, int prot);
    void fixNewPage();
    int unfixPage(int page_id);
    int numFreeFrames();
    BCB getBCB(int ptof_id);
    bFrame readFrame(int frame_id);
    void writeFrame(int frame_id, bFrame frm);
    //Internal functions
    int selectVictim(int page_id);
    int hash(int page_id);
    void removeBCB(BCB *ptr, int ptof_id);
    void removeLRUEle(int frame_id);
    void setDirty(int frame_id);
    void unsetDirty(int frame_id);
    void writeDirtys();
    void printFrame(int frame_id);
    void initBCB(int ptof_id, int page_id, int isDirty);
    void initFtop(int frame_id, int page_id);

private:
    //Hash Table
    int ftop[DEFBUFSIZE];
    //Actually, ptof.frame_id = the position where BCB is in the BCB array
    BCB ptof[DEFBUFSIZE];

    //Buffer
    bFrame buffer[DEFBUFSIZE];
};

#endif //ADB_BUFFERSYSTEM_BMGR_H
