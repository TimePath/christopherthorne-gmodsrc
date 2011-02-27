{$define WS2_32_DLL := stdcall;external 'ws2_32.dll'}

{$define WINSOCK_VERSION := 514} (*2.2*)

{$define AF_INET := 2}

{$define SOCK_STREAM := 1}
{$define SOCK_DGRAM := 2}

{$define IPPROTO_TCP := 6}
{$define IPPROTO_UDP := 17}

{$define INVALID_SOCKET := TSOCKET(0)}
{$define SOCKET_ERROR := -1}

type
	(*BEGIN C ALIASES*)
	UCHAR = byte;
	U_CHAR = byte;

	USHORT = word;
	U_SHORT = word;

	ULONG = longint;
	U_LONG = longint;

	INT = longint;
	(*END C ALIASES*)

	TSOCKET = ^ULONG;
	
	WSAData = record
		version : WORD;
		high_version : WORD;
		
		{$ifdef win64}
		max_sockets : USHORT;
		max_udp_datagram : USHORT;
		vendor_info : PCHAR;
		description : array[0..256] of char;
		system_status : array[0..128] of char;
		{$else}
		description : array[0..256] of char;
		system_status : array[0..128] of char;
		max_sockets : USHORT;
		max_udp_datagram : USHORT;
		vendor_info : PCHAR;
		{$endif}
	end;
	
	LPWSADATA = ^WSAData;
	
	ADDRESS_FAMILY = USHORT;

	IN_ADDR = record
		S_un : record
			case integer of
			0:
			(
				S_un_b : record
					s_b1, s_b2, s_b3, s_b4 : U_CHAR;
				end;
			);
			1:
			(
				S_un_w : record
					s_w1, s_w2 : U_SHORT;
				end;
			);
			2:
			(
				S_addr : U_LONG
			);
		end;
	end;

	SOCKADDR_IN = record
		sin_family : ADDRESS_FAMILY;
		sin_port : USHORT;
		sin_addr : IN_ADDR;
		sin_zero : array[0..7] of char;
	end;
	
	LPSOCKADDR_IN = ^SOCKADDR_IN;

function WSAStartup(version : WORD; data : LPWSADATA) : INT;WS2_32_DLL;
function WSACleanup() : INT;WS2_32_DLL;
function WSAGetLastError() : INT;WS2_32_DLL;

function socket(af : longint; stype : longint; protocol : longint) : TSOCKET;WS2_32_DLL;
function closesocket(s : TSOCKET) : INT;WS2_32_DLL;

function bind(s : TSOCKET; name : LPSOCKADDR_IN; namelen : INT) : INT;WS2_32_DLL;

function sendto(s : TSOCKET; buf : PCHAR; len : INT; flags : INT; toaddr : LPSOCKADDR_IN; tolen : INT) : INT;WS2_32_DLL;

function inet_addr(cp : PCHAR) : ULONG;WS2_32_DLL;
function inet_ntoa(inaddr : IN_ADDR) : PCHAR;WS2_32_DLL;

function htons(hostshort : U_SHORT) : U_SHORT;WS2_32_DLL;
function ntohs(netshort : U_SHORT) : U_SHORT;WS2_32_DLL;