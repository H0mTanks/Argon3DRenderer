#include "PrecompiledHeaders.hpp"

int count = 0;
int allocated = 0;
int deleted = 0;

void* operator new(size_t size)
{
	std::cout << ++count << " Allocating " << size << " bytes\n";
	allocated += size;
	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	std::cout << count << " Deleting " << size << " bytes\n";
	allocated -= size;
	free(memory);
}

//void operator delete[](void* memory, size_t size)
//{
//	std::cout << count << " Deleting " << size << " bytes\n";
//	allocated -= size;
//	free(memory);
//}