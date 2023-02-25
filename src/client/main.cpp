#include "spdlog/spdlog.h"
#include "asio.hpp"
#include <iostream>
#include "CLI11.hpp"

using namespace std;
using namespace asio::ip;

int main(int argc, char* argv[]) {
    CLI::App app{"Digest Client"};

    string username;
    string password;
    string digestURI;
    app.add_option("username", username);
    app.add_option("password", password);
    app.add_option("digestURI", digestURI);


    CLI11_PARSE(app, argc, argv);

    tcp::iostream stream("localhost", "1113");

    if (stream) {
        while(true) {
            stream << "LOGON;user;password;test@test.com" << endl;
        }
    } else {
        spdlog::error("Could not connect to server");
    }
}