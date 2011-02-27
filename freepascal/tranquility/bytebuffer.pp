unit bytebuffer;

interface
	type
		PBYTE = ^byte;
		PWORD = ^word;
		plongword = ^longword;
		PQWORD = ^qword;
		PSTRING = PCHAR;

		bytebuffer_t = record
			data : PBYTE;
			size, pos : longword;
		end;
		
		pbytebuffer_t = ^bytebuffer_t;

	function bb_check(buf : pbytebuffer_t; val : word) : boolean;

	function bb_getpos(buf : pbytebuffer_t) : word;
	function bb_getdata(buf : pbytebuffer_t) : PCHAR;
	function bb_getdatasize(buf : pbytebuffer_t) : word;

	function bb_read_new(data : pointer; size : word) : bytebuffer_t;
	function bb_read_byte(buf : pbytebuffer_t) : byte;
	function bb_read_bytes(buf : pbytebuffer_t; data : PCHAR; bytes : word) : boolean;
	function bb_read_char(buf : pbytebuffer_t) : char;
	function bb_read_string(buf : pbytebuffer_t; pstr : PSTRING) : boolean;
	function bb_read_num16(buf : pbytebuffer_t) : word;
	function bb_read_num32(buf : pbytebuffer_t) : longword;
	function bb_read_num64(buf : pbytebuffer_t) : qword;

	function bb_write_new(data : pointer; size : word) : bytebuffer_t;
	function bb_write_byte(buf : pbytebuffer_t; val : byte) : boolean;
	function bb_write_bytes(buf : pbytebuffer_t; data : PCHAR; datasz : word) : boolean;
	function bb_write_char(buf : pbytebuffer_t; val : char) : boolean;
	function bb_write_string(buf : pbytebuffer_t; str : string) : boolean;
	function bb_write_num16(buf : pbytebuffer_t; num : word) : boolean;
	function bb_write_num32(buf : pbytebuffer_t; num : longword) : boolean;
	function bb_write_num64(buf : pbytebuffer_t; num : qword) : boolean;

implementation
	uses sysutils;

	function bb_new_internal(data : pointer; size : word; clean : boolean) : bytebuffer_t;
	var
		i : word;
	begin
		bb_new_internal.data := PBYTE(data);
		bb_new_internal.size := size;
		bb_new_internal.pos := 0;
		
		if clean then
		begin
			for i := 0 to size-1 do
				bb_new_internal.data[i] := 0;
		end;
	end;
		
	function bb_check(buf : pbytebuffer_t; val : word) : boolean;
	begin
		if buf^.pos + val > buf^.size then
			exit(false);
			
		exit(true);
	end;
	
	function bb_getpos(buf : pbytebuffer_t) : word;
	begin
		exit(buf^.pos);
	end;

	function bb_getdata(buf : pbytebuffer_t) : PCHAR;
	begin
		exit(PCHAR(buf^.data));
	end;
	
	function bb_getdatasize(buf : pbytebuffer_t) : word;
	begin
		exit(buf^.size);
	end;
	
	function bb_getdatasize(buf : bytebuffer_t) : word;
	begin
		exit(buf.pos);
	end;

	function bb_read_new(data : pointer; size : word) : bytebuffer_t;
	begin
		exit(bb_new_internal(data, size, false));
	end;

	function bb_read_byte(buf : pbytebuffer_t) : byte;
	begin
		if bb_check(buf, 1) = false then
			exit(0);

		buf^.pos := buf^.pos + 1;

		exit((buf^.data + buf^.pos - 1)^);
	end;
	
	function bb_read_bytes(buf : pbytebuffer_t; data : PCHAR; bytes : word) : boolean;
	var
		i : word;
	begin
		if bb_check(buf, bytes) = false then
			exit(false);

		for i := 0 to bytes-1 do
			data[i] := char(bb_read_byte(buf));
			
		exit(true);
	end;
	
	function bb_read_char(buf : pbytebuffer_t) : char;
	begin
		exit(char(bb_read_byte(buf)));
	end;
	
	function bb_read_string(buf : pbytebuffer_t; pstr : PSTRING) : boolean;
	var
		c : char;
		i : byte = 1;
	begin
		while bb_check(buf, 1) do
		begin
			c := bb_read_char(buf);

			if c = chr(0) then
			begin
				pstr[0] := chr(i-1);

				exit(true);
			end;

			pstr[i] := c;

			i := i + 1;
		end;
		
		exit(false);
	end;

	function bb_read_num_internal(buf : pbytebuffer_t; bytes : byte) : POINTER;
	begin
		if bb_check(buf, bytes) = false then
			exit(NIL);
			
		buf^.pos := buf^.pos + bytes;
		
		exit(buf^.data + buf^.pos - bytes);
	end;

	function bb_read_num16(buf : pbytebuffer_t) : word;
	begin
		exit(PWORD(bb_read_num_internal(buf, 2))^);
	end;

	function bb_read_num32(buf : pbytebuffer_t) : longword;
	begin
		exit(PQWORD(bb_read_num_internal(buf, 4))^);
	end;
	
	function bb_read_num64(buf : pbytebuffer_t) : qword;
	begin
		exit(PQWORD(bb_read_num_internal(buf, 8))^);
	end;

	function bb_write_new(data : pointer; size : word) : bytebuffer_t;
	begin
		exit(bb_new_internal(data, size, true));
	end;

	function bb_write_byte(buf : pbytebuffer_t; val : byte) : boolean;
	begin
		if bb_check(buf, 1) = false then
			exit(false);
		
		(buf^.data + buf^.pos)^ := val;
	
		buf^.pos := buf^.pos + 1;

		exit(true);
	end;
	
	function bb_write_bytes(buf : pbytebuffer_t; data : PCHAR; datasz : word) : boolean;
	var
		i : word;
	begin
		for i := 0 to datasz-1 do
		begin
			if bb_write_char(buf, data[i]) = false then
				exit(false);
		end;
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