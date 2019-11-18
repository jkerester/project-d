//James Kerester
main()
{
	while(1)
	{
		char line[80];
		syscall(0, "SHELL>");
		syscall(1, line);

		// "type" command
		if(line[0]=='t' && line[1]=='y' && line[2]=='p' && line[3]=='e')
		{
			char buffer[13312];
			int sectorsRead;
			char fileName[6];
			int i;
			//puts anything typed after "type" into an array
			for(i=0; i<6; i++)
			{
				fileName[i] = line[i+5]; 
			}
			//calls readFile to load the file into buffer
			syscall(3, fileName, buffer, &sectorsRead);
			//prints the file if found
			if(sectorsRead > 0)
			{
				syscall(0, buffer);
			}
			else
			{
				syscall(0, "File not found.\r\n");
			}
		}
		//"exec" command
		else if(line[0]=='e' && line[1]=='x' && line[2]=='e' && line[3]=='c')
		{
			char fileName[6];
			int i;
			//puts everything typed after "exec" into an array
			for(i=0; i<6; i++)
                        {
                                fileName[i] = line[i+5]; 
                        }
			//calls executeProgram to execute the file typed
			syscall(4, fileName); 
			syscall(0, "File not found.\r\n");

		}
		else if(line[0]=='d' && line[1]=='i' && line[2]=='r')
		{
			int entry;
			char dir[512];
			syscall(2, dir, 2);

			for(entry = 0; entry < 512; entry = entry + 32)
			{
				if(dir[entry] != '\0')
				{
					char fileName2[7];
					//need to terminate file name
					//make sure filename is 7 long
					//put the terminate filename after
					//the char fileName2[7] = 0?? or \0
					int i;
					for(i=0; i<6; i++)
					{
						fileName2[i] = dir[entry+i];
					}
					fileName2[6] = '\0';
					syscall(0, fileName2);
					syscall(0, "\r\n");
				}
			}


		}
		else if(line[0]=='d' && line[1]=='e' && line[2]=='l')
		{
			char fileName[6];
			int i;
			for(i=0; i<6; i++)
			{
				fileName[i] = line[i+4];
			}
			syscall(7, fileName);
		}
		else
		{
			syscall(0, "Command not recognized.\r\n");
		}
		//this terminates and calls for shell prompt again
		syscall(5);
	}
}
