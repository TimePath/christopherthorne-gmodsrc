program problem34;

uses strnum;

var
	i, digsum : strnum_t;
	j : word;

begin
	i := 3;

	while true do
	begin
		digsum := 0;

		for j := 1 to getstrnumlen(i) do
			digsum := digsum + shoutstrnum(getstrnumunit(i, j));

		if digsum = i then
			writestrnumln(i);

		i := i + 1;
	end;

	readln();
end.