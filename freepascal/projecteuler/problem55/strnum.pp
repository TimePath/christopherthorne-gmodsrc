unit strnum;

interface
	type
		strnum_t = record
			m_chNum : array[1..4096] of byte;
			m_nLength : word;
		end;
		
		pstrnum_t = ^strnum_t;
		
	const
		CRLF = #13 + #10;

	function strnumtostring(n : strnum_t) : string;
	function strnumispalindrome(n : strnum_t) : boolean;
	
	function cmpstrnum(n1, n2 : strnum_t) : boolean;
	function reversestrnum(n : strnum_t) : strnum_t;

	function getstrnumlen(n : strnum_t) : word;
	procedure setstrnumlen(pn : pstrnum_t; len : word);

	function getstrnumarrayvalue(n : strnum_t; pos : word) : byte;
	function getstrnumunit(n : strnum_t; pos : word) : byte;

	procedure setstrnumunit(pn : pstrnum_t; val : byte; pos : word);

	function createstrnum(str : string) : strnum_t;

	function addstrnum(n1, n2 : strnum_t) : strnum_t;
	function substrnum(n1, n2 : strnum_t) : strnum_t;
	function multstrnum(n1, n2 : strnum_t) : strnum_t;
	function shoutstrnum(n : strnum_t) : strnum_t;

	procedure writestrnum(bn : strnum_t);
	procedure writestrnumln(bn : strnum_t);
	
	Operator := (a : string) r : strnum_t;
	Operator := (a : word) r : strnum_t;
	Operator := (a : longword) r : strnum_t;
	Operator := (a : qword) r : strnum_t;

	Operator + (a, b: strnum_t) r : strnum_t;
	Operator - (a, b: strnum_t) r : strnum_t;
	Operator * (a, b: strnum_t) r : strnum_t;
	
	Operator = (a, b: strnum_t) r : boolean;
	
implementation
	uses sysutils;

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

	function strnumtostring(n : strnum_t) : string;
	var
		i : word;
	begin
		strnumtostring[0] := chr(n.m_nLength);

		for i := 1 to getstrnumlen(n) do
			strnumtostring[i] := chr(getstrnumunit(n, i)+48);		
	end;
	
	function strnumispalindrome(n : strnum_t) : boolean;
	var
		str, left, right : string;
		i, chunk : word;
	begin
		str := strnumtostring(n);
		chunk := length(str) div 2;	
		left := LeftStr(str, chunk);
		right := RightStr(str, chunk);

		for i := 1 to chunk do
		begin
			if (left[i] <> right[chunk-i+1]) then
				exit(false);
		end;
			
		exit(true);
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
				if (getstrnumunit(n1, i) <> getstrnumunit(n2, i)) then
				begin
					cmpstrnum := false;
					
					break;
				end;
			end;
		end;
	end;
	
	function reversestrnum(n : strnum_t) : strnum_t;
	var
		i, len : word;
		writezero : boolean = false;
		val : byte;
	begin
		len := getstrnumlen(n);

		reversestrnum.m_nLength := 0;

		for i := 0 to len-1 do
		begin
			val := n.m_chNum[i+1];
			
			if (not writezero) then
			begin
				if (val <> 0) then
					writezero := true
				else
					continue;
			end;

			reversestrnum.m_chNum[len-i] := val;
			reversestrnum.m_nLength := reversestrnum.m_nLength + 1;
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

	function getstrnumarrayvalue(n : strnum_t; pos : word) : byte;
	begin
		if ((pos > 0) and (pos <= getstrnumlen(n))) then
			getstrnumarrayvalue := n.m_chNum[pos]
		else
			getstrnumarrayvalue := 0;
	end;

	function getstrnumunit(n : strnum_t; pos : word) : byte;
	begin
		getstrnumunit := getstrnumarrayvalue(n, getstrnumlen(n)-pos+1);
	end;

	procedure setstrnumunit(pn : pstrnum_t; val : byte; pos : word);
	var
		len, key : word;
	begin
		len := getstrnumlen(pn^);
		key := len-pos+1;

		if ((key > 0) and (key <= len)) then
			pn^.m_chNum[key] := val;
	end;
	
	function createstrnum(str : string) : strnum_t;
	var
		i : word;
	begin
		setstrnumlen(@createstrnum, length(str));
		
		for i := 1 to getstrnumlen(createstrnum) do
			setstrnumunit(@createstrnum, byte(str[i])-48, i);
	end;

	function addstrnum(n1, n2 : strnum_t) : strnum_t;
	var
		i, sum, col : word;
		j : word = 0;
		carry : word = 0;
	begin
		for i := 1 to getmax(getstrnumlen(n1), getstrnumlen(n2))+1 do
		begin
			sum := word(getstrnumarrayvalue(n1, i)) + word(getstrnumarrayvalue(n2, i)) + carry;
			col := sum-floor10(sum);
			carry := (sum-col) div 10;

			j := j + 1;

			addstrnum.m_chNum[j] := byte(col);
		end;

		setstrnumlen(@addstrnum, j);
		
		for i := 1 to j do (*Strip meaningless zeroes*)
		begin
			if (getstrnumunit(addstrnum, i) <> 0) then
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
		if (n1 = n2) then
			substrnum := 0
		else
		begin
			for i := 1 to getmax(getstrnumlen(n1), getstrnumlen(n2)) do
			begin
				col := word(getstrnumarrayvalue(n1, i)) - word(getstrnumarrayvalue(n2, i)) - carry;

				if (col < 0) then
				begin
					carry := 1;
					col := 10 + col;
				end
				else
					carry := 0;

				j := j + 1;

				substrnum.m_chNum[j] := byte(col);
			end;

			setstrnumlen(@substrnum, j);

			for i := 1 to j do (*Strip meaningless zeroes*)
			begin
				if (getstrnumunit(substrnum, i) <> 0) then
					break;
					
				j := j - 1;
			end;

			setstrnumlen(@substrnum, j);
		end;
	end;

	(*function multstrnum(n1, n2 : strnum_t) : strnum_t;
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
	end;*)
	
	function multstrnum(n1, n2 : strnum_t) : strnum_t;
	var
		i, j, k, min, max, product, col : word;
		carry : word = 0;
		minv, maxv, row : strnum_t;
	begin
		multstrnum := '0';

		min := getmin(getstrnumlen(n1), getstrnumlen(n2));
		max := getmax(getstrnumlen(n1), getstrnumlen(n2));

		if min = getstrnumlen(n1) then
		begin
			minv := n1;
			maxv := n2;
		end
		else
		begin
			minv := n2;
			maxv := n1;
		end;
		
		for i := 1 to min do
		begin
			k := i-1;
			row := 0;

			for j := 1 to i-1 do
				row.m_chNum[j] := 0;

			for j := 1 to max+1 do
			begin
				product := (word(getstrnumarrayvalue(minv, i)) * word(getstrnumarrayvalue(maxv, j))) + carry;
				col := product-floor10(product);
				carry := (product-col) div 10;

				row.m_chNum[i+j-1] := byte(col);
				row.m_chNum[i+j] := byte(carry);

				k := k + 1;
			end;

			setstrnumlen(@row, k);
			
			multstrnum := multstrnum + row;
		end;
	end;

	function shoutstrnum(n : strnum_t) : strnum_t;
	var
		i : strnum_t;
	begin
		shoutstrnum := 1;
		i := 1;
		
		while i <> n do
		begin
			shoutstrnum := shoutstrnum * n - (i - 1);

			i := i + 1;
		end;
	end;

	procedure writestrnum(bn : strnum_t);
	begin
		write(strnumtostring(bn));
	end;

	procedure writestrnumln(bn : strnum_t);
	begin
		writestrnum(bn);
		write(CRLF);
	end;
	
	Operator := (a : string) r : strnum_t;
	begin
		r := createstrnum(a);
	end;

	Operator := (a : word) r : strnum_t;
	begin
		r := createstrnum(IntToStr(a));
	end;

	Operator := (a : longword) r : strnum_t;
	begin
		r := createstrnum(IntToStr(a));
	end;
	
	Operator := (a : qword) r : strnum_t;
	begin
		r := createstrnum(IntToStr(a));
	end;

	Operator * (a, b: strnum_t) r : strnum_t;
	begin
		r := multstrnum(a, b);
	end;
	
	Operator + (a, b: strnum_t) r : strnum_t;
	begin
		r := addstrnum(a, b);
	end;

	Operator - (a, b: strnum_t) r : strnum_t;
	begin
		r := substrnum(a, b);
	end;
	
	Operator = (a, b: strnum_t) r : boolean;
	begin
		r := cmpstrnum(a, b);
	end;
end.