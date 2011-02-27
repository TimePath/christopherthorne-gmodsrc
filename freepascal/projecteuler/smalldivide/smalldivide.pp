program smalldivide;

const
	lowerdiv : int64 = 1;
	upperdiv : int64 = 20;

var
	monitor : boolean;
	i : int64;
	i2 : integer;

begin
	i := upperdiv;

	while true do
	begin
		monitor := true;

		for i2 := lowerdiv to upperdiv do
		begin
			if (i mod i2 <> 0) then
			begin
				monitor := false;
				
				break;
			end;
		end;
		
		if (monitor) then
		begin
			writeln('Num: ', i);
			
			break;
		end;
		
		i := i + 1;
	end;

	readln();
end.