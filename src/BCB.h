//
// Created by Thoh Testarossa on 2017/12/17.
//

#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#endif

#ifndef ADB_BUFFERSYSTEM_BCB_H
#define ADB_BUFFERSYSTEM_BCB_H

class BCB
{
public:
    //Useless constructor I think
    BCB()
    {
        BCB(0, 0);
    }

    BCB(int page_id, int frame_id)
    {
        this->setPage_id(page_id);
        this->setFrame_id(frame_id);
        //I have no idea if these initial values are right
        this->setLatched(UNLATCHED);
        this->setCount(0);
        this->setDirty(CLEAN);
        this->setNextBCB(nullptr);
    }

    int getPage_id() {return this->page_id; }
    int getFrame_id() {return this->frame_id; }
    int getIfLatched() {return this->latch; }
    int getCount() {return this->count; }
    int getIfDirty() {return this->dirty; }
    BCB *getNextBCB() {return this->next; }

    void setPage_id(int page_id) {this->page_id = page_id; }
    void setFrame_id(int frame_id) {this->frame_id = frame_id; }
    void setLatched(int latch) {this->latch = latch; }
    void setCount(int count) {this->count = count; }
    void setDirty(int dirty) {this->dirty = dirty; }
    void setNextBCB(BCB *next) {this->next = next; }

private:
    int page_id;
    int frame_id;
    int latch;
    int count;
    int dirty;
    BCB *next;
};

#endif //ADB_BUFFERSYSTEM_BCB_H
