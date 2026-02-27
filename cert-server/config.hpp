
#ifndef HTTPS_CONFIG_HPP
#define	HTTPS_CONFIG_HPP

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/bufferevent_ssl.h>
#include <event2/buffer.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define CERT_PEM "cert.pem"
#define KEY_PEM "key_pem"

#endif
