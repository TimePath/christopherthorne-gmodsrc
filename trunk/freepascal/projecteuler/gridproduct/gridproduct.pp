program gridproduct;

var
	i : word;

	c1, c2, discard : char;
	gridfile : text;
	numarr : array[1..400] of byte;
	base : byte;
	
	max : longword;

procedure checkmax(val : longword);
begin
	if (val > max) then
		max := val;
end;

begin
	assign(gridfile, 'grid.txt');
	reset(gridfile);
	
	for i := 1 to 400 do
	begin
		read(gridfile, c1);
		read(gridfile, c2);
		read(gridfile, discard); (*Space*)
		
		numarr[i] := (ord(c1)-48)*10 + (ord(c2)-48);
	end;
	
	for i := 1 to 400 do
	begin
		base := numarr[i];

		if (i+3 <= 400) then
			checkmax(base+numarr[i+1]+numarr[i+2]+numarr[i+3]); (*Right*)
		
		if (i+60 <= 400) then
			checkmax(base*numarr[i+20]*numarr[i+40]*numarr[i+60]); (*Down*)
		
		if (i+57 <= 400) then
			checkmax(base*numarr[i+19]*numarr[i+38]*numarr[i+57]); (*Left down diag*)

		if (i+63 <= 400) then
			checkmax(base*numarr[i+21]*numarr[i+42]*numarr[i+63]); (*Right down diag*)
		
		if (i-3 > 0) then
			checkmax(base*numarr[i-1]*numarr[i-2]*numarr[i-3]); (*Left*)
		
		if (i-60 > 0) then
			checkmax(base*numarr[i-20]*numarr[i-40]*numarr[i-60]); (*Up*)
		
		if (i-63 > 0) then
			checkmax(base*numarr[i-21]*numarr[i-42]*numarr[i-63]); (*Left up diag*)
		
		if (i-57 > 0) then
			checkmax(base*numarr[i-19]*numarr[i-38]*numarr[i-57]); (*Right up diag*)
	end;
	
	close(gridfile);
	
	writeln('Max: ', max);
	readln();
end.