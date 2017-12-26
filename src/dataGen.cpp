//
// Created by Thoh Testarossa on 2017/12/22.
//

#ifndef CONFIG_H
#define CONFIG_H

#include "config.h"

#endif

std::string randomPageGen()
{
    std::random_device r;
    std::uniform_int_distribution<int> uniform_dist((int)'0', (int)'z');
    std::default_random_engine e1(r());

    std::string page = std::string("");
    for(int i = 0; i < FRAMESIZE; i++)
        page += (char)uniform_dist(e1);
    return page;
}

int main()
{
    std::ofstream fout = std::ofstream(FILE_NAME);
    for(int i = 0; i < TOTALPAGENUMBER; i++)
    {
        fout << i << std::endl;
        fout << randomPageGen() << std::endl;
    }
    fout.close();

    return 0;
}