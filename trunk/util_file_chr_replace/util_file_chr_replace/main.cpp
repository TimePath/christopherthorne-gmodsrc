#include <stdio.h>
#include <conio.h>
#include <Windows.h>

char *chr_replace(char *str, char o, char n)
{
	for (int i=0;i<strlen(str);i++)
	{
		if (str[i] == o)
			str[i] = n;
	}

	return str;
}

char *name_from_path(char *path)
{
	int path_len = strlen(path);

	if (path_len <= 0)
		return NULL;

	static char name[256];

	int i = 0;

	for (int j=0;j<path_len;j++)
	{
		char c = path[path_len-j-1];

		if (c == '\\' || c == '/')
		{
			i = path_len - j;

			break;
		}
	}

	int k = 0;

	for (i;i<path_len;i++)
		name[k++] = path[i];

	name[k] = '\0';

	return name;
}

int main()
{
	char old_chr;
	char new_chr;

	printf("Input old character: ");
	old_chr = _getch();
	printf("'%c'\n", old_chr);

	printf("Input new character: ");
	new_chr = _getch();
	printf("'%c'\n", new_chr);

	char self_path[256];

	GetModuleFileName(NULL, self_path, 256);

	char *self_name = name_from_path(self_path);

	WIN32_FIND_DATA data;

	HANDLE hFindFile = FindFirstFile("./*", &data);

	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(data.cFileName, self_name) != 0 && strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, ".."))
			{
				char old_name[256];
				char new_name[256];
				
				strcpy(old_name, data.cFileName);
				strcpy(new_name, chr_replace(data.cFileName, old_chr, new_chr));

				if (strcmp(old_name, new_name) != 0)
				{
					printf("Renamed %s to %s\n", old_name, new_name);

					rename(old_name, new_name);
				}
			}
		}
		while (FindNextFile(hFindFile, &data));
	}

	_getch();

	return 0;
}