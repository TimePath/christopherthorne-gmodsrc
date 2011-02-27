program palindrome;

uses sysutils;

var
	i, i2 : integer;
	product, max : cardinal;

function ispalindrome(num : cardinal) : boolean;
var
	istr, left, right : string;
	istrlen, istrchunk, i : integer;
begin
	istr := IntToStr(num);
	istrlen := length(istr);

	if (istrlen < 2) then
	begin
		ispalindrome := false;
	end
	else
	begin
		ispalindrome := true;

		istrchunk := istrlen div 2;
		
		left := LeftStr(istr, istrchunk);
		right := RightStr(istr, istrchunk);
					
		for i := 1 to istrchunk do
		begin
			if (left[i] <> right[istrchunk-i+1]) then
			begin
				ispalindrome := false;
				
				break;
			end;
		end;
	end;
end;

begin
	for i := 1 to 999 do
	begin
		for i2 := 1 to 999 do
		begin
			product := i*i2;

			if (product > max) and (ispalindrome(product)) then
			begin
				max := product;
				
				writeln('New max: ', max);
			end;
		end;
	end;
	
	writeln('Max: ', max);
	readln();
end.