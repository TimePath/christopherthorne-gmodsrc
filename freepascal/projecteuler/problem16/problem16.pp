program problem16;

uses math, strnum;

var
	num, prev : strnum_t;
	i, sum : word;

begin
	num := createstrnum('1');
	sum := 0;
	
	for i := 1 to 1000 do
	begin
		prev := num;
		num := addstrnum(prev, prev);
	end;
	
	write('Result: ');
	writestrnumln(num);
	
	for i := 1 to getstrnumlen(num) do
		sum := sum + getstrnumunit_int(num, i);

	writeln('Sum: ', sum);
	readln();
end.