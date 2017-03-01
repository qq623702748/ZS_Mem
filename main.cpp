#include "ZSMemoryPool.h"
#include <iostream>
#include <ctime>
#include <memory>
#include<stdlib.h>
using namespace std;

void test1(ZSMemoryPool * zs)
{
    srand((unsigned short)time(nullptr));
    time_t ts2 = time(0);
    for (int i = 0; i < 100000000; i++)
    {
        char * pData = new char[rand() % 51 * 1024];
        delete[](char*)pData;
    }
    cout << "new系统函数耗时:" << time(0) - ts2 << "s" << endl;


    time_t ts1 = time(0);
    for (int i = 0; i < 100000000; i++)
    {
        char * pData = (char*)malloc(rand() % 51 * 1024);
        free(pData);
    }
    cout << "malloc系统函数耗时:" << time(0) - ts1 << "s" << endl;

    time_t ts = time(0);
    for (int i = 0; i < 100000000; i++)
    {
        void * pData = zs->ZSAlloc(rand() % 51 * 1024);
        zs->ZSFree(pData);
    }
    cout << "ZS耗时:" << time(0) - ts << "s" << endl;
}

void test2(ZSMemoryPool * zs)
{
    for (int i = 0; i < 15; ++i)
    {
        zs->ZSAlloc(8);
    }
    zs->ZSAlloc(10);
}
int main()
{
    ZSMemoryPool *zs = new ZSMemoryPool(6, 51*1024,1*1024*1024);
    //shared_ptr<ZSMemoryPool> zs(new ZSMemoryPool(6, 51 * 1024, 1 * 1024 * 1024));
    //int total = 0;
    //for (int i = 32; i <= 64 * 1024; i <<= 1)
    //{
    //    total += i;
    //}
    //cout << "total:" << total << " 1M:" << 1 * 1024 * 1024 << endl;
    
    test1(zs);
    //test2(zs);
    //char * ptr = new char[20]();
    //zs->ZSFree(ptr);
#ifdef __WIN32
    system("pause");
#endif
    return 0;
}
