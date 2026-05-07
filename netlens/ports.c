

// Important and common TCP ports for a LAN / local network scanner.
// These help detect routers, web servers, dev servers, databases,
// Windows services, Linux services, printers, IoT devices, and remote access tools.

int common_ports[] = {
    20,     // FTP Data - transfers file data in active FTP mode
    21,     // FTP Control - file transfer login/commands
    22,     // SSH - secure remote shell, Linux servers, Raspberry Pi
    23,     // Telnet - old insecure remote shell, sometimes old routers/IoT
    25,     // SMTP - mail server sending
    53,     // DNS - domain name service
    67,     // DHCP Server - gives IP addresses to devices
    68,     // DHCP Client - receives IP address from router
    69,     // TFTP - simple file transfer, routers/firmware/network boot
    80,     // HTTP - normal web server, router admin page, ESP32 web server
    110,    // POP3 - email receiving
    123,    // NTP - time synchronization
    135,    // Microsoft RPC - Windows networking/service control
    137,    // NetBIOS Name Service - old Windows name discovery
    138,    // NetBIOS Datagram - old Windows network messaging
    139,    // NetBIOS Session - old Windows file/printer sharing
    143,    // IMAP - email receiving
    161,    // SNMP - router/printer/network device monitoring
    389,    // LDAP - directory service, authentication systems
    443,    // HTTPS - secure web server, secure router admin page
    445,    // SMB - Windows file sharing
    465,    // SMTPS - secure SMTP mail sending
    500,    // IKE/IPsec - VPN-related
    515,    // LPD/LPR - printer service
    587,    // SMTP Submission - email sending by clients
    631,    // IPP - Internet Printing Protocol
    993,    // IMAPS - secure IMAP email
    995,    // POP3S - secure POP3 email

    1433,   // Microsoft SQL Server database
    1521,   // Oracle Database
    1723,   // PPTP VPN
    1883,   // MQTT - IoT messaging, ESP32/IoT devices
    2049,   // NFS - Linux/Unix network file sharing
    2375,   // Docker API without TLS
    2376,   // Docker API with TLS
    2483,   // Oracle DB listener
    2484,   // Oracle DB listener over SSL
    3000,   // Node.js/React/Next.js dev server
    3306,   // MySQL/MariaDB database
    3389,   // RDP - Windows Remote Desktop
    4000,   // dev server / local web apps
    4200,   // Angular dev server
    5000,   // Flask/local dev server
    5001,   // alternate Flask/local dev server
    5432,   // PostgreSQL database
    5601,   // Kibana dashboard
    5672,   // RabbitMQ messaging
    5900,   // VNC remote desktop
    5985,   // WinRM HTTP - Windows remote management
    5986,   // WinRM HTTPS - Windows remote management
    6379,   // Redis database/cache
    6443,   // Kubernetes API server
    7000,   // local/dev services
    7474,   // Neo4j browser
    7687,   // Neo4j Bolt protocol
    8000,   // Python HTTP server/Django/local dev server
    8008,   // alternate HTTP/local services
    8080,   // alternate HTTP/proxy/Tomcat/ESP32 web server
    8081,   // alternate HTTP/dev server
    8443,   // alternate HTTPS
    8888,   // Jupyter Notebook/local web tools
    9000,   // MinIO/PHP-FPM/dev tools/admin panels
    9090,   // Prometheus/admin dashboard
    9200,   // Elasticsearch
    9300,   // Elasticsearch cluster communication
    10000,  // Webmin/server admin panel
    11211,  // Memcached cache server
    15672,  // RabbitMQ management dashboard
    27017,  // MongoDB database
    27018,  // MongoDB shard/alternate instance
    27019   // MongoDB config server
};

int common_port_count = sizeof(common_ports) / sizeof(common_ports[0]);

// Fast scan ports: good balance between speed and useful detection.

int fast_ports[] = {
    21,     // FTP - file transfer
    22,     // SSH - secure remote shell
    23,     // Telnet - old remote shell
    53,     // DNS - domain name service
    80,     // HTTP - web server/router/ESP32
    135,    // Windows RPC
    139,    // NetBIOS - old Windows sharing
    443,    // HTTPS - secure web server
    445,    // SMB - Windows file sharing
    1883,   // MQTT - IoT messaging
    3000,   // Node/React/Next.js dev server
    3306,   // MySQL/MariaDB
    3389,   // Windows Remote Desktop
    5000,   // Flask/dev server
    5173,   // Vite dev server
    5432,   // PostgreSQL
    5900,   // VNC remote desktop
    6379,   // Redis
    8000,   // Python/Django/dev server
    8080,   // alternate HTTP/Tomcat/ESP32
    8081,   // alternate HTTP/dev server
    8443,   // alternate HTTPS
    8888,   // Jupyter Notebook
    9000,   // MinIO/dev/admin tools
    9090,   // Prometheus
    9200,   // Elasticsearch
    27017   // MongoDB
};

int fast_port_count = sizeof(fast_ports) / sizeof(fast_ports[0]);

// Unique common TCP ports for LAN scanner.
// No port is repeated.

int all_ports[] = {
    20,     // FTP Data
    21,     // FTP Control
    22,     // SSH
    23,     // Telnet
    25,     // SMTP
    53,     // DNS
    67,     // DHCP Server
    68,     // DHCP Client
    69,     // TFTP
    80,     // HTTP
    110,    // POP3
    123,    // NTP
    135,    // Microsoft RPC
    137,    // NetBIOS Name Service
    138,    // NetBIOS Datagram
    139,    // NetBIOS Session
    143,    // IMAP
    161,    // SNMP
    389,    // LDAP
    443,    // HTTPS
    445,    // SMB
    465,    // SMTPS
    500,    // IKE/IPsec VPN
    515,    // LPD/LPR Printer
    587,    // SMTP Submission
    631,    // IPP Printer
    993,    // IMAPS
    995,    // POP3S

    1433,   // Microsoft SQL Server
    1521,   // Oracle Database
    1723,   // PPTP VPN
    1883,   // MQTT IoT Messaging
    2049,   // NFS
    2375,   // Docker API without TLS
    2376,   // Docker API with TLS
    2483,   // Oracle DB Listener
    2484,   // Oracle DB Listener SSL
    3000,   // Node.js / React / Next.js Dev Server
    3306,   // MySQL / MariaDB
    3389,   // Windows Remote Desktop
    4000,   // Local Dev Server
    4200,   // Angular Dev Server
    5000,   // Flask / Dev Server
    5001,   // Alternate Flask / Dev Server
    5173,   // Vite Dev Server
    5432,   // PostgreSQL
    5601,   // Kibana
    5672,   // RabbitMQ
    5900,   // VNC Remote Desktop
    5985,   // WinRM HTTP
    5986,   // WinRM HTTPS
    6379,   // Redis
    6443,   // Kubernetes API Server
    7000,   // Local Dev Service
    7474,   // Neo4j Browser
    7687,   // Neo4j Bolt
    8000,   // Python HTTP / Django / Dev Server
    8008,   // Alternate HTTP
    8080,   // Alternate HTTP / Tomcat / ESP32
    8081,   // Alternate HTTP / Dev Server
    8443,   // Alternate HTTPS
    8888,   // Jupyter Notebook
    9000,   // MinIO / Admin Panel / Dev Tool
    9090,   // Prometheus
    9200,   // Elasticsearch
    9300,   // Elasticsearch Cluster
    10000,  // Webmin
    11211,  // Memcached
    15672,  // RabbitMQ Management
    27017,  // MongoDB
    27018,  // MongoDB Alternate
    27019   // MongoDB Config Server
};

int port_count = sizeof(all_ports) / sizeof(all_ports[0]);