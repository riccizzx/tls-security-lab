@echo off
cd /d %~dp0

if not exist cert-server.exe (
    echo ERROR: cert-server.exe not found!
    pause
    exit /b
)

echo Starting HTTPS server on port 8443...
cert-server.exe 8443 localhost+2.pem localhost+2-key.pem

if %errorlevel% neq 0 (
    echo Server crashed with error %errorlevel%
)

pause