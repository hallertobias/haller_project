#include "spdlog/spdlog.h"
#include "asio.hpp"
#include <chrono>
#include <cstring>
#include "CLI11.hpp"
#include <string>

using namespace std;
using namespace asio::ip;

string* getCreds(string line) {
    char* charArr = new char[line.length()+1];
    strcpy(charArr, line.c_str());
    char *ptr; 
    ptr = strtok(charArr, ";");
    int counter = 0;
    string* creds = new string[3];
    while (ptr != NULL)  
    {  
        string s = ptr;
        creds[counter] = s;
        counter++;
        ptr = strtok(NULL, ";");  
    }  
    return creds;
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
            string* creds = getCreds(line);
            cout << creds[0] << endl;
        } else {
            spdlog::error("Could not connect to client");
        }
        strm.close(); 
    }
}