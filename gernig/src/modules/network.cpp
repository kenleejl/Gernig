#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <shellapi.h>


using namespace std;


string generate_ip(){
    string randip;
    for (int i = 0; i < 3; i++)
        randip += to_string(rand() % 255 + 1) + ".";
    randip += to_string(rand() % 255 + 1);

    return randip;
}

string generate_cmd(string ip){
    string fin_cmd = "/c ";
    string port = to_string(rand() % 65535 + 1);
    switch(rand() % 5){
        case(0):
            // SSH
            fin_cmd += "ssh " + ip + " -p " + port;
            break;
        case(1):
            // NC
            fin_cmd += "netcat " + ip + " " + port;
            break;
        case(2):
            // FTP
            fin_cmd += "ftp " + ip;
            break;
        case(3):
            // CURL
            fin_cmd += "curl " + ip;
            break;
        case(4):
            // PING
            fin_cmd += "ping " + ip;
            break;
    }
    return fin_cmd;
}

int main(){
    string temp;
    while(1){
        temp = generate_cmd(generate_ip());
        cout << temp << endl;
        ShellExecute(NULL, "open", "cmd", temp.c_str(), NULL, 0);
        Sleep(5000);
    }
}