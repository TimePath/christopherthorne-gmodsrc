program problem48;

(*uses strnum;

var
	i, j : word;
	res, sum : strnum_t;

begin
	sum := 0;

	for i := 1 to 1000 do
	begin
		res := i;

		for j := 1 to i-1 do
			res := res * i;

		sum := sum + res;
		
		writeln(i);
	end;
	
	write('Sum: ');
	writestrnumln(sum);

	readln();
end.*)

(*MUCH faster (and proper) solution taken from the thread, only keeps digits we're interested in*)

var
	i, j : word;
	f, sum : qword;
begin
	sum := 0;

	for i := 1 to 1000 do
	begin
		f := i;

		for j := 1 to i-1 do
			f := (f*i) mod 10000000000;
		
		sum := sum + f;
	end;

	writeln(sum);
	readln();
end.