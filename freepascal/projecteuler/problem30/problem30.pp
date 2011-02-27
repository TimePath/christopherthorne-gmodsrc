program problem30;

uses strnum;

var
	i, sum, total : strnum_t;
	j : word;

begin
	i := 2;
	total := 0;

	while true do
	begin
		sum := 0;

		for j := 1 to getstrnumlen(i) do
			sum := sum + expstrnum(getstrnumunit(i, j), 5);

		if sum = i then
		begin
			total := total + i;
			writestrnumln(i);
		end;

		i := i + 1;
	end;
	
	write('Total: ');
	writestrnumln(total);

	readln();
end.