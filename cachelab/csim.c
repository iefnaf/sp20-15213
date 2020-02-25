/*
1.如果出现跨cacheLine如何解决？
  比如，0x1100，定位到cacheLine，但是要求的字节超过了该cacheline的最后一个字节
  出现跨块的情况
2.如何设计数据结构?
3.如何malloc一块内存作为cache?如何对该块内存进行便捷的操作?
4.c语言如何进行文件io?
5.命令行参数的解析方法
6.lru算法
*/

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    int tag;   //int 是否足够？
    int valid;
    int lastUsedTime;    //lru replacement algs   
}CacheLine;

int s = -1, E = -1, b = -1, verbose = 0;

int hit = 0, miss = 0, eviction = 0;

int overAllTime = 0; //used for lru

CacheLine* cachePool;


void displayHelp()
{
    return;

}

void visitCache(long address)
{
    int tag = address >> (b + s);
    unsigned cacheIndex = address >> b & ((1 << s) - 1);

    CacheLine* cacheRow = cachePool + E * cacheIndex;
    CacheLine* victim = cacheRow;

    for (int i = 0; i < E; i++)
    {
        if (!cacheRow[i].valid)     // compulsory miss
        {
            ++miss;
            cacheRow[i].valid = 1;
            cacheRow[i].lastUsedTime = overAllTime;
            cacheRow[i].tag = tag;
            return;
        }

        if (cacheRow[i].valid && cacheRow[i].tag == tag)       // hit
        {
            cacheRow[i].lastUsedTime = overAllTime;
            ++hit;
            return;
        }

        if (cacheRow[i].lastUsedTime < victim->lastUsedTime)
        {
            victim = cacheRow + i;
        }
    }

    // capacity miss
    ++miss;
    ++eviction;
    victim->lastUsedTime = overAllTime;
    victim->tag = tag;
}


int main(int argc, char *argv[])
{
    int opt;
    FILE* tracefile;

    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch(opt)
        {
            case 's':
                s = atoi(optarg);
                break;
            
            case 'E':
                E = atoi(optarg);
                break;
            
            case 'b':
                b = atoi(optarg);
                break;
            
            case 't':
                tracefile = fopen(optarg, "r");
                break;
            
            case 'v':
                verbose = 1;
                break;
            
            case 'h':
                displayHelp();

            default:
                displayHelp();
        }
    }

    if (s == -1 || E == -1 || b == -1 || tracefile == NULL)
    {
        printf("%s: Missing required command line argument\n", argv[0]);
		displayHelp();
    }

    cachePool = (CacheLine*) malloc(sizeof(CacheLine) * E * (1 << s));
    memset(cachePool, 0, sizeof(CacheLine) * E * (1 << s));
    
    char operation;
    long address;
    int size;

    while (fscanf(tracefile, "%s%lx,%d\n", &operation, &address, &size) == 3)
    {
        ++overAllTime;
        
        switch(operation)
        {
            case 'M':
                visitCache(address);
                ++hit;
                break;
            
            case 'L':
                visitCache(address);
                break;
            
            case 'S':
                visitCache(address);
                break;
            
            case 'I':
                break;
        }
    }

    fclose(tracefile);

    free(cachePool);

    printSummary(hit, miss, eviction);
    return 0;
}
