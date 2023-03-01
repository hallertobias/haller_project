#include "spdlog/spdlog.h"
#include "asio.hpp"
#include <iostream>
#include <fstream>
#include "CLI11.hpp"
#include "../md5.cpp"
#include <vector>

using namespace std;
using namespace asio::ip;

string eraseSpaces(string &str) {
   str.erase(remove(str.begin(), str.end(), ' '), str.end());
   return str;
}

int main(int argc, char* argv[]) {
    CLI::App app{"Digest Client"};

    string credentials = "";
    unsigned short port = 1113;
    app.add_option("-p, --port", port, "port to connect to");
    app.add_option("-c, --credentials", credentials, "username;password;URI");
    CLI11_PARSE(app, argc, argv);

    vector<string> creds;
    if (credentials.length() == 0) {
        fstream file;
        file.open("../credentials.txt",ios::in);
        if (file.is_open()){ 
            string tp;
            while(getline(file, tp)){ 
                string part = tp.substr(tp.find(":")+1, tp.length());
                part = eraseSpaces(part);
                creds.push_back(part);
                credentials += part + ";";
            }
            file.close(); 
        }
        credentials = credentials.substr(0, credentials.length()-1);
    }

    tcp::iostream stream("localhost", to_string(port));

    if (stream) {
        stream << "LOGON " << creds[2] << endl;
        string realm;
        getline(stream, realm);
        string nonce;
        getline(stream, nonce);
        string ha1 = creds[0] + realm + creds[1];
        string ha2 = "GET" + creds[2];
        MD5 md5;
        string response = md5(md5(ha1)+nonce+md5(ha2));
        stream << creds[0] << "," << response << endl;
    } else {
        spdlog::error("Could not connect to server");
    }
}