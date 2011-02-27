program bubble_sort;

type
	sortarray = array[1..5] of longint;
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

procedure sort_internal(numbers : psortarray; size : word; comparisons, swaps : pword);
var
	i : byte;
	tmp : longint;
	curswaps : word = 0;
begin
	if size = 1 then
		exit();

	for i := 1 to size-1 do
	begin
		comparisons^ := comparisons^ + 1;

		if numbers^[i] > numbers^[i+1] then
		begin
			tmp := numbers^[i+1];

			numbers^[i+1] := numbers^[i];
			numbers^[i] := tmp;

			curswaps := curswaps + 1;
		end;
	end;
	
	swaps^ := swaps^ + curswaps;

	if curswaps > 0 then
		sort_internal(numbers, size-1, comparisons, swaps);
end;

procedure sort(numbers : psortarray);
var
	comparisons : word = 0;
	swaps : word = 0;
begin
	sort_internal(numbers, high(numbers^), @comparisons, @swaps);
	
	writeln('Comparisons: ', comparisons);
	writeln('Swaps: ', swaps);
end;

begin
	numbers[1] := 5;
	numbers[2] := 4;
	numbers[3] := 3;
	numbers[4] := 2;
	numbers[5] := 1;

	writenumbers(numbers);
	sort(@numbers);
	writenumbers(numbers);

	readln();
end.