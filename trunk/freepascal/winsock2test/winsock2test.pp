program winsock2test;

uses bytebuffer;

{$INCLUDE ws2_32.pp}

var
	s : TSOCKET;
	winsockdata : WSAData;
	localaddr, targetaddr : SOCKADDR_IN;

function sendbuftest(s : TSOCKET; target : SOCKADDR_IN) : boolean;
var
	data : array[1..200] of char;
	databuf : bytebuffer_t;
begin
	databuf := bb_create(@data, 200);
	
	bb_write_byte(@databuf, 5);
	bb_write_byte(@databuf, 6);
	bb_write_string(@databuf, 'test');
	bb_write_num16(@databuf, 2000);
	
	writeln('Wrote ', bb_getbufferpos(@databuf), ' bytes');

	exit(sendto(s, bb_getdata(@databuf), bb_getbufferpos(@databuf), 0, @target, sizeof(SOCKADDR_IN)) > 0);
end;

begin
	if WSAStartup(WINSOCK_VERSION, @winsockdata) = 0 then
	begin
		writeln('WSAStartup(...) succeeded');

		s := socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		if s <> INVALID_SOCKET then
		begin
			localaddr.sin_family := AF_INET;
			localaddr.sin_addr.S_un.S_addr := inet_addr('192.168.1.9');
			localaddr.sin_port := htons(27025);

			if bind(s, @localaddr, sizeof(SOCKADDR_IN)) = 0 then
			begin
				writeln('bind(...) succeeded');
				
				targetaddr.sin_family := AF_INET;
				targetaddr.sin_addr.S_un.S_addr := inet_addr('86.3.55.232');
				targetaddr.sin_port := htons(27015);

				sendbuftest(s, targetaddr);
			end;
		end;
	
		WSACleanup();
	end;

	readln();
end.