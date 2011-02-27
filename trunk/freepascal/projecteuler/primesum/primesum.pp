program primesum;

const
	upperbound : longword = 2000000;

var
	i : longword;
	sum : qword;

function isprime(num : int64) : boolean; (*primes.pp*)
var
	i, max : int64;
begin	
	if (num = 1) or ((num > 2) and (num mod 2 = 0)) then
		isprime := false
	else
	begin
		isprime := true;

		i := 2;
		max := trunc(sqrt(num));

		while i <= max do (*int64 loop*)
		begin
			if (num mod i) = 0 then
			begin
				isprime := false;

				break;
			end;
			
			i := i + 1;
		end;
	end;
end;

begin
	for i := 2 to upperbound-1 do
	begin
		if (isprime(i)) then
			sum := sum + i;
	end;
	
	writeln('Sum: ', sum);
	readln();
end.