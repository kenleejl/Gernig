#include <windows.h>

#include <MemoryModule.h>

#define MAX_CALLS 20

struct CallList
{
    int current_alloc_call, current_free_call;
    CustomAllocFunc alloc_calls[MAX_CALLS];
    CustomFreeFunc free_calls[MAX_CALLS];
};

void *ReadLibrary(size_t *pSize);

void LoadFromMemory(void);

LPVOID MemoryFailingAlloc(LPVOID address, SIZE_T size, DWORD allocationType, DWORD protect, void *userdata);

LPVOID MemoryMockAlloc(LPVOID address, SIZE_T size, DWORD allocationType, DWORD protect, void *userdata);

BOOL MemoryMockFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType, void *userdata);

void InitFuncs(void **funcs, va_list args);

void InitAllocFuncs(CallList *calls, ...);

void InitFreeFuncs(CallList *calls, ...);

void InitFreeFunc(CallList *calls, CustomFreeFunc freeFunc);

void TestFailingAllocation(void *data, size_t size);

void TestCleanupAfterFailingAllocation(void *data, size_t size);

void TestFreeAfterDefaultAlloc(void *data, size_t size);

#ifdef _WIN64
LPVOID MemoryAllocHigh(LPVOID address, SIZE_T size, DWORD allocationType, DWORD protect, void *userdata);
#endif // _WIN64

void TestCustomAllocAndFree(void);
