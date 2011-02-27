program primes;

const
	number_to_factorise : int64 = 600851475143;

var
	i : int64 = 1;
	factor_mult : int64 = 1;

function isprime(num : int64) : boolean;
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

function isfactor(factor : int64; num : int64) : boolean;
begin
	isfactor := false;
	
	if (num mod factor) = 0 then
		isfactor := true;
end;

function isprimefactor(factor : int64; num : int64) : boolean;
begin
	isprimefactor := false;

	if (isfactor(factor, num)) and (isprime(i)) then
		isprimefactor := true;
end;

begin
	writeln('Calculating primes...');
	
	while i <= number_to_factorise do (*int64 loop*)
	begin
		if (isprimefactor(i, number_to_factorise)) then
		begin
			writeln('Prime: ', i);

			factor_mult := factor_mult * i;
			
			if (factor_mult = number_to_factorise) then (*This isn't a proper implementation but works for this challenge*)
				break;
		end;
			
		i := i + 1;
	end;

	writeln('Finished!');
	readln();
end.