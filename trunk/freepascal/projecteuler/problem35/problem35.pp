program problem35;

uses strnum;

var
	test : word = 0;
	i : word;

function is_circular_prime(n : strnum_t) : boolean;
var
	i : word;
begin
	for i := 1 to getstrnumlen(n) do
	begin
		(*writestrnumln(shiftstrnum(n, i));*)
	end;

	exit(true);
end;

begin
	readln();
end.