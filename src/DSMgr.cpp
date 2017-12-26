//
// Created by Thoh Testarossa on 2017/12/18.
//

#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#endif

#ifndef DSMGR_H
#define DSMGR_H

#include "DSMgr.h"

#endif

#ifndef BFRAME_H
#define BFRAME_H

#include "bFrame.h"

#endif

#ifndef BMGR_H
#define BMGR_H

#include "BMgr.h"

#endif

DSMgr::DSMgr()
{
    this->currFile = nullptr;
    this->numPages = 0;
    for(int i = 0; i < MAXPAGES; i++)
        this->pages[i] = 0;
}

int DSMgr::openFile(std::string fileName)
{
    this->currFile = fopen(fileName.c_str(), "r+");
    if(this->currFile != nullptr)
    {
        fseek(this->currFile, 0, SEEK_SET);
        return 0;
    }
    else
        return FILE_NOT_FOUND;
}

int DSMgr::closeFile()
{
    if(this->currFile != nullptr)
    {
        fclose(this->currFile);
        this->currFile = nullptr;
    }
    return 0;
}

//The size of bFrame is the same as the size of page
bFrame DSMgr::readPage(int page_id)
{
    //Initialize a blank frame
    bFrame result;
    for(int i = 0; i < FRAMESIZE; i++)
        result.field[i] = (char)0;

    //Try to find the specific page in the file "data.dbf"
    int currPage_id = -1;
    fseek(this->currFile, 0, SEEK_SET);
    while(!feof(this->currFile))
    {
        fscanf(this->currFile, "%d\n", &currPage_id);
        //If succeeded, copy the page content into the bFrame structure
        if(currPage_id == page_id) {
            fread((void *)result.field, sizeof(char), FRAMESIZE, this->currFile);
            break;
        }
        else
            fseek(this->currFile, FRAMESIZE, SEEK_CUR);
    }

    //Return the content of this page
    return result;
}

int DSMgr::writePage(int page_id, bFrame frm)
{
    //Try to find the page's position in the file and write there
    fseek(this->currFile, 0, SEEK_SET);
    int currPage_id = -1;
    while(!feof(this->currFile))
    {
        fscanf(this->currFile, "%d\n", &currPage_id);
        if(currPage_id == page_id)
        {
            fseek(this->currFile, 0, SEEK_CUR);
            fwrite((void *)frm.field, sizeof(char), FRAMESIZE, this->currFile);
            return FRAMESIZE;
        }
        else
            fseek(this->currFile, FRAMESIZE, SEEK_CUR);
    }

    //If not, move to the end of this file and add the content there
    fseek(this->currFile, 0, SEEK_END);
    fprintf(this->currFile, "%d\n", page_id);
    fwrite((void *)frm.field, sizeof(char), FRAMESIZE, this->currFile);
    fprintf(this->currFile, "\n");

    return FRAMESIZE;
}

int DSMgr::seek(int offset, int pos)
{
    return 0;
}

FILE *DSMgr::getFile()
{
    return this->currFile;
}

void DSMgr::incNumPages()
{
    this->numPages++;
}

int DSMgr::getNumPages()
{
    return this->numPages;
}

void DSMgr::setUse(int index, int use_bit)
{

}

int DSMgr::getUse(int index)
{
    return 0;
}
