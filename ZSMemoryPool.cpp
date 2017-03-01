#include "ZSMemoryPool.h"
#include <stdio.h>

ZSMemoryPool::ZSMemoryPool()
{
}

ZSMemoryPool::ZSMemoryPool(const ZSMemoryPool&)
{

}

ZSMemoryPool::ZSMemoryPool(size_t min_alloc_size, size_t max_alloc_size, size_t max_mem)
{
	this->min_alloc_size = min_alloc_size + EXTRA_MEM;
	this->max_alloc_size = max_alloc_size + EXTRA_MEM;
	this->max_mem = max_mem;
	trunks = nullptr;
	blocks = nullptr;
	__alloc_mem = nullptr;
	__extra_mem = nullptr;
	ZSPoolInit();
}


ZSMemoryPool::~ZSMemoryPool()
{
	ZSDestroy();
}

int ZSMemoryPool::ZSPoolInit()
{
	size_t n = min_alloc_size;
	size_t m = max_alloc_size;
	size_t k = max_mem;
	for (size_t i = 1;; i <<= 1)//矫正最大值m，矫正为m<=2^i
	{
		if (m <= i)
		{
			m = i;
			max_alloc_size = i;
			break;
		}
	}
	for (size_t i = m;; i >>= 1)//矫正最小值n，矫正为n>=2^i
	{
		if (n >= i)
		{
			n = i<<1;
			min_alloc_size = n;
			break;
		}
	}
	for (size_t i = k;; i <<= 1)//矫正总内存k，矫正为k<=2^i
	{
		if (k <= i)
		{
			k = i;
			max_mem = k;
			break;
		}
	}
	ZSPoolMemInit(n, m, k);
	return 0;
}

void * ZSMemoryPool::ZSAlloc(size_t alloc_size)
{
	//匹配最佳位置
	int idx = 0;
	size_t _fit_mem = min_alloc_size;
	while (_fit_mem < alloc_size || blocks[idx].free_cnt <= 0)
	{
		_fit_mem <<= 1;
		++idx;
	}
	if (idx >= _distance)
	{
		printf("内存池已满,直接分配内存\n");
		return new char[alloc_size]();
	}
	for (int i = 0; i < bk_cnt; ++i)
	{
		MBlock_t * rtBk = (MBlock_t*)blocks[idx].start_adr + i*_fit_mem;
		if (rtBk->isUsed = MEM_IS_FREE)
		{
			rtBk->isUsed = MEM_IS_USED;
			blocks[idx].use_cnt += 1;
			blocks[idx].free_cnt -= 1;
			use_mem += rtBk->iLength;
			return rtBk->pBuffer;
		}
	}
	return nullptr;
}

int ZSMemoryPool::ZSFree(void * _freePtr)
{
	if (_freePtr != nullptr)
	{
		MBlock_t *check = (MBlock_t *)((char*)_freePtr - EXTRA_MEM);
		if (check->isUsed == MEM_IS_USED)
		{
			check->isUsed = MEM_IS_FREE;
			MBlock *bk = (MBlock*)check->pHead;
			bk->free_cnt += 1;
			bk->use_cnt -= 1;
			use_mem -= check->iLength;
			return 0;
		}
		else
		{
			printf("该内存不是由此内存池分配,无法处理");
			return 1;
		}
	}
	return -1;
}

int ZSMemoryPool::ZSDestroy()
{
	MChunk *tmp = trunks;
	MChunk *pPre = trunks;
	while (pPre != nullptr)
	{
		tmp = pPre->pNext;
		if (pPre->buf != nullptr)
			delete[] pPre->buf;
		delete pPre;
		pPre = tmp;
	}
	return 0;
}

int ZSMemoryPool::add_head(MChunk * ck)
{
	if (ck != nullptr)
	{
		ck->pNext = trunks;
		trunks = ck;
		return 0;
	}
	return -1;
}

int ZSMemoryPool::ZSPoolMemInit(size_t _Ssize, size_t _Esize, size_t _Tsize)
{
	if (__alloc_mem == nullptr)
		__alloc_mem = new char[max_mem]();

	//计算每个slab中的buf分配个数
	int alloc_count = _Tsize / (_Esize << 1);
	bk_cnt = alloc_count;
	int offset = 0;//内存块偏移量
	
	size_t st = _Ssize;
	size_t ed = _Esize;
	int dst = 0;
	while (st <= ed)
	{
		st <<= 1;
		++dst;
	}
	_distance = dst;//计算最大最小内存块距离


	--dst;
	blocks = new MBlock [_distance]();
	for (size_t i = _Esize; i >= _Ssize; i >>= 1)
	{
		MChunk *ck = new MChunk();
		ck->size = i;
		ck->buf = (char*)__alloc_mem + offset;
		for (int j = 0; j < alloc_count; ++j)
		{
			MBlock_t *_tmp_block = (MBlock_t *)((char*)__alloc_mem + offset + j*ck->size);
			_tmp_block->iLength = ck->size;
			_tmp_block->isUsed = MEM_IS_FREE;
			_tmp_block->pBuffer = MEM_OFFSER(_tmp_block);
			if (j == 0)
			{
				blocks[dst].free_cnt = bk_cnt;
				blocks[dst].use_cnt = 0;
				blocks[dst].start_adr = (char*)_tmp_block;
			}
			_tmp_block->pHead = (void*)&blocks[dst];
		}
		dst--;

		add_head(ck);
		offset += i*alloc_count;
	}

	return 0;
}
