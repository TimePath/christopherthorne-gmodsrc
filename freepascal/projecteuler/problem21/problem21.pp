program problem21;

var
	i, sum : word;

function add_divisors(num : word) : word;
var
	i : word;
begin
	add_divisors := 0;

	for i := 1 to num div 2 do
	begin
		if (num mod i) = 0 then
			add_divisors := add_divisors + i;
	end;
end;

function is_amicable(num : word) : boolean;
var
	a : word;
	b : word;
begin
	a := add_divisors(num);
	
	if a = num then (*Perfect numbers are not amicable*)
		exit(false);

	b := add_divisors(a);
	
	if b = num then
		exit(true);
		
	exit(false);
end;

begin
	sum := 0;

	for i := 1 to 9999 do
	begin
		if is_amicable(i) then
			sum := sum + i;
	end;

	writeln('Amicable numbers sum: ', sum);
	readln();
end.