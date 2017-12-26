//
// Created by Thoh Testarossa on 2017/12/17.
//

#ifndef ADB_BUFFERSYSTEM_CONFIG_H
#define ADB_BUFFERSYSTEM_CONFIG_H

//Common headers
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <random>

#include <cstdlib>
#include <climits>
#include <ctime>

//Some marco definition
#define MAXPAGES 1024
#define FRAMESIZE 4096
#define DEFBUFSIZE 1024

#define INITAGE 500
#define AGINGCOUNT 1

#define TOTALPAGENUMBER 50000

//Some paramaters
#define FILE_NAME "./data.dbf"
#define SERIES_NAME "./IOSeries.txt"

//Some status definition
#define LATCHED 77577
#define UNLATCHED 26504

#define CLEAN 175664
#define DIRTY 113325

#define PAGENULL 55532
#define FRAMENULL 54084

//Some error definition
#define PAGE_NOT_FOUND -213
#define PTOF_NOT_FOUND -222
#define FRAME_NOT_FOUND -227
#define FILE_NOT_FOUND -234

#endif //ADB_BUFFERSYSTEM_CONFIG_H
