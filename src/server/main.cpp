#include "spdlog/spdlog.h"
#include "asio.hpp"
#include <chrono>
#include <cstring>
#include <vector>
#include "CLI11.hpp"
#include <random>
#include <string>

using namespace std;
using namespace asio::ip;


vector<string> getCreds(string line) {
    vector<string> creds;
    string str;
    stringstream ss(line); 
    while (getline(ss, str, ';')) 
        if (str != "LOGON") {
            creds.push_back(str);
        }
    return creds;
}

unsigned int generateNonce() {
    random_device rd; 
    mt19937 gen(rd()); 
    uniform_int_distribution<unsigned int> distrib(0, UINT_MAX);
    return distrib(gen);
}

int main(int argc, char* argv[]) { 
    spdlog::info("Digest server started");

    CLI::App app{"Daytime Server"};
    unsigned short port = 1113;
    app.add_option("-p, --port", port, "server port");

    CLI11_PARSE(app, argc, argv);

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
            vector<string> creds = getCreds(line);
            cout << creds[0] << creds[1] << creds[2] << endl;
            if (creds[2] == "/login.html") {
                unsigned int nonce = generateNonce();
                strm << "testRealm" << endl;
                strm << nonce << endl;
            }
        } else {
            spdlog::error("Could not connect to client");
        }
        strm.close(); 
    }
}