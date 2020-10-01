/*
	Author : Tobias Stein
	Date   : 22nd October, 2016
	File   : StackAllocator.h

	Stack allocator.

	All Rights Reserved. (c) Copyright 2016.
*/

#ifndef __STACK_ALLOC_H__
#define __STACK_ALLOC_H__

#include "IAllocator.h"

namespace ECS {

	/*
	*/
	class StackAllocator : public IAllocator
	{
	private:

		struct AllocMetaInfo
		{
			u8 adjustment;
		};

	public:

		StackAllocator(size_t memSize, const void* mem);

		virtual ~StackAllocator();

		virtual void* allocate(size_t size, u8 alignment) override;
		virtual void free(void* p) override;
		virtual void clear() override;

	}; // class StackAllocator

} // namespace ECS::Memory::Allocator

#endif // __STACK_ALLOC_H__