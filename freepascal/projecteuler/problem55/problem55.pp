program problem55;

uses sysutils, strutils, strnum;

var
	i : word;
	count : word = 0;

function islychrel(num : word) : boolean;
type
	pword = ^word;
var
	i : word;
	num1, num2, sum : strnum_t;
begin
	sum := num;

	for i := 1 to 49 do
	begin
		num1 := sum;
		num2 := reversestrnum(num1);
		
		sum := num1 + num2;

		if strnumispalindrome(sum) then
			exit(false);
	end;
	
	exit(true);
end;

begin
	for i := 0 to 9999 do
	begin
		if islychrel(i) then
			count := count + 1;
	end;
	
	writeln('Lychrel count: ', count);
	readln();
end.