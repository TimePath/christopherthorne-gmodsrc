program hangman;

uses sysutils;

const
	CRLF = #13 + #10;
	GUESS_WORD : string = 'SeCreT woRd';

var
	i, len, len2, guesscount : byte;
	guess : string;
	guessmap : array[1..255] of boolean;
	mapcomplete : boolean;
	failed : boolean = false;

begin
	len := length(GUESS_WORD);

	guesscount := 0;

	for i := 1 to len do
	begin
		if GUESS_WORD[i] = ' ' then
			guessmap[i] := true
		else
			guessmap[i] := false;
	end;

	while true do
	begin
		write(CRLF);

		for i := 1 to len do
		begin
			if guessmap[i] then
				write(GUESS_WORD[i])
			else
				write('_');
			
			if i <> len then
				write(' ')
			else
				writeln(CRLF);
		end;
		
		write('Enter a letter or a full guess: ');
		readln(guess);
	
		len2 := length(guess);

		if len2 > 0 then
		begin
			guesscount := guesscount + 1;

			if len2 > 1 then
			begin
				if LowerCase(guess) = LowerCase(GUESS_WORD) then
					break;
			end
			else
			begin
				mapcomplete := true;

				for i := 1 to len do
				begin
					if LowerCase(guess[1]) = LowerCase(GUESS_WORD[i]) then
						guessmap[i] := true;
						
					if (mapcomplete = true) and (guessmap[i] = false) then (*Saves 2 loops*)
						mapcomplete := false;
				end;
				
				if mapcomplete then
					break;
			end;
		end
	end;

	write(CRLF);

	if failed then
		writeln('Game over, thanks for playing!')
	else
	begin
		write('You win, the string was "', GUESS_WORD, '" and it only took you ', guesscount);

		if guesscount > 1 then
			writeln(' guesses!')
		else
			writeln(' guess!');
	end;

	write(CRLF, 'Press ENTER to exit...');
	readln();
end.