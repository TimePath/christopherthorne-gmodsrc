program fiftydigitsum;

uses strnum;

var
	discard, tmp : char;
	numfile : text;
	numarr : array[1..100] of strnum_t;
	numsum : strnum_t;
	i, j : word;

begin
	numsum := createstrnum('0');

	assign(numfile, 'numfile.txt');
	reset(numfile);
	
	for i := 1 to 100 do
	begin
		setstrnumlen(@numarr[i], 50);

		for j := 1 to 50 do
		begin
			read(numfile, tmp);
			setstrnumunit_chr(@numarr[i], tmp, j);
		end;
				
		numsum := addstrnum(numsum, numarr[i]);

		read(numfile, discard); (*CR*)
		read(numfile, discard); (*LF*)
	end;

	close(numfile);

	write('Sum: ');
	writestrnumln(numsum);
	readln();
end.