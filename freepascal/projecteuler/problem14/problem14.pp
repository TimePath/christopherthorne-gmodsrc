program problem14;

var
	i, count, maxval, maxkey : longword;

function getseqcount(n : longword) : word;
begin
	getseqcount := 1;

	while n <> 1 do
	begin
		if ((n mod 2) = 0) then
			n := n div 2
		else
			n := (n*3)+1;
			
		getseqcount := getseqcount + 1;
	end;
end;

begin
	maxval := 0;
	maxkey := 0;

	for i := 1 to 1000000 do
	begin
		count := getseqcount(i);

		if (count > maxval) then
		begin
			maxval := count;
			maxkey := i;
		end;
	end;

	writeln('Max: ', maxkey);
	readln();
end.