#include "spdlog/spdlog.h"
#include "asio.hpp"
#include <vector>
#include "CLI11.hpp"
#include <random>
#include <string>
#include "../md5.cpp"

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

unsigned int generateNonce() {
    spdlog::info("Generating nonce");
    random_device rd; 
    mt19937 gen(rd()); 
    uniform_int_distribution<unsigned int> distrib(0, UINT_MAX);
    return distrib(gen);
}

string generateHashes(string username, string password, string uri, string realm, unsigned int nonce) {
    spdlog::info("Generating hashes for nonce: " + to_string(nonce));
    string ha1 = username + realm + password;
    string ha2 = "GET" + uri;
    MD5 md5;
    string responseCalc = md5(md5(ha1)+to_string(nonce)+md5(ha2));
    return responseCalc;
}

int main(int argc, char* argv[]) { 
    spdlog::info("Digest server started");

    CLI::App app{"Daytime Server"};
    unsigned short port = 1113;
    app.add_option("-p, --port", port, "server port");

    CLI11_PARSE(app, argc, argv);

    string username = "username";
    string password = "password";
    string realm = "testRealm";
    string uri = "/login.html";

    asio::io_context ctx;
    tcp::endpoint ep{tcp::v4(), port};
    tcp::acceptor acceptor{ctx, ep}; 
    acceptor.listen();
    tcp::socket sock{ctx};
    while(true) {
        acceptor.accept(sock);
        tcp::iostream strm{move(sock)};
        if(strm) {
            string line;
            getline(strm, line);
            vector<string> request = split(line, ' ');
            if (request[0] == "LOGON" && request[1] == uri) {
                unsigned int nonce = generateNonce();
                strm << realm << endl;
                strm << nonce << endl;
                getline(strm, line);
                vector<string> response = split(line, ',');
                if (response[0] == username) {
                    string responseCalc = generateHashes(username, password, uri, realm, nonce);
                    if (response[1] == responseCalc) {
                        spdlog::info("Authentification was succesful for uri " + uri + "!");
                        strm << "ACK" << "," << nonce << endl;
                    } else {
                        spdlog::info("Authentification was not succesful for uri " + uri + "!");
                        strm << "NAK" << endl;
                    }
                }
            } else {
                spdlog::info("False request!");
            }
        } else {
            spdlog::error("Could not connect to client");
        }
        strm.close(); 
    }
}