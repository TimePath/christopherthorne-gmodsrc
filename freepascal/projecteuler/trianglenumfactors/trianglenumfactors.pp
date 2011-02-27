program trianglenumfactors;

var
	i, j, k : longword;
	val, valrt : longword;
	fcount : word;

begin
	i := 2;

	while true do
	begin
		val := 0;
		fcount := 0;

		for j := 1 to i do
			val := val + j;
		
		valrt := trunc(sqrt(val)); (*The amount of factors below the square root is half of the total factors*)

		for k := 1 to valrt do
		begin
			if (val mod k) = 0 then
				fcount := fcount + 2;
		end;

		if (fcount > 500) then
		begin
			writeln('The number ', val, ' has over 500 factors');
			
			break;
		end;
		
		i := i + 1;
	end;
	
	writeln('Finished');
	readln();
end.