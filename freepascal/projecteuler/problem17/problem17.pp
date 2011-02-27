program problem17;

uses sysutils;

var
	i : word;
	sum : word = 0;

function smallnum2str(num : word) : string;
begin
	case num of
		1  : smallnum2str := 'one';
		2  : smallnum2str := 'two';
		3  : smallnum2str := 'three';
		4  : smallnum2str := 'four';
		5  : smallnum2str := 'five';
		6  : smallnum2str := 'six';
		7  : smallnum2str := 'seven';
		8  : smallnum2str := 'eight';
		9  : smallnum2str := 'nine';
		10 : smallnum2str := 'ten';
		11 : smallnum2str := 'eleven';
		12 : smallnum2str := 'twelve';
		13 : smallnum2str := 'thirteen';
		14 : smallnum2str := 'fourteen';
		15 : smallnum2str := 'fifteen';
		16 : smallnum2str := 'sixteen';
		17 : smallnum2str := 'seventeen';
		18 : smallnum2str := 'eighteen';
		19 : smallnum2str := 'nineteen';
		20 : smallnum2str := 'twenty';
		30 : smallnum2str := 'thirty';
		40 : smallnum2str := 'forty';
		50 : smallnum2str := 'fifty';
		60 : smallnum2str := 'sixty';
		70 : smallnum2str := 'seventy';
		80 : smallnum2str := 'eighty';
		90 : smallnum2str := 'ninety';
	else
		smallnum2str := '';
	end;
end;

function num2str(num : word) : string;
var
	str : string;
	len, tmp : word;
	use_and : boolean = false;
begin
	num2str := '';
	str := IntToStr(num);
	len := length(str);

	if len >= 4 then
	begin
		tmp := ord(str[len-3])-48;
		
		if tmp <> 0 then
			num2str := num2str + smallnum2str(tmp) + 'thousand';
	end;
	
	if len >= 3 then
	begin
		if str[len] <> '0' then
			use_and := true;

		tmp := ord(str[len-2])-48;
		
		if tmp <> 0 then
			num2str := num2str + smallnum2str(tmp) + 'hundred';
	end;
	
	if len >= 2 then
	begin
		tmp := ord(str[len-1])-48;

		if tmp <> 0 then
		begin
			if num2str <> '' then
			begin
				num2str := num2str + 'and';
				
				use_and := false;
			end;

			tmp := tmp * 10;

			if tmp = 10 then
			begin
				num2str := num2str + smallnum2str(tmp + (ord(str[len])-48));
				
				exit();
			end
			else
				num2str := num2str + smallnum2str(tmp);
		end;
	end;
	
	if len >= 1 then
	begin
		if use_and then
			num2str := num2str + 'and';

		tmp := (ord(str[len])-48);
		
		if tmp <> 0 then
			num2str := num2str + smallnum2str(tmp);
	end;
end;

begin
	for i := 1 to 1000 do
	begin
		writeln(num2str(i));

		sum := sum + length(num2str(i));
	end;

	writeln('Sum: ', sum);
	readln();
end.