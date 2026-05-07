# NetLens - Multithreaded LAN Scanner in C

NetLens is a lightweight multithreaded LAN scanner written in C for Windows.  
It scans devices on your local network, checks common TCP ports, and displays which IP addresses have active services.

This project is built to practice low-level networking, sockets, multithreading, synchronization, and Windows API programming.

---

## Features

- Scan a local subnet like `192.168.1.1 - 192.168.1.254`
- Detect open TCP ports
- Use multithreading for faster scanning
- Uses Windows Winsock API
- Uses `CreateThread`, `CRITICAL_SECTION`, and optional condition variables
- Supports common LAN, development, database, and remote-access ports
- Clean terminal output
- Beginner-friendly C codebase

---

## Example Output

```txt
NetLens LAN Scanner

Target subnet: 192.168.1.0/24
Scanned range: 192.168.1.1 - 192.168.1.254
Mode: fast
Threads: 8
Timeout: 500ms

---------------------------------------------------------------
IP Address       Result       Open Ports
---------------------------------------------------------------
192.168.1.1      FOUND        80(HTTP), 443(HTTPS)
192.168.1.5      FOUND        445(SMB)
192.168.1.12     FOUND        8080(HTTP-ALT)
192.168.1.23     FOUND        3000(Node), 5173(Vite)
192.168.1.45     NO RESPONSE  -
---------------------------------------------------------------

Scan complete.
IPs scanned: 254
Devices/services found: 4
Open ports found: 6
