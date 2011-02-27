program problem37;

uses strnum, strutils, sysutils;

var
	i : byte = 0;
	j : qword = 10;
	sum : qword = 0;

function is_prime(num : qword) : boolean;
var
	i, max : qword;
begin	
	if (num = 1) or ((num > 2) and (num mod 2 = 0)) then
		exit(false);

	i := 2;
	max := trunc(sqrt(num));

	while i <= max do
	begin
		if num mod i = 0 then
			exit(false);
			
		i := i + 1;
	end;

	exit(true);
end;

function is_truncatable_prime(num : qword) : boolean;
var
	i, len : word;
	str : string;
begin
	str := IntToStr(num);
	len := length(str);

	for i := 1 to len do
	begin
		if not is_prime(strnumtoqword(RightStr(str, len-i+1))) then
			exit(false);
	end;

	for i := 1 to len do
	begin
		if not is_prime(strnumtoqword(LeftStr(str, len-i+1))) then
			exit(false);
	end;

	exit(true);
end;

begin
	while i < 11 do
	begin
		if is_truncatable_prime(j) then
		begin
			i := i + 1;
			sum := sum + j;

			writeln(j, ' is a truncatable prime!');
		end;
		
		j := j + 1;
	end;
	
	writeln('Sum: ', sum);
	readln();
end.