#ifndef _ZSMEMORY_H_
#define _ZSMEMORY_H_
#include <stdio.h>
/*!
 * \file ZSMemoryPool.h
 *
 * \author judy
 * \date ���� 2017
 *
 * ˽���ڴ��
 */


/*!
 * \class MemoryBlock
 *
 * \brief �����ڴ����Ľṹ
 *
 * \author judy
 * \date ���� 2017
 */
#define MEM_IS_USED 0xFFFF
#define MEM_IS_FREE 0x7FFF
#define EXTRA_MEM 16//����16���ֽ�
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
 * \brief �������Ľṹ�����MemoryBlock���һ��MemoryChunk
 *
 * \author judy
 * \date ���� 2017
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
 * \brief SLAB�㷨�����ڴ���࣬ͨ��ʹ�ø����ȡ
 *
 * \author admin
 * \date ���� 2017
 */
class ZSMemoryPool
{
public:
	ZSMemoryPool(size_t min_alloc_size, size_t max_alloc_size, size_t max_mem);
	//�ڴ�س�ʼ������
	int ZSPoolInit();
	//�ڴ�������ڴ�ӿ�
	void * ZSAlloc(size_t alloc_size);
	//�ڴ���ͷ��ڴ�ӿ�
	int ZSFree(void * _freePtr);
	//�ڴ���ͷ�������Դ�ӿ�
	int ZSDestroy();
protected:
	ZSMemoryPool();
	ZSMemoryPool(const ZSMemoryPool&);
	~ZSMemoryPool();

private:
	int add_head(MChunk * ck);
	//��ʼ�����ڴ���з��䲢�Ҷ�λ�±�
	int ZSPoolMemInit(size_t _Ssize, size_t _Esize, size_t _Tsize);


private:
	//8 16 32 62 128 256 512 1k 2k 4k 8k 16k 32k 64k
	MChunk * trunks;		//����Ľṹ
	size_t bk_cnt;			//ÿ����SLAB����Ŀ
	MBlock * blocks;		//�ڴ�����

	size_t _distance;			//�����С�ڴ�����2^n����
	size_t min_alloc_size;		//8byte
	size_t max_alloc_size;		//64k
	size_t max_mem;			//����ڴ�
	void * __alloc_mem;		//�������ڴ�
	void * __extra_mem;		//��������ڴ��ʣ����ڴ�
	size_t use_mem;			//�Ѿ�ʹ�����ڴ�

};
#endif
