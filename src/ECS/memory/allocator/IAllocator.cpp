/*
	Author : Tobias Stein
	Date   : 9th July, 2016
	File   : IAllocator.cpp

	Base allocator class.

	All Rights Reserved. (c) Copyright 2016.
*/
#include "../../../../include/ECS/memory/allocator/IAllocator.h"

namespace ECS {

	IAllocator::IAllocator(const size_t memSize, const void* mem) :
		m_MemorySize(memSize),
		m_MemoryFirstAddress(mem),
		m_MemoryUsed(0),
		m_MemoryAllocations(0)
	{}

	IAllocator::~IAllocator()
	{}

} // ECS::Memory::Allocator