unit bytebuffer;

interface
	type
		PBYTE = ^byte;

		bytebuffer_t = record
			data : PBYTE;
			size, pos : longword;
		end;
		
		pbytebuffer_t = ^bytebuffer_t;

	function bb_create(data : pointer; size : word) : bytebuffer_t;
	function bb_check(buf : pbytebuffer_t; val : word) : boolean;
	function bb_getdata(buf : pbytebuffer_t) : PCHAR;
	function bb_getdatasize(buf : pbytebuffer_t) : word;
	function bb_getbufferpos(buf : pbytebuffer_t) : word;

	(*Read*)
	(*TODO*)
	
	(*Write*)
	function bb_write_byte(buf : pbytebuffer_t; val : byte) : boolean;
	function bb_write_char(buf : pbytebuffer_t; val : char) : boolean;
	function bb_write_string(buf : pbytebuffer_t; str : string) : boolean;
	function bb_write_num16(buf : pbytebuffer_t; num : word) : boolean;
	function bb_write_num32(buf : pbytebuffer_t; num : longword) : boolean;
	function bb_write_num64(buf : pbytebuffer_t; num : qword) : boolean;

implementation
	function bb_create(data : pointer; size : word) : bytebuffer_t;
	begin
		bb_create.data := PBYTE(data);
		bb_create.size := size;
		bb_create.pos := 0;
	end;
	
	function bb_check(buf : pbytebuffer_t; val : word) : boolean;
	begin
		if buf^.pos + val > buf^.size then
			exit(false);
			
		exit(true);
	end;
	
	function bb_getdata(buf : pbytebuffer_t) : PCHAR;
	begin
		exit(PCHAR(buf^.data));
	end;
	
	function bb_getdatasize(buf : pbytebuffer_t) : word;
	begin
		exit(buf^.size);
	end;
	
	function bb_getbufferpos(buf : pbytebuffer_t) : word;
	begin
		exit(buf^.pos);
	end;
	
	function bb_getdatasize(buf : bytebuffer_t) : word;
	begin
		exit(buf.pos);
	end;

	function bb_write_byte(buf : pbytebuffer_t; val : byte) : boolean;
	begin
		if bb_check(buf, 1) = false then
			exit(false);
		
		(buf^.data + buf^.pos)^ := val;
	
		buf^.pos := buf^.pos + 1;

		exit(true);
	end;
	
	function bb_write_char(buf : pbytebuffer_t; val : char) : boolean;
	begin
		exit(bb_write_byte(buf, byte(val)));
	end;

	function bb_write_string(buf : pbytebuffer_t; str : string) : boolean;
	var
		i : word;
	begin
		for i := 1 to length(str) do
		begin
			if bb_write_char(buf, str[i]) = false then
				exit(false);
		end;
		
		if bb_write_char(buf, char(0)) = false then
			exit(false);
		
		exit(true);
	end;
	
	function bb_write_num_internal(buf : pbytebuffer_t; pnum : pointer; bytes : byte) : boolean;
	var
		i : word;
	begin
		for i := 0 to bytes-1 do
		begin
			if bb_write_byte(buf, byte((pnum + i)^)) = false then
				exit(false);
		end;
		
		exit(true);
	end;

	function bb_write_num16(buf : pbytebuffer_t; num : word) : boolean;
	begin
		exit(bb_write_num_internal(buf, @num, 2));
	end;

	function bb_write_num32(buf : pbytebuffer_t; num : longword) : boolean;
	begin
		exit(bb_write_num_internal(buf, @num, 4));
	end;

	function bb_write_num64(buf : pbytebuffer_t; num : qword) : boolean;
	begin
		exit(bb_write_num_internal(buf, @num, 8));
	end;
end.