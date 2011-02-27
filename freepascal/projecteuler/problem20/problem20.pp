program problem20;

uses strnum;

var
	shout : strnum_t;
	i : word;
	sum : word = 0;

begin		
	shout := shoutstrnum(createstrnum('100'));
	
	write('Answer: ');
	writestrnumln(shout);
	
	for i := 1 to getstrnumlen(shout) do
		sum := sum + getstrnumunit_int(shout, i);
	
	writeln('Sum: ', sum);
	readln();
end.