program prime10001;

const
	primeindex : integer = 10001;
	test : qword = 5147483647;
	
var
	i, primecount : longint;

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
	i := 2;

	while true do
	begin
		if (isprime(i)) then
		begin
			primecount := primecount + 1;
			
			if (primecount = primeindex) then
			begin
				writeln('Found specified prime: ', i);

				break;
			end;
		end;
		
		i := i + 1;
	end;

	readln();
end.