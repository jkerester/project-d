//James Kerester and Molly Croke
void printChar(char);
void printString(char*);
void readString(char*);
void handleInterrupt21(int, int, int, int);
void readSector(char*, int);
void readFile(char*, char*,  int*);
void executeProgram(char*);
void terminate();
void writeSector(char*, int);
void deleteFile(char*);
main()
{
	makeInterrupt21();
	interrupt(0x21, 4, "shell", 0, 0);

	while(1);   /*hang up*/ 
}
//debugging function
void printChar(char c)
{
	interrupt(0x10, 0xe*256 + c, 0, 0, 0);
}

void printString(char* chars)
{
	int i=0;
	while(chars[i] != '\0')
	{
		interrupt(0x10, 0xe*256 + chars[i], 0, 0, 0);
		i++;
	} 
}

void readString(char* line)
{
	char keyPress = interrupt(0x16, 0, 0, 0, 0);
	int i=0;
	interrupt(0x10, 0xe*256 + keyPress, 0, 0, 0);//prints out first char

	while(keyPress != 0xd) //0xd is the enter key
	{
		if(keyPress == 0x8 && i>0)//0x8 is the backspace key
		{ 
		        i--;//this decrements array index so that it won't
			    //save the character deleted by backspace
		}
		else if(keyPress == 0x8)
		{
		//this handles if array index is already at 0
			i=0;
		}
		else
		{
			//saves char to the array if not a backspace
			line[i] = keyPress;
			i++;
		}
		//gets next char
		keyPress = interrupt(0x16, 0, 0, 0, 0);
		//prints next char
		interrupt(0x10, 0xe*256 + keyPress, 0, 0, 0);

		//if next char is backspace, overwrite prior char with space 
		//and backspace again for the screen
		if(keyPress == 0x8)
		{
		interrupt(0x10, 0xe*256 + ' ', 0, 0, 0);
		interrupt(0x10, 0xe*256 + 0x8, 0, 0, 0);
		}
	}
	line[i] = 0xa; //line feed added to end of array
	line[i+1] = 0x0; //"end of string" added to end of array
	interrupt(0x10, 0xe*256 + 0xd, 0, 0, 0);//prints enter on screen
	interrupt(0x10, 0xe*256 + 0xa, 0, 0, 0);//prints new line on screen
}

void handleInterrupt21( int ax, int bx, int cx, int dx)
{
	if(ax == 0){
		printString(bx);
	}else if(ax == 1){
		readString(bx);
	}else if(ax == 2){
		readSector(bx, cx);
	}else if (ax == 3){
		readFile(bx, cx, dx);
	}else if(ax == 4){
		executeProgram(bx);
	}else if(ax == 5){
		terminate();
	}else if(ax == 6){
		writeSector(bx, cx);
	}else if(ax == 7) {
		deleteFile(bx);
	}else {
		printString("Error, ax should be less than 8");
	}
}

void readSector(char* buffer, int sector)
{
	int track = 0;
	int head = 0;
	int relSector = sector+1;
	interrupt(0x13, 2*256+1, buffer, track*256+relSector,head*256+0x80);
}

void readFile(char* name, char* buffer, int* sectorsRead)
{
	int entry;
	char dir[512];
	readSector(dir,2);

	for(entry = 0; entry < 512; entry = entry + 32)
	{
		if(dir[entry]==name[0] && dir[entry+1]==name[1] && dir[entry+2]==name[2] && dir[entry+3]==name[3] && dir[entry+4]==name[4] && dir[entry+5]==name[5])
		{
			//use for loop to load the file
			int e;
			for(e=entry+6; e < (entry + 32); e++)
			{   //entry now increments by one instead of 32
			readSector(buffer, dir[e]);
			buffer += 512;
			*sectorsRead = *sectorsRead+1;
			}
		}
	}
}

void executeProgram(char* name)
{
	int sectorsRead;
	char buffer[13312];
	int address;
	readFile(name, buffer, &sectorsRead);

	//this prevents trying to execute a file that doesn't exist
	if(sectorsRead > 0)
	{
		for(address = 0; address < 13312; address++)
		{
			putInMemory(0x2000, address, buffer[address]);
		}
		launchProgram(0x2000);
	}
}

void terminate()
{
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';
	executeProgram(shell);
	while(1);
}

void writeSector(char* buffer, int sector)
{
        int track = 0;
        int head = 0;
        int relSector = sector+1;
        interrupt(0x13, 3*256+1, buffer, track*256+relSector,head*256+0x80);
}

void deleteFile(char* name)
{
	int entry;
	char dir[512];
	char map[512];

	readSector(dir, 2);
	readSector(map, 3);

	for(entry = 0; entry < 512; entry = entry + 32)
	{
		if(dir[entry]==name[0] && dir[entry+1]==name[1] && dir[entry+2]==name[2] && dir[entry+3]==name[3] && dir[entry+4]==name[4] && dir[entry+5]==name[5])
		{
			dir[entry] = '\0';

			int e;
                        for(e=entry+6; e < (entry + 32); e++)
			{
				if(map[e] == dir[e])
				{
					map[e] = 0;
				}
			}
		} 
	}
	writeSector(dir, 2);
	writeSector(map, 3);

}
