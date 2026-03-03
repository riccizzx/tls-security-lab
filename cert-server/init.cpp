
#include "config.hpp"

/*
    init winsock api and set ssl 
*/

bool start_winsock(WSADATA& wsa)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return false;
    }
    return true;
}

// get openSSL errors
void print_ssl_error(const char* msg)
{
    std::cerr << msg << std::endl;
    ERR_print_errors_fp(stderr);
}