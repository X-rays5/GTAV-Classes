#pragma once
#include <cstddef>

namespace rage
{
    class sysMemAllocator
    {
    public:
        virtual ~sysMemAllocator() = 0;

        virtual void SetQuitOnFail(bool) = 0;
        virtual void* Allocate(std::size_t size, std::size_t align, int subAllocator) = 0;
        virtual void* TryAllocate(std::size_t size, std::size_t align, int subAllocator) = 0;
        virtual void Free(void* pointer) = 0;
        virtual void TryFree(void* pointer) = 0;
        virtual void Resize(void* pointer, std::size_t size) = 0;
        virtual sysMemAllocator* GetAllocator(int allocator) const = 0;
        virtual sysMemAllocator* GetAllocator(int allocator) = 0;
        virtual sysMemAllocator* GetPointerOwner(void* pointer) = 0;
        virtual std::size_t GetSize(void* pointer) const = 0;
        virtual std::size_t GetMemoryUsed(int memoryBucket) = 0;
        virtual std::size_t GetMemoryAvailable() = 0;

    public:
        static sysMemAllocator* UpdateAllocatorValue()
        {
            //B9 ? ? ? ? 48 8B 0C 01 45 33 C9 49 8B D2 48
            auto g_gtaTlsEntry = *(sysMemAllocator**)(*(uintptr_t*)(__readgsqword(88)) + 0xC8);

            if (g_gtaTlsEntry == nullptr)
                return nullptr;

            *(sysMemAllocator**)(*(uintptr_t*)(__readgsqword(88)) + 0xC8) = g_gtaTlsEntry;
            *(sysMemAllocator**)(*(uintptr_t*)(__readgsqword(88)) + 0xC8 - 8) = g_gtaTlsEntry;

            return g_gtaTlsEntry;
        }
        
    };

    inline sysMemAllocator* GetAllocator()
    {
        sysMemAllocator* allocator = *(sysMemAllocator**)(*(uintptr_t*)(__readgsqword(88)) + 0xC8);

        if (!allocator)
        {
        return sysMemAllocator::UpdateAllocatorValue();
        }

        return allocator;
    }
}