#include "spdlog/spdlog.h"
#include "asio.hpp"
#include <iostream>
#include <fstream>
#include "CLI11.hpp"
#include "../md5.cpp"
#include <vector>

using namespace std;
using namespace asio::ip;

vector<string> split(string line, char seperator) {
    vector<string> creds;
    string str;
    stringstream ss(line); 
    while (getline(ss, str, seperator)) 
        creds.push_back(str);
    return creds;
}

string eraseSpaces(string &str) {
   str.erase(remove(str.begin(), str.end(), ' '), str.end());
   return str;
}

string generateHashes(vector<string> creds, string nonce, string realm) {
    spdlog::info("Generating hashes");
    string ha1 = creds[0] + realm + creds[1];
    string ha2 = "GET" + creds[2];
    MD5 md5;
    string response = md5(md5(ha1)+nonce+md5(ha2));
    return response;
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
        spdlog::info("Reading creds from file");
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
        string response = generateHashes(creds, nonce, realm);
        stream << creds[0] << "," << response << endl;
        getline(stream, response);
        vector<string> responseServer = split(response, ',');
        if (responseServer[0] == "ACK") {
            cout << "Login war erfolgreich!" << endl;
        } else {
            cout << "Login war nicht erfolgreich!" << endl;
        }
    } else {
        spdlog::error("Could not connect to server");
    }
}