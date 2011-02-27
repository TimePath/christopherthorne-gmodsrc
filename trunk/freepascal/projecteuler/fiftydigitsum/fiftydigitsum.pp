program fiftydigitsum;

uses math;

var
	tmp : char;
	i, j : word;
	mult, sum : real;

	numfile : text;
	numarr : array[1..100] of real;

begin
	sum := 0;

	assign(numfile, 'numbers.txt');
	reset(numfile);
	
	for i := 1 to 100 do
	begin
		numarr[i] := 0;

		for j := 1 to 50 do
		begin
			read(numfile, tmp);

			if (j = 1) then
				mult := 1
			else
				mult := power(10, j-1);

			numarr[i] := numarr[i] + ((ord(tmp)-48)/mult);
		end;

		sum := sum + numarr[i];
	end;

	close(numfile);

	writeln('Sum: ', sum);
	readln();
end.