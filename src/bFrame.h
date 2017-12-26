//
// Created by Thoh Testarossa on 2017/12/17.
//

#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#endif

#ifndef ADB_BUFFERSYSTEM_BFRAME_H
#define ADB_BUFFERSYSTEM_BFRAME_H

struct bFrame
{
    char field[FRAMESIZE];
};

class bBuffer
{
public:
    bBuffer()
    {
        bBuffer(DEFBUFSIZE);
    }

    bBuffer(int bufferSize)
    {
        this->buf = new bFrame[bufferSize];
        for(int i = 0; i < bufferSize; i++)
        {
            for(int j = 0; j < FRAMESIZE; j++)
                this->buf[i].field[j] = (char)0;
        }
    }

    bFrame *buf;
};

#endif //ADB_BUFFERSYSTEM_BFRAME_H
