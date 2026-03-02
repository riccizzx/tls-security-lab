
very simple https implementation in cpp using libevent and openssl.

CERTIFICATE:
you can use the CA you want, in this case im using mkcert to generate private key and certificate.
include your certificates in debug and change them into .bat file.

```
cert-server.exe <port> <certificate.pem> <key.pem>
```

mkcert: https://github.com/FiloSottile/mkcert
