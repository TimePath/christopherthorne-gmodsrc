program shuttle_sort;

type
	sortarray = array[1..8] of longint;
	psortarray = ^sortarray;

const
	CRLF = #13 + #10;

var
	numbers : sortarray;

procedure writenumbers(numbers : sortarray);
var
	i : byte;
	upper : word;
begin
	upper := high(numbers);

	for i := 1 to upper do
	begin
		write(numbers[i]);
		
		if i = upper then
			write(CRLF)
		else
			write(', ');
	end;
end;

function sort_compare(numbers : psortarray; n1, n2 : word) : boolean;
var
	tmp : longint;
begin
	if numbers^[n1] > numbers^[n2] then
	begin
		tmp := numbers^[n2];

		numbers^[n2] := numbers^[n1];
		numbers^[n1] := tmp;
		
		exit(true);
	end;
	
	exit(false);
end;

procedure sort_internal(numbers : psortarray; size : word);
var
	i, j : byte;
	comparisons : word = 0;
	swaps : word = 0;
begin
	for i := 1 to size-1 do
	begin
		comparisons := comparisons + 1;

		if sort_compare(numbers, i, i+1) then
		begin
			swaps := swaps + 1;
			
			j := i;
			
			while j > 1 do
			begin
				comparisons := comparisons + 1;

				if sort_compare(numbers, j-1, j) then
					swaps := swaps + 1
				else
					break;
					
				j := j - 1;
			end;
		end;
	end;
	
	writeln('Comparisons: ', comparisons);
	writeln('Swaps: ', swaps);
end;

procedure sort(numbers : psortarray);
begin
	sort_internal(numbers, high(numbers^));
end;

begin
	numbers[1] := 95;
	numbers[2] := 74;
	numbers[3] := 61;
	numbers[4] := 87;
	numbers[5] := 71;
	numbers[6] := 82;
	numbers[7] := 53;
	numbers[8] := 57;

	writenumbers(numbers);
	sort(@numbers);
	writenumbers(numbers);

	readln();
end.