program tranquility_swap;

uses bytebuffer, srcnet;

{$INCLUDE srcnet.ph}

var
	client, server : SOCKADDR_IN;

procedure net_processoob(t : char; databuf : pbytebuffer_t; from : SOCKADDR_IN);
var
	reply : array[1..2048] of byte;
	replybuf : bytebuffer_t;
begin
	replybuf := bb_write_new(@reply, 2048);

	case t of
		'k':
		begin
		end;
	end;

	if bb_getpos(@replybuf) > 0 then
		net_sendoob(from, @replybuf);
end;

begin
	if not net_startup('192.168.1.9', 27015) then
		exit();

	net_begin();
	net_shutdown();

	writeln('Press any key to exit...');
	readln();
end.