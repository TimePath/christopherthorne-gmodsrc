#ifndef COMPONENT_SOURCENET_H
#define COMPONENT_SOURCENET_H

// Components
#include "component.h"

// C/C++ headers
#include <winsock2.h>

// SDK headers
#include "bitbuf.h"
#include "netadr.h"

// Default ports
#define CLIENT_PORT 27005
#define SERVER_PORT 27015

// Client/server info
extern char net_game_version[128];
extern unsigned int net_game_id;

extern netadr_t net_client_adr;
extern netadr_t net_server_adr;

extern int net_client_challenge;
extern int net_server_challenge;

// Timeout
extern int net_timeout;

// Socket
extern SOCKET net_socket;

// Net buffer
extern bf_write net_datagram;

bool NET_SendDatagram();
bool NET_ReceiveDatagram( const char *buf, int buflen );

// Component registration
CREATE_COMPONENT( SourceNet );

#endif // COMPONENT_SOURCENET_H