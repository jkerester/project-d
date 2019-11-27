//James Kerester and Molly Croke
main()
{
	while(1)
	{

		char line[80];
	//	syscall(0, line); //test to see whats in line
		syscall(0, "SHELL>");
		syscall(1, line);
	//	syscall(0, line); //test to see whats in line

		// "type" command
		if(line[0]=='t' && line[1]=='y' && line[2]=='p' && line[3]=='e')
		{
			int sectorsRead=0;
			int i=0;
			int m;
			char buffer[13312];
			char fileName[7];

			for(m=0; m<7; m++)
			{
				fileName[m] = '\0';
			}
			//puts anything typed after "type" into an array

			while(line[i+5] != 0xa)
			{
				fileName[i] = line[i+5]; 
				i++;
			}



			//calls readFile to load the file into buffer
			syscall(3, fileName, buffer, &sectorsRead);
			//prints the file if found
			if(sectorsRead !=  0)
			{
				syscall(0, buffer);
				syscall(0, "\r");
			}
			else
			{
				syscall(0, "File not found.\r\n");
			}
		}
		//"exec" command
		else if(line[0]=='e' && line[1]=='x' && line[2]=='e' && line[3]=='c')
		{
			int i=0;
			int m;
			char fileName[7];

			 for(m=0; m<7; m++)
                        {
                                fileName[m] = '\0';
                        }

			  while(line[i+5] != 0xa)
                        {
                                fileName[i] = line[i+5]; 
                                i++;
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
			int i=0;
			int m;
			char fileName[7];

			 for(m=0; m<7; m++)
                        {
                                fileName[m] = '\0';
                        }

		          while(line[i+5] != 0xa)
                        {
                                fileName[i] = line[i+5]; 
                                i++;
                        }
			syscall(7, fileName);
		}
		else if(line[0]=='c' && line[1]=='o' && line[2]=='p' && line[3]=='y')
		{
			int sectorsRead=0;
			int i=0;
			int e=0;
			int x;
			int z;
			char buffer[13312];
			char fileName[7];
			char fileName2[7];

			for(x=0; x<7; x++)
                        {
                                fileName[x] = '\0';
                        }

			for(z=0; z<7; z++)
                        {
                                fileName2[z] = '\0';
                        }

			while(line[i+5] != ' ')
			{
				fileName[i] = line[i+5];
				i++;
			}
			//syscall(0, fileName); //test

			syscall(3, fileName, buffer, &sectorsRead);


			while(line[i+6] != 0xa)
			{
				fileName2[e] = line[i+6];
				i++;
				e++;
			}
		//	syscall(0,fileName2);//test
		//	syscall(0, buffer);//test

			syscall(8, buffer, fileName2, sectorsRead); 
		}
		else if(line[0]=='c' && line[1]=='r' && line[2]=='e' && line[3]=='a' && line[4]=='t' && line[5]=='e')
		{
			int buffTotal=0;
			int x;
			int i=0;
			char textLine[80];
			char buffer[13312];
			char fileName[7];

			for(x=0; x<7; x++)
			{
				fileName[x] = '\0';
			}
			while(line[i+7] != 0xa)
			{
				fileName[i] = line[i+7];
				i++;
			}


			syscall(1, textLine);
			i=0;
			while(textLine[i] != 0xa)
			{
				buffer[i] = textLine[i];
				i++;
			}
		//	buffer[buffTotal+i+1]=0x0D; //"\r"
                  //    buffer[buffTotal+i+2]=0x0A; //"\n"
                        buffTotal+=i;

			while(textLine[0] != 0x0A)
			{
				i=0;
				syscall(1, textLine);
				while(textLine[i] != 0xa)
				{
					buffer[buffTotal+i] = textLine[i];
					i++;
				}
			//	buffer[buffTotal+i+1]=0x0D; //"\r"
			//	buffer[buffTotal+i+2]=0x0A; //"\n"
				buffTotal+=i;
			}
			//syscall(0, buffer);//test
			syscall(8, buffer, fileName, 1);
		}
		else
		{
			syscall(0, "Command not recognized.\r\n");
		}
		//this terminates and calls for shell prompt again
		syscall(5);
	}
}
