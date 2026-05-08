#include "netlens.h"

// A small list of common educational ports
const port_info_t EDUCATIONAL_PORTS[] = {{21, "FTP"},     {22, "SSH"},          {23, "Telnet"},
                                         {25, "SMTP"},    {53, "DNS"},          {80, "HTTP"},
                                         {110, "POP3"},   {143, "IMAP"},        {443, "HTTPS"},
                                         {3306, "MySQL"}, {5432, "PostgreSQL"}, {8080, "HTTP-ALT"}};

const size_t EDUCATIONAL_PORTS_COUNT = sizeof(EDUCATIONAL_PORTS) / sizeof(port_info_t);
