program problem36;

uses strnum, strutils;

var
	i : longword;
	sum : qword = 0;
	
begin
	for i := 1 to 999999 do
	begin
		if (strnumispalindrome(i)) and (strnumispalindrome(IntToBin(i, 255))) then
		begin
			writeln(i, ' is palindromic in base-2 and base-10!');

			sum := sum + i;
		end;
	end;

	writeln('Sum: ', sum);
	readln();
end.