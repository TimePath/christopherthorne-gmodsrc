program fibonacci;

const
	CRLF = #13 + #10;

var
	sum, i, p1, p2 : cardinal;
	
begin
	i := 1;
	p1 := i;
	p2 := 0;

	while i < 4000000 do
	begin
		write(p1, ',', p2);
		
		i := p1 + p2;
		
		write(' -> ', i, CRLF);

		p2 := p1;
		p1 := i;

		if (i mod 2) = 0 then
			sum := sum + i;
	end;
	
	writeln(CRLF, 'Total: ', sum);
	readln();
end.