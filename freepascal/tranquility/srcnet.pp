unit srcnet;
	
interface
	uses bytebuffer;
	
	{$INCLUDE ws2_32.ph}

	type
		net_processoob_t = procedure(t : char; databuf : pbytebuffer_t; from : SOCKADDR_IN);

	procedure net_setoobhandle(func : net_processoob_t);
	
	function net_startup(ip : PCHAR; port : word) : boolean;
	function net_shutdown() : boolean;
	function net_sendoob(target : SOCKADDR_IN; buf : pbytebuffer_t) : boolean;
	
	procedure net_begin();

implementation
	var
		g_socket : TSOCKET;
		g_net_processoob : net_processoob_t = NIL;

	procedure net_setoobhandle(func : net_processoob_t);
	begin
		g_net_processoob := func;
	end;

	function net_startup(ip : PCHAR; port : word) : boolean;
	var
		winsockdata : WSADATA;
		local : SOCKADDR_IN;
	begin
		if WSAStartup(WINSOCK_VERSION, @winsockdata) <> 0 then
			exit(false);
			
		g_socket := socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		if g_socket = INVALID_SOCKET then
			exit(false);
			
		local.sin_family := AF_INET;
		local.sin_addr.S_un.S_addr := inet_addr(ip);
		local.sin_port := htons(port);
		
		if bind(g_socket, @local, sizeof(local)) <> 0 then
			exit(false);
			
		exit(true);
	end;

	function net_shutdown() : boolean;
	begin
		if WSACleanup() <> 0 then
			exit(false);
			
		exit(true);
	end;

	function net_sendoob(target : SOCKADDR_IN; buf : pbytebuffer_t) : boolean;
	begin
		if sendto(g_socket, PCHAR(bb_getdata(buf)), bb_getpos(buf), 0, @target, sizeof(target)) <= 0 then
			exit(false);
			
		exit(true);
	end;

	procedure net_begin();
	var
		data : array[1..2048] of char;
		databuf : bytebuffer_t;

		from : SOCKADDR_IN;
		fromlen : INT = sizeof(from);
	begin
		while true do
		begin
			databuf := bb_read_new(@data, recvfrom(g_socket, PCHAR(POINTER(@data)), 2048, 0, @from, @fromlen));

			if WSAGetLastError() <> 0 then
			begin
				writeln('WSAGetLastError: ', WSAGetLastError());

				break;
			end;

			if bb_read_num32(@databuf) = $FFFFFFFF then
			begin
				if g_net_processoob <> NIL then
					g_net_processoob(bb_read_char(@databuf), @databuf, from);
			end;
		end;
	end;
end.