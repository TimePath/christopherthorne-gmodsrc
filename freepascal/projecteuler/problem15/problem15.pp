program problem15;

var
	i : word;

function shout(n : qword) : qword;
begin
	shout := n;

	while n > 1 do
	begin
		n := n - 1;

		shout := shout*n;
	end;
end;

function calcpaths(n : word) : qword;
begin
	calcpaths := shout(n*2) div (shout(n)*shout(n));
end;

begin
	for i := 1 to 20 do
		writeln(calcpaths(i));

	readln();
end.