# NetLens

NetLens is a local lab TCP connection checker designed for educational purposes. It helps in learning POSIX sockets, pthreads, command-line parsing, IP address validation, timeout handling, and clean C project architecture.

## ⚠️ Responsible Use

**This tool is strictly for educational purposes and private local lab environments.** 
It should only be used on private local networks, personal devices, and virtual machines that you own or have explicit permission to test. 

It does not include and will not support:
- Stealth scanning or raw sockets
- SYN scanning or vulnerability detection
- Banner grabbing or exploitation
- Brute forcing or credential testing
- Public internet scanning

Please see [docs/responsible_use.md](docs/responsible_use.md) for full details.

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
./netlens <ip_start> <ip_end>
```
