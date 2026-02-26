
#include "config.hpp"

// ----------------------------------------------------
// Utility: Print OpenSSL errors
// ----------------------------------------------------
void print_ssl_error(const char* msg)
{
    std::cerr << msg << std::endl;
    ERR_print_errors_fp(stderr);
}

// ----------------------------------------------------
// HTTPS bufferevent callback
// ----------------------------------------------------
static bufferevent* https_bev_cb(event_base* base, void* arg)
{
    SSL_CTX* ctx = static_cast<SSL_CTX*>(arg);

    SSL* ssl = SSL_new(ctx);
    if (!ssl)
    {
        print_ssl_error("SSL_new() failed");
        return nullptr;
    }

    bufferevent* bev = bufferevent_openssl_socket_new(
        base,
        -1,
        ssl,
        BUFFEREVENT_SSL_ACCEPTING,
        BEV_OPT_CLOSE_ON_FREE);

    return bev;
}

// ----------------------------------------------------
// HTTP Request Handler
// ----------------------------------------------------
static void handle_request(evhttp_request* req, void*)
{
    if (!req)
        return;

    evbuffer* reply = evbuffer_new();
    if (!reply)
        return;

    const char* uri = evhttp_request_get_uri(req);
    const char* host = evhttp_request_get_host(req);

    std::cout << "[+] Request received\n";
    std::cout << "    URI  : " << (uri ? uri : "null") << "\n";
    std::cout << "    Host : " << (host ? host : "null") << "\n";

    evbuffer_add_printf(reply,
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>Libevent HTTPS</title></head>"
        "<body>"
        "<h1>HTTPS Server Running on Windows</h1>"
        "</body>"
        "</html>");

    evhttp_add_header(
        evhttp_request_get_output_headers(req),
        "Content-Type",
        "text/html");

    evhttp_send_reply(req, HTTP_OK, "OK", reply);

    evbuffer_free(reply);
}

// ----------------------------------------------------
// Signal Handler (Windows-friendly)
// ----------------------------------------------------
static void signal_cb(evutil_socket_t, short, void* arg)
{
    event_base* base = static_cast<event_base*>(arg);
    std::cout << "\n[!] Shutdown signal received\n";
    event_base_loopbreak(base);
}

// ----------------------------------------------------
// MAIN
// ----------------------------------------------------
int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: server.exe <port> <cert.pem> <key.pem>\n";
        return 1;
    }

    // ------------------------------------------------
    // WinSock Initialization
    // ------------------------------------------------
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // ------------------------------------------------
    // OpenSSL Initialization (OpenSSL 1.1+/3.x safe)
    // ------------------------------------------------
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx)
    {
        print_ssl_error("SSL_CTX_new failed");
        return 1;
    }

    // Enforce TLS 1.2+
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);

    if (SSL_CTX_use_certificate_file(ctx, argv[2], SSL_FILETYPE_PEM) <= 0)
    {
        print_ssl_error("Loading certificate failed");
        return 1;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, argv[3], SSL_FILETYPE_PEM) <= 0)
    {
        print_ssl_error("Loading private key failed");
        return 1;
    }

    if (!SSL_CTX_check_private_key(ctx))
    {
        std::cerr << "Private key does not match certificate\n";
        return 1;
    }

    // ------------------------------------------------
    // Libevent Setup
    // ------------------------------------------------
    event_base* base = event_base_new();
    if (!base)
    {
        std::cerr << "event_base_new failed\n";
        return 1;
    }

    evhttp* http = evhttp_new(base);
    if (!http)
    {
        std::cerr << "evhttp_new failed\n";
        return 1;
    }

    evhttp_set_bevcb(http, https_bev_cb, ctx);
    evhttp_set_gencb(http, handle_request, nullptr);

    evhttp_set_timeout(http, 30); // 30s timeout

    int port = std::stoi(argv[1]);

    if (evhttp_bind_socket(http, "0.0.0.0", port) != 0)
    {
        std::cerr << "Bind failed\n";
        return 1;
    }

    // Windows CTRL+C handler
    //event* sig = evsignal_new(base, SIGINT, signal_cb, base);
    //event_add(sig, nullptr);

    std::cout << "[+] HTTPS Server running on port " << port << "\n";

    event_base_dispatch(base);

    // ------------------------------------------------
    // Cleanup
    // ------------------------------------------------
    evhttp_free(http);
    //event_free(sig);
    event_base_free(base);
    SSL_CTX_free(ctx);
    EVP_cleanup();
    WSACleanup();

    std::cout << "[+] Server stopped cleanly\n";
    return 0;
}