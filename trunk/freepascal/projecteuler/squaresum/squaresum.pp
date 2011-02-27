program squaresum;

const
	lowerbound : integer = 1;
	upperbound : integer = 100;

var
	i : integer;
	rangesumv, squaresumv : int64;

begin
	for i := lowerbound to upperbound do
	begin
		rangesumv := rangesumv + i;
		squaresumv := squaresumv + sqr(i);
	end;

	writeln('Difference: ', sqr(rangesumv)-squaresumv);
	readln();
end.