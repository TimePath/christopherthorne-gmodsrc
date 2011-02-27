program problem9;

var
	i, j, k : word;

begin
	for i := 1 to 1000 do
	begin
		for j := i+1 to 1000 do
		begin
			for k := j+1 to 1000 do
			begin
				if ((sqr(i) + sqr(j) = sqr(k)) and (i + j + k = 1000)) then
				begin
					writeln(i, ', ', j, ', ', k, '->', i + j + k);
					writeln('Product: ', i*j*k);

					break;
				end;
			end;
		end;
	end;
	
	readln();
end.