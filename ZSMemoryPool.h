#ifndef _ZSMEMORY_H_
#define _ZSMEMORY_H_
#include <stdio.h>
/*!
 * \file ZSMemoryPool.h
 *
 * \author judy
 * \date 二月 2017
 *
 * 私有内存池
 */


/*!
 * \class MemoryBlock
 *
 * \brief 定义内存块核心结构
 *
 * \author judy
 * \date 二月 2017
 */
#define MEM_IS_USED 0xFFFF
#define MEM_IS_FREE 0x7FFF
#define EXTRA_MEM 16//额外16个字节
#define MEM_OFFSER(BASE_ADDR) (char*)BASE_ADDR+EXTRA_MEM
typedef struct MemoryBlock_t
{
	int iLength;
	int isUsed;
	void *pHead;
	void *pBuffer;
}MBlock_t;

typedef struct MemoryBlock
{
	int free_cnt;
	int use_cnt;
	void * start_adr;
}MBlock;

/*!
 * \class MemoryChunk
 *
 * \brief 定义块核心结构，多个MemoryBlock组成一个MemoryChunk
 *
 * \author judy
 * \date 二月 2017
 */
typedef struct MemoryChunk
{
	MemoryChunk * pNext;
	int size;
	void *buf;
}MChunk;


/*!
 * \class ZSMemoryPool
 *
 * \brief SLAB算法无锁内存池类，通过使用该类获取
 *
 * \author admin
 * \date 二月 2017
 */
class ZSMemoryPool
{
public:
	ZSMemoryPool(size_t min_alloc_size, size_t max_alloc_size, size_t max_mem);
	//内存池初始化函数
	int ZSPoolInit();
	//内存池申请内存接口
	void * ZSAlloc(size_t alloc_size);
	//内存池释放内存接口
	int ZSFree(void * _freePtr);
	//内存池释放所有资源接口
	int ZSDestroy();
protected:
	ZSMemoryPool();
	ZSMemoryPool(const ZSMemoryPool&);
	~ZSMemoryPool();

private:
	int add_head(MChunk * ck);
	//初始化的内存进行分配并且定位下标
	int ZSPoolMemInit(size_t _Ssize, size_t _Esize, size_t _Tsize);


private:
	//8 16 32 62 128 256 512 1k 2k 4k 8k 16k 32k 64k
	MChunk * trunks;		//块核心结构
	size_t bk_cnt;			//每区域SLAB的数目
	MBlock * blocks;		//内存数组

	size_t _distance;			//最大最小内存块距离2^n次幂
	size_t min_alloc_size;		//8byte
	size_t max_alloc_size;		//64k
	size_t max_mem;			//最大内存
	void * __alloc_mem;		//申请总内存
	void * __extra_mem;		//分配好总内存后剩余的内存
	size_t use_mem;			//已经使用总内存

};
#endif
