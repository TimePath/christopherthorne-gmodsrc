program problem25;

uses strnum;

var
	i, p1, p2 : strnum_t;
	j : word = 1;
	
begin
	i := createstrnum('1');
	p1 := i;
	p2 := createstrnum('0');

	while true do
	begin
		i := addstrnum(p1, p2);
		j := j + 1;

		p2 := p1;
		p1 := i;

		if (getstrnumlen(i) >= 1000) then
			break;
	end;
	
	writeln('Answer: ', j);

	readln();
end.