#ifndef NOS_HEAPMEMORY
#define NOS_HEAPMEMORY

#include "NOS_Includes.h"

#define HEAP_PAGE_SIZE 256
#define HEAP_PAGE_COUNT 256

typedef union HeapMemoryStateByte
{
	uint8_t data;
	
};

typedef struct NOS_HeapMemory_t
{
	bool pageInfo[HEAP_PAGE_COUNT];
	uint8_t data[HEAP_PAGE_COUNT * HEAP_PAGE_SIZE];

}NOS_HeapMemory;

void NOS_HeapMemory_Init(NOS_HeapMemory* mem)
{
	for (int i = 0; i < HEAP_PAGE_COUNT; i++)
	{
		mem->pageInfo[i] = false;
	}

	for (int i = 0; i < HEAP_PAGE_SIZE * HEAP_PAGE_COUNT; i++)
	{
		mem->data[i] = 0xFF;
	}
}

static int GetPageCount(int size)
{
	int currPage = 0;
	int pageCount = 0;
	int count = size % HEAP_PAGE_SIZE;

	if (size < HEAP_PAGE_SIZE)
	{
		pageCount = 1;
	}
	else
	{
		if (count == 0)
		{
			pageCount = (size / HEAP_PAGE_SIZE);
		}
		else
		{
			pageCount = (size / HEAP_PAGE_SIZE) + 1;
		}
	}

	return pageCount;
}

static void FreePage(NOS_HeapMemory* mem, uint8_t* ptr)
{
	int offset = ptr - mem->data;
	int pageOffset = offset / HEAP_PAGE_SIZE;
	for (int i = 0; i < HEAP_PAGE_SIZE; i++)
	{
		mem->data[offset + i] = 0xFF;
	}

	mem->pageInfo[pageOffset] = false;
}

uint8_t* NOS_Alloc(NOS_HeapMemory* mem, int size)
{

	int pageCount = GetPageCount(size);
	int currPage = 0;
	uint8_t* result = NULL;
	bool first = true;
	bool found = false;
	bool done = false;


	while(!done)
	{
		if (currPage < HEAP_PAGE_COUNT)
		{
			if (!mem->pageInfo[currPage])
			{
				for (int i = 0; i < pageCount; i++)
				{
					if (mem->pageInfo[currPage + i])
					{

					}
				}

				found = true;
			}

			if(found)
			{
				if (first)
				{
					result = &mem->data[currPage * HEAP_PAGE_SIZE];
					first = false;
				}

				for (int x = 0; x < pageCount; x++)
				{
					mem->pageInfo[currPage] = true;
					currPage++;
				}

				done = true;
			}

			currPage++;
		}
		else
		{
			done = true;
		}
	}

	return result;
}

void NOS_Free(NOS_HeapMemory* mem,uint8_t* ptr,int size)
{
	int pageCount = GetPageCount(size);

	uint8_t* currPtr = ptr;

	for (int i = 0; i < pageCount; i++)
	{
		FreePage(mem, currPtr);
		currPtr += HEAP_PAGE_SIZE;
	}
}

#endif