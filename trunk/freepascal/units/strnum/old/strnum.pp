unit strnum;

interface
	type
		strnum_t = record
			m_chNum : array[1..4096] of char;
			m_nLength : word;
		end;
		
		pstrnum_t = ^strnum_t;
		
	const
		CRLF = #13 + #10;
	
	function cmpstrnum(n1, n2 : strnum_t) : boolean;

	function getstrnumlen(n : strnum_t) : word;
	procedure setstrnumlen(pn : pstrnum_t; len : word);

	function getstrnumarrayvalue_chr(n : strnum_t; pos : word) : char;
	function getstrnumunit_chr(n : strnum_t; pos : word) : char;
	procedure setstrnumunit_chr(pn : pstrnum_t; val : char; pos : word);

	function getstrnumarrayvalue_int(n : strnum_t; pos : word) : word;
	function getstrnumunit_int(n : strnum_t; pos : word) : word;
	procedure setstrnumunit_int(pn : pstrnum_t; val, pos : word);

	function createstrnum(str : string) : strnum_t;

	function addstrnum(n1, n2 : strnum_t) : strnum_t;
	function substrnum(n1, n2 : strnum_t) : strnum_t;
	function multstrnum(n1, n2 : strnum_t) : strnum_t;

	function shoutstrnum(n : strnum_t) : strnum_t;

	procedure writestrnum(bn : strnum_t);
	procedure writestrnumln(bn : strnum_t);
	
implementation
	function floor10(num : word) : word;
	begin
		while (num mod 10) <> 0 do
			num := num - 1;
			
		floor10 := num;
	end;

	function getmin(n1, n2 : word) : word;
	begin
		if (n1 <= n2) then
			getmin := n1
		else
			getmin := n2;
	end;

	function getmax(n1, n2 : word) : word;
	begin
		if (n1 >= n2) then
			getmax := n1
		else
			getmax := n2;
	end;

	function cmpstrnum(n1, n2 : strnum_t) : boolean;
	var
		len1, len2, i : word;
	begin
		len1 := getstrnumlen(n1);
		len2 := getstrnumlen(n2);

		if (len1 <> len2) then
			cmpstrnum := false
		else
		begin
			cmpstrnum := true;

			for i := 1 to len1 do
			begin
				if (getstrnumunit_chr(n1, i) <> getstrnumunit_chr(n2, i)) then
				begin
					cmpstrnum := false;
					
					break;
				end;
			end;
		end;
	end;

	function getstrnumlen(n : strnum_t) : word;
	begin
		getstrnumlen := n.m_nLength;
	end;

	procedure setstrnumlen(pn : pstrnum_t; len : word);
	begin
		pn^.m_nLength := len;
	end;

	function getstrnumarrayvalue_chr(n : strnum_t; pos : word) : char;
	begin
		if ((pos > 0) and (pos <= getstrnumlen(n))) then
			getstrnumarrayvalue_chr := n.m_chNum[pos]
		else
			getstrnumarrayvalue_chr := '0';
	end;

	function getstrnumunit_chr(n : strnum_t; pos : word) : char;
	begin
		getstrnumunit_chr := getstrnumarrayvalue_chr(n, getstrnumlen(n)-pos+1);
	end;

	procedure setstrnumunit_chr(pn : pstrnum_t; val : char; pos : word);
	var
		len, key : word;
	begin
		len := getstrnumlen(pn^);
		key := len-pos+1;

		if ((key > 0) and (key <= len)) then
			pn^.m_chNum[key] := val;
	end;
	
	function getstrnumarrayvalue_int(n : strnum_t; pos : word) : word;
	begin
		getstrnumarrayvalue_int := ord(getstrnumarrayvalue_chr(n, pos))-48;
	end;

	function getstrnumunit_int(n : strnum_t; pos : word) : word;
	begin
		getstrnumunit_int := getstrnumarrayvalue_int(n, getstrnumlen(n)-pos+1);
	end;

	procedure setstrnumunit_int(pn : pstrnum_t; val, pos : word);
	begin
		setstrnumunit_chr(pn, chr(val+48), pos);
	end;
	
	function createstrnum(str : string) : strnum_t;
	var
		i : word;
	begin
		setstrnumlen(@createstrnum, length(str));
		
		for i := 1 to getstrnumlen(createstrnum) do
			setstrnumunit_chr(@createstrnum, str[i], i);
	end;

	function addstrnum(n1, n2 : strnum_t) : strnum_t;
	var
		i, sum , col : word;
		j : word = 0;
		carry : word = 0;
	begin
		for i := 1 to getmax(getstrnumlen(n1), getstrnumlen(n2))+1 do
		begin
			sum := getstrnumarrayvalue_int(n1, i) + getstrnumarrayvalue_int(n2, i) + carry;
			col := sum-floor10(sum);
			carry := (sum-col) div 10;

			j := j + 1;

			addstrnum.m_chNum[j] := chr(col+48);
		end;

		setstrnumlen(@addstrnum, j);
		
		for i := 1 to j do (*Strip meaningless zeroes*)
		begin
			if (getstrnumunit_chr(addstrnum, i) <> '0') then
				break;
					
			j := j - 1;
		end;

		setstrnumlen(@addstrnum, j);	
	end;

	function substrnum(n1, n2 : strnum_t) : strnum_t;
	var
		col : smallint;
		i : word;
		j : word = 0;
		carry : word = 0;
	begin
		if (cmpstrnum(n1, n2)) then
			substrnum := createstrnum('0')
		else
		begin
			for i := 1 to getmax(getstrnumlen(n1), getstrnumlen(n2)) do
			begin
				col := getstrnumarrayvalue_int(n1, i) - getstrnumarrayvalue_int(n2, i) - carry;

				if (col < 0) then
				begin
					carry := 1;
					col := 10 + col;
				end
				else
					carry := 0;

				j := j + 1;

				substrnum.m_chNum[j] := chr(col+48);
			end;

			setstrnumlen(@substrnum, j);

			for i := 1 to j do (*Strip meaningless zeroes*)
			begin
				if (getstrnumunit_chr(substrnum, i) <> '0') then
					break;
					
				j := j - 1;
			end;

			setstrnumlen(@substrnum, j);
		end;
	end;

	function multstrnum(n1, n2 : strnum_t) : strnum_t; (*Temporary hack*)
	var
		one, count : strnum_t;
	begin
		one := createstrnum('1');	
		count := one;
		multstrnum := n1;

		while cmpstrnum(count, n2) <> true do
		begin
			multstrnum := addstrnum(multstrnum, n1);
			count := addstrnum(count, one);
		end;
	end;
	
	function shoutstrnum(n : strnum_t) : strnum_t;
	var
		i, one : strnum_t;
	begin
		one := createstrnum('1');
		shoutstrnum := one;
		i := one;
		
		while cmpstrnum(i, n) <> true do
		begin
			shoutstrnum := multstrnum(shoutstrnum, substrnum(n, substrnum(i, one)));
			i := addstrnum(i, one);
		end;
	end;

	procedure writestrnum(bn : strnum_t);
	var
		i : word;
	begin
		for i := 1 to getstrnumlen(bn) do
			write(bn.m_chNum[getstrnumlen(bn)-i+1]);
	end;

	procedure writestrnumln(bn : strnum_t);
	begin
		writestrnum(bn);
		write(CRLF);
	end;
end.