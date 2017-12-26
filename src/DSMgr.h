//
// Created by Thoh Testarossa on 2017/12/17.
//

#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#endif

#ifndef BFRAME_H
#define BFRAME_H

#include "bFrame.h"

#endif

#ifndef ADB_BUFFERSYSTEM_DSMGR_H
#define ADB_BUFFERSYSTEM_DSMGR_H

class DSMgr
{
public:
    DSMgr();
    int openFile(std::string fileName);
    int closeFile();
    bFrame readPage(int page_id);
    int writePage(int page_id, bFrame frm);
    int seek(int offset, int pos);
    FILE *getFile();
    void incNumPages();
    int getNumPages();
    void setUse(int index, int use_bit);
    int getUse(int index);
private:
    FILE *currFile;
    int numPages;
    int pages[MAXPAGES];
};

#endif //ADB_BUFFERSYSTEM_DSMGR_H
