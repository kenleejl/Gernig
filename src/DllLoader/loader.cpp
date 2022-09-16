#include <stdio.h>
#include <stdlib.h>

#include <MemoryModule.h>
#include <loader.hpp>

typedef int (*entryPointFunction)();

void *ReadLibrary(char *fpath, size_t *pSize)
{
    size_t read;
    void *result;
    FILE *fp;

    fp = fopen(fpath, "rb");
    if (fp == NULL)
    {
        printf("Can't open file \"%s\".", fpath);
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

void LoadFromMemory(void *data, size_t size)
{
    HMEMORYMODULE handle;

    PMEMORYMODULE pMemoryModule;
    entryPointFunction entryPoint;

    handle = MemoryLoadLibrary(data, size);
    if (handle == NULL)
    {
        printf("Can't load library from memory.\n");
        return;
    }

    pMemoryModule = (PMEMORYMODULE)handle;
    // entryPoint = (entryPointFunction)(0x0000000140001544); // call main, will return
    entryPoint = (entryPointFunction)(pMemoryModule->exeEntry); // entry point, will not return
    entryPoint();

    MemoryFreeLibrary(handle);
}

void LoadFromFile(char *fpath)
{
    void *data;
    size_t size;

    data = ReadLibrary(fpath, &size);
    if (data == NULL)
    {
        return;
    }

    LoadFromMemory(data, size);

    free(data);
}