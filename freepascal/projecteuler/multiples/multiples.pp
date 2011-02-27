program multiples;

var
	sum, i : longint;

begin
	sum := 0;

	for i := 3 to 999 do
	begin
		if ((i mod 3) = 0) or ((i mod 5) = 0) then
			sum := sum + i;
	end;

	writeln('Multiples total: ', sum);
	readln();
end.