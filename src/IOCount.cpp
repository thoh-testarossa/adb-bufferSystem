#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#endif

#ifndef DSMGR_H
#define DSMGR_H

#include "DSMgr.h"

#endif

#ifndef BMGR_H
#define BMGR_H

#include "BMgr.h"

#endif

int main()
{
    std::cout << "Hello, World!" << std::endl;

    //Test 1 for DSMgr file IO
    DSMgr ds_mgr = DSMgr();
    if(ds_mgr.openFile(FILE_NAME) == FILE_NOT_FOUND) return -1;

    //I test
    bFrame pageA = ds_mgr.readPage(5);
    std::cout << pageA.field << std::endl;

    //O test
    //bFrame r_bFrame;
    //for(int i = 0; i < FRAMESIZE; i++) r_bFrame.field[i] = (char)0;
    //strcpy(r_bFrame.field, "test");
    //ds_mgr.writePage(3, r_bFrame);

    ds_mgr.closeFile();

    //Test 1 passed

    //Test 2 for Read IO series
    std::vector<int> IO_ops_series = std::vector<int>();
    std::vector<int> page_series = std::vector<int>();

    std::ifstream fin_series = std::ifstream(SERIES_NAME);
    if(fin_series.is_open())
    {
        while(!fin_series.eof())
        {
            int io, p_id;
            char quote;
            fin_series >> io >> quote >> p_id;
            IO_ops_series.push_back(io);
            page_series.push_back(p_id);
            std::cout << io << " " << p_id << std::endl;
        }
    }

    int pageFlag[TOTALPAGENUMBER];
    for(int i = 0; i < TOTALPAGENUMBER; i++)
        pageFlag[i] = 0;
    for(int i = 0; i < page_series.size(); i++)
        pageFlag[page_series.at(i)] = 1;
    int pageCount = 0;
    for(int i = 0; i < TOTALPAGENUMBER; i++)
        pageCount += pageFlag[i];
    std::cout << pageCount << std::endl;

    //Test 2 passed

    //Main program part
    //Global I/O counter
    int iCount = 0, oCount = 0;

    //temp frame variable
    bFrame frm;

    //BMgr initialization
    BMgr b_mgr = BMgr();

    //For each instruction, BMgr manage the BCB, buffer and other things
    for(int i = 0; i < IO_ops_series.size(); i++)
    {
        int page_id = page_series.at(i) - 1;
        //0 for read, 1 for write
        std::cout << i << " " << IO_ops_series.at(i) << " " << page_id << std::endl;
        //For debug
        if(IO_ops_series.at(i) == 0)
        {
            //When reading a page, BMgr will first try to find the page in the buffer
            int frame_id = b_mgr.fixPage(page_id, 0);
            if(frame_id != FRAME_NOT_FOUND)
            {
                std::cout << "A frame " << frame_id << " has been selected" << std::endl;
                //Check if the frame does contain the page by checking corresponding BCB
                if (b_mgr.getBCB(frame_id).getPage_id() == PAGENULL)
                {
                    //If not, a DSMgr will be used to read the page in the storage, which will make 1 time of storage access
                    //...and, it means a frame has been written back to the storage, which will make 1 time of storage access
                    std::cout << "A empty frame" << std::endl << "Read from storage and write it into the frame" << std::endl;
                    oCount++;
                    ds_mgr.openFile(FILE_NAME);
                    frm = ds_mgr.readPage(page_id);
                    ds_mgr.closeFile();
                    iCount++;
                    //BCB & hash table maintenance part
                    b_mgr.initBCB(frame_id, page_id, CLEAN);
                    b_mgr.initFtop(frame_id, page_id);

                    b_mgr.writeFrame(frame_id, frm);
                }
                else if(b_mgr.getBCB(frame_id).getPage_id() == page_id)
                {
                    //If the frame which contains this page exists in the buffer, just fetch it from buffer
                    std::cout << "Hit at frame " << frame_id << std::endl << "Read from the frame directly" << std::endl;
                    frm = b_mgr.readFrame(frame_id);
                    b_mgr.initBCB(frame_id, page_id, CLEAN);
                    b_mgr.initFtop(frame_id, page_id);
                }
                else
                    std::cout << "error0" << std::endl;
            }
            else
            {
                //This part means that BMgr can't select a frame to store this page, so it will be taken from storage directly by using DSMgr
                //This thing also make 1 time of storage access
                std::cout << "Can not select a frame for current page" << std::endl << "Read the page from the storage" << std::endl << "Nothing will be saved into the buffer" << std::endl;
                ds_mgr.openFile(FILE_NAME);
                frm = ds_mgr.readPage(page_id);
                ds_mgr.closeFile();
                iCount++;
            }
        }
        else if(IO_ops_series.at(i) == 1)
        {
            //Make a frame to write if needed
            for(int j = 0; j < FRAMESIZE; j++)
                frm.field[j] = (char)0;
            for(int j = 0; j < i && j < FRAMESIZE; j++)
                frm.field[j] = 'k';

            //When writing a page, BMgr will first try to find the page in the buffer and write it in the buffer
            int frame_id = b_mgr.fixPage(page_id, 0);
            if(frame_id != FRAME_NOT_FOUND)
            {
                std::cout << "A frame " << frame_id << " has been selected" << std::endl;
                if(b_mgr.getBCB(frame_id).getPage_id() == PAGENULL)
                {
                    //If not, BMgr will call a DSMgr to read the page in the storage, which will make 1 time of storage access
                    //...and, it means a frame has been written back to the storage, which will make 1 time of storage access
                    std::cout << "A empty frame" << std::endl << "Read from storage and write the modified version into the frame" << std::endl;
                    oCount++;
                    //BMgr will write the change into the page which was read and placed in the buffer just now instead of write it into storage directly
                    ds_mgr.openFile(FILE_NAME);
                    bFrame frm_tmp = ds_mgr.readPage(page_id);
                    ds_mgr.closeFile();
                    iCount++;
                    //BCB & hash table maintenance part
                    b_mgr.initBCB(frame_id, page_id, DIRTY);
                    b_mgr.initFtop(frame_id, page_id);
                }
                else if(b_mgr.getBCB(frame_id).getPage_id() == page_id)
                {
                    //If the frame which contains this page exists in the buffer, just change this frame's status to DIRTY
                    std::cout << "Hit at frame " << frame_id << std::endl << "Read from the frame directly and write the modified version into the frame" << std::endl;
                    b_mgr.setDirty(frame_id);
                    b_mgr.initBCB(frame_id, page_id, DIRTY);
                    b_mgr.initFtop(frame_id, page_id);
                }
                else
                    std::cout << "error1" << std::endl;
                //Write this page into buffer
                b_mgr.writeFrame(frame_id, frm);
            }
            else
            {
                //This part means that BMgr can't select a frame to store this page, so this page will be written into storage directly
                //This thing also make 1 time of storage access
                std::cout << "Can not select a frame for current page" << std::endl << "Write the page to the storage" << std::endl << "Nothing will be saved into the buffer" << std::endl;
                ds_mgr.openFile(FILE_NAME);
                ds_mgr.writePage(page_id, frm);
                ds_mgr.closeFile();
                oCount++;
            }
        }
        else;
        //After that, all other frames' BCB are needed to be modified(mainly age)
        std::cout << "Fix the aging count for available frames" << std::endl;
        for(int j = 0; j < DEFBUFSIZE; j++)
        {
            if(b_mgr.getBCB(j).getPage_id() != PAGENULL)
                b_mgr.unfixPage(b_mgr.getBCB(j).getPage_id());
        }
    }

    std::cout << iCount << std::endl << oCount << std::endl;
    return 0;
}