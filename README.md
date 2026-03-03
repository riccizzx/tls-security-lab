
Experimental environment for studying TLS, applied cryptography, and network security.

This project implements an HTTPS server in C++ using libevent and OpenSSL, for educational purposes and experimental analysis of security protocols and cryptographically controlled environments.

The server allows:
- Manual TLS implementation with OpenSSL
- Custom configuration of versions and cipher suites
- Testing with self-signed certificates and local CA (mkcert)
- Analysis of the TLS handshake with Wireshark
- Simulation of SSL configuration errors
- Experimentation with certificate validation

The project's objective is to deepen the practical understanding of:
- TLS protocol structure
- Public Key Infrastructure (PKI)
- X.509 certificate verification
- Transport security in network applications

Technologies used:
-C++
-Libevent
-OpenSSL
-Winsock (Windows)
-mkcert

CERTIFICATE:
you can use the CA you want, in this case im using mkcert to generate private key and certificate.
include your certificates in debug and change them into .bat file.

See the mkcert guide install, to generate the valid certficate and private-key

```
cert-server.exe <port> <certificate.pem> <key.pem>
```

Libevent: https://github.com/libevent/libevent
OpenSSL: https://github.com/openssl/openssl
mkcert: https://github.com/FiloSottile/mkcert
