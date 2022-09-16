#define WIN32_LEAN_AND_MEAN
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <malloc.h>

#include <MemoryModule.h>

typedef int (*entryPointFunction)();

#define DLL_FILE TEXT("program.exe")

void *ReadLibrary(size_t *pSize)
{
    size_t read;
    void *result;
    FILE *fp;

    fp = _tfopen(DLL_FILE, _T("rb"));
    if (fp == NULL)
    {
        _tprintf(_T("Can't open DLL file \"%s\"."), DLL_FILE);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    *pSize = static_cast<size_t>(ftell(fp));
    if (*pSize == 0)
    {
        fclose(fp);
        return NULL;
    }

    result = (unsigned char *)malloc(*pSize);
    if (result == NULL)
    {
        return NULL;
    }

    fseek(fp, 0, SEEK_SET);
    read = fread(result, 1, *pSize, fp);
    fclose(fp);
    if (read != *pSize)
    {
        free(result);
        return NULL;
    }

    return result;
}

void LoadFromMemory(void)
{
    void *data;
    size_t size;
    HMEMORYMODULE handle;

    PMEMORYMODULE pMemoryModule;
    entryPointFunction entryPoint;

    data = ReadLibrary(&size);
    if (data == NULL)
    {
        return;
    }

    handle = MemoryLoadLibrary(data, size);
    if (handle == NULL)
    {
        _tprintf(_T("Can't load library from memory.\n"));
        goto exit;
    }

    pMemoryModule = (PMEMORYMODULE)handle;
    entryPoint = (entryPointFunction)(pMemoryModule->exeEntry);
    entryPoint();

    MemoryFreeLibrary(handle);

exit:
    free(data);
}

#define MAX_CALLS 20

struct CallList
{
    int current_alloc_call, current_free_call;
    CustomAllocFunc alloc_calls[MAX_CALLS];
    CustomFreeFunc free_calls[MAX_CALLS];
};

LPVOID MemoryFailingAlloc(LPVOID address, SIZE_T size, DWORD allocationType, DWORD protect, void *userdata)
{
    UNREFERENCED_PARAMETER(address);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(allocationType);
    UNREFERENCED_PARAMETER(protect);
    UNREFERENCED_PARAMETER(userdata);
    return NULL;
}

LPVOID MemoryMockAlloc(LPVOID address, SIZE_T size, DWORD allocationType, DWORD protect, void *userdata)
{
    CallList *calls = (CallList *)userdata;
    CustomAllocFunc current_func = calls->alloc_calls[calls->current_alloc_call++];
    assert(current_func != NULL);
    return current_func(address, size, allocationType, protect, NULL);
}

BOOL MemoryMockFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType, void *userdata)
{
    CallList *calls = (CallList *)userdata;
    CustomFreeFunc current_func = calls->free_calls[calls->current_free_call++];
    assert(current_func != NULL);
    return current_func(lpAddress, dwSize, dwFreeType, NULL);
}

void InitFuncs(void **funcs, va_list args)
{
    for (int i = 0;; i++)
    {
        assert(i < MAX_CALLS);
        funcs[i] = va_arg(args, void *);
        if (funcs[i] == NULL)
            break;
    }
}

void InitAllocFuncs(CallList *calls, ...)
{
    va_list args;
    va_start(args, calls);
    InitFuncs((void **)calls->alloc_calls, args);
    va_end(args);
    calls->current_alloc_call = 0;
}

void InitFreeFuncs(CallList *calls, ...)
{
    va_list args;
    va_start(args, calls);
    InitFuncs((void **)calls->free_calls, args);
    va_end(args);
    calls->current_free_call = 0;
}

void InitFreeFunc(CallList *calls, CustomFreeFunc freeFunc)
{
    for (int i = 0; i < MAX_CALLS; i++)
    {
        calls->free_calls[i] = freeFunc;
    }
    calls->current_free_call = 0;
}

void TestFailingAllocation(void *data, size_t size)
{
    CallList expected_calls;
    HMEMORYMODULE handle;

    InitAllocFuncs(&expected_calls, MemoryFailingAlloc, MemoryFailingAlloc, NULL);
    InitFreeFuncs(&expected_calls, NULL);

    handle = MemoryLoadLibraryEx(
        data, size, MemoryMockAlloc, MemoryMockFree, MemoryDefaultLoadLibrary,
        MemoryDefaultGetProcAddress, MemoryDefaultFreeLibrary, &expected_calls);

    assert(handle == NULL);
    assert(GetLastError() == ERROR_OUTOFMEMORY);
    assert(expected_calls.current_free_call == 0);

    MemoryFreeLibrary(handle);
    assert(expected_calls.current_free_call == 0);
}

void TestCleanupAfterFailingAllocation(void *data, size_t size)
{
    CallList expected_calls;
    HMEMORYMODULE handle;
    int free_calls_after_loading;

    InitAllocFuncs(&expected_calls,
                   MemoryDefaultAlloc,
                   MemoryDefaultAlloc,
                   MemoryDefaultAlloc,
                   MemoryDefaultAlloc,
                   MemoryFailingAlloc,
                   NULL);
    InitFreeFuncs(&expected_calls, MemoryDefaultFree, NULL);

    handle = MemoryLoadLibraryEx(
        data, size, MemoryMockAlloc, MemoryMockFree, MemoryDefaultLoadLibrary,
        MemoryDefaultGetProcAddress, MemoryDefaultFreeLibrary, &expected_calls);

    free_calls_after_loading = expected_calls.current_free_call;

    MemoryFreeLibrary(handle);
    assert(expected_calls.current_free_call == free_calls_after_loading);
}

void TestFreeAfterDefaultAlloc(void *data, size_t size)
{
    CallList expected_calls;
    HMEMORYMODULE handle;
    int free_calls_after_loading;

    // Note: free might get called internally multiple times
    InitFreeFunc(&expected_calls, MemoryDefaultFree);

    handle = MemoryLoadLibraryEx(
        data, size, MemoryDefaultAlloc, MemoryMockFree, MemoryDefaultLoadLibrary,
        MemoryDefaultGetProcAddress, MemoryDefaultFreeLibrary, &expected_calls);

    assert(handle != NULL);
    free_calls_after_loading = expected_calls.current_free_call;

    MemoryFreeLibrary(handle);
    assert(expected_calls.current_free_call == free_calls_after_loading + 1);
}

#ifdef _WIN64

LPVOID MemoryAllocHigh(LPVOID address, SIZE_T size, DWORD allocationType, DWORD protect, void *userdata)
{
    int *counter = static_cast<int *>(userdata);
    if (*counter == 0)
    {
        // Make sure the image gets loaded to an address above 32bit.
        uintptr_t offset = 0x10000000000;
        address = (LPVOID)((uintptr_t)address + offset);
    }
    (*counter)++;
    return MemoryDefaultAlloc(address, size, allocationType, protect, NULL);
}
#endif // _WIN64

void TestCustomAllocAndFree(void)
{
    void *data;
    size_t size;

    data = ReadLibrary(&size);
    if (data == NULL)
    {
        return;
    }

    _tprintf(_T("Test MemoryLoadLibraryEx after initially failing allocation function\n"));
    TestFailingAllocation(data, size);
    _tprintf(_T("Test cleanup after MemoryLoadLibraryEx with failing allocation function\n"));
    TestCleanupAfterFailingAllocation(data, size);
    _tprintf(_T("Test custom free function after MemoryLoadLibraryEx\n"));
    TestFreeAfterDefaultAlloc(data, size);

    free(data);
}

int main()
{
    LoadFromMemory();
    return 0;
}
