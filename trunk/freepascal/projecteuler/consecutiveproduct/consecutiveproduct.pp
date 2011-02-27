program consecutiveproduct;

var
	numfile : text;
	strarr : array[1..1000] of char;
	numarr : array[1..1000] of byte;
	
	i, product, max : word;
	i2 : byte;

begin
	assign(numfile, 'number.txt');
	reset(numfile);
	
	for i := 1 to 1000 do
	begin
		read(numfile, strarr[i]);
		
		numarr[i] := ord(strarr[i])-48;
	end;

	close(numfile);
	
	for i := 1 to 996 do
	begin
		product := 1;

		for i2 := 0 to 4 do
			product := product * numarr[i+i2];

		if (product > max) then
			max := product;
	end;
	
	writeln('Max: ', max);
	readln();
end.