program problem97;

var
	i : longword;
	product : qword;

begin
	product := 1;

	for i := 1 to 7830457 do
	begin
		product := (product * 2) mod 10000000000;
	end;
	
	product := 28433 * product + 1;

	writeln('Product: ', product);
	readln();
end.