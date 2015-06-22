 /********************************************************************
 * Name: Shahzeb Siddiqui				ID: 124211  				 *
 * Name: Omniah Nagoor					ID: 123923      			 *
 * Final Project: Encryption Techniques in D Programming Language    *
 ********************************************************************/
 
//////////////////////////
// Implemented Features //
////////////////////////// 
 /*
 - Associative Array - have an index that is not necessarily an integer. This index is called the key, and its type is called the KeyType.
 - Dynamic Array     - have variable number of elements at run time
 - Variadic Function - Function takes variable number of arg
 - 	
 - foreach()         - iterator that start by index 0 to the end of the array  
 - ~=				 - means append
 - Array Slicing     - means specify the subarray and create new pointer refrence to it 
 - .length			 - returns the number of values in the array.
 - .dup              - Create a new array of the same size and copy the contents of the array into it.
 - .reverse			 - reverses in place the order of the elements in the array and returns the array.
 - fun(...)			 - varaidic function parameters
 - Nested Function   - Function inside other function (can share the a global var)
 - Assert() 		 - Check if the given Expr
 - to!string(i)      - to family of functions converts a value from type Source to type Targe.
 
 - write(T...)(T args)   - For each argument arg in args, format the argument (as per to!(string)(arg)) and write the resulting string to args[0]. 
					       A call without any arguments will fail to compile.
 - writeln(T...)(T args) - Equivalent to write(args, '\n'). Calling writeln without arguments is valid and just prints a newline to the standard output.
 - readln()				 - Read line from stream fp. It return a string datatype	
 - contract programming  - preconditioner check for the Expression and if it is true then execute the body.
 */
 
///////////////
//  Headers  //
///////////////
 import std.math; 
 import std.conv;
 import std.stdio;
 import std.random;
 import std.string;


////////////////
// Prototypes //
////////////////
 void ceasar_cipher(int shiftletter);
 void variable_keys_ceaser_cipher(string plaintext,in int[] keysarray ...);
 void welcome();
 void Hill_Cipher();
 void toBinary(ulong num);
 void Hell_Cipher();
 ulong gen_key(string keyname); 
 void streamcipher(ulong key);
 
//////////////////////
// Global variables //
////////////////////// 
 ulong long_max =  ulong.max;
 ulong [string] keylog;			 // Associate Array - contains keylog with person name and associated key
 string [] person_with_key;      // Dynamic Array - have variable number of elements at run time
 int numkeys_assign = 0;
 immutable int charCount = 82;


/******************************************************
*  main function:				    				  * 
* The program core which call program function  	  *
* depending on the user choice (interactive program)  *
* The user choice will determine which Encryption     * 
* algorithm will be implemented				          *
******************************************************/
 void main()
 {
	
	person_with_key = ["Shahzeb","Ali","Muhammad","Kamran","Farhan","Rahim","Abdullah","Fatima","Aisha","Enas","Umar","Usman","Othman"];
	string choice; 	 // user choice
	bool quit = false;
	
	do 			     // Interactive code 
	{
		welcome();
			
		write("\n Please select an option (1-5):");
		choice = stdin.readln();

		switch (choice)
		{
			case "1\n": 
				{			
				
					int key = uniform(1,charCount);    // Give a random number between the range 
					ceasar_cipher(key); 
					break;			
				}
			case "2\n":
				{
					int key1, key2 ,key3, key4, key5;
					key1 = uniform(1,charCount);
					key2 = uniform(1,charCount);
					key3 = uniform(1,charCount);
					key4 = uniform(1,charCount);
					key5 = uniform(1,charCount);
					
					write("Enter Plaintext: ");
					string plaintext = stdin.readln();
					
					variable_keys_ceaser_cipher(plaintext,[key1]);  					// Variadic Function - Function with variable number of arg
					writeln(); variable_keys_ceaser_cipher(plaintext,[key1,key2,key3]);
					writeln(); variable_keys_ceaser_cipher(plaintext,[key1,key2,key3,key4,key5]);
					break;
				}
			case "3\n":
				{
					// block cipher 
					Hill_Cipher();
					break;
				}
			case "4\n":
				{
						if (numkeys_assign == 0)
						{
							foreach(name; person_with_key)  // iterator that start by index 0 to the end of the array  
							{
								numkeys_assign++; 		
								gen_key(name);
								writeln("keylog[",name,"] = ", keylog[name]);
							}
						}	
											
						for (int z = 0; z < numkeys_assign; z++)
							{
								writeln("Encrypting with ", person_with_key[z], "'s private key\n");
								streamcipher(keylog[person_with_key[z]]);								
							}
						numkeys_assign = 0;
						break;
				}
			case "5\n":
				quit = true;
			break;
			default:	
				writeln("Invalid Input! \nPlease try again!\n");
			break;
					
		}
	} while (!quit);	

}

/******************************************************
*  welcome function:			    				  * 
* It is only a printing function which print the      *
* choices to the user						          *
******************************************************/
void welcome()    
{
	writeln();
	write("\n\t\t -------------------------------------------------------- \n");
	
	write("\n\t\t      ");	
	writeln("Welcome to Encryption Program implemented in D \n");
	
	write("\t\t\t ");	
	writeln("1. Ceaser Cipher Encryption");
	
	write("\t\t\t ");
	writeln("2. Ceaser Cipher Encryption with multiple keys");
	
	write("\t\t\t ");
	writeln("3. Hill Cipher Encryption");
	
	write("\t\t\t ");
	writeln("4. Stream Cipher Encryption with Private Keys");
	
	write("\t\t\t ");
	writeln("5. Quit Program");
	
	writeln();
	write("\t\t -------------------------------------------------------- \n");
}

/******************************************************
*  toBinary function:			    				  * 
*  take ulong number and convert it to binary for     *
*  printing purposes						          *
******************************************************/
void toBinary(ulong num)
{
	char[] binary,binaryreverse;
	int index = 0;
	ulong orignum = num;
	
	while (num != 0)     				// Convert Decimal to Binary 
	{
		if (num % 2 == 0)
		  binary ~= '0';
		else
		  binary ~= '1';
		
		num /= 2;	
	}
	
	while (binary.length != 64) 	   // 0s Padding to 64 bit
	  binary ~= '0';
	
	binaryreverse = binary.reverse;    							// Reverse the order of elements in the array 
	writeln(orignum,  "= ", binaryreverse);
	
}

/******************************************************
*  ceasar_cipher function:							  * 
* This function take a plain text and cipher it with  *
* the given key which represtent the shift amount it  *
* the table  						                  *
******************************************************/
void ceasar_cipher(int key)
{  

 
  write("Enter Plaintext: ");
  string plaintext = stdin.readln();
  
  int i,j;
  char table[charCount] = "abcdefghijklmnopqrstuvxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*(),./;'[]{}:";
  char[] newtable = table[key .. charCount] ~ table[0 .. key];	// Array Slicing - means specify the subarray and create new pointer refrence to it 
																// Decompose the array from key to the end and append it with the rest
  char[] plaintext_inchar = plaintext.dup;  					// Doulbecate the content of the array into a dynamic array 
  char[] ciphertext;									
  

  for (i = 0; i < plaintext_inchar.length; i++)
  {
	for (j = 0; j < table.length; j++)
	{
		if (plaintext_inchar[i] == table[j])
		{		
			ciphertext ~= newtable[j];
		}	
	}	
  }	 

  writeln("key = ",key);
  writeln("table    = ",table);
  writeln("newtable = ",newtable);
  writeln("plaintext =  ", plaintext);
  writeln("ciphertext = ",ciphertext);
  
}

/******************************************************
*  variable_keys_ceaser_cipher function:			  * 
* This function take a plain text and cipher it with  *
* multiple number of keys which ciphers each character*
* in the plaintext by a different key in the table 	  *
******************************************************/
void variable_keys_ceaser_cipher(string plaintext, int[] numkeys ...) // varaidic function parameters
{    
   int i, index;
   
   char table[charCount] = "abcdefghijklmnopqrstuvxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*(),./;'[]{}:";  
   
   char [][charCount] newtable;   
   char [] plaintext_inchar = plaintext.dup;			 // Dublicate the content of the array into a dynamic array  
   char [] ciphertext;   
   
   
	/******************************************************
	*  ceasarcipher function:							  * 
	* This function take a character and cipher it with   *
	* the given key which represtent the shift amount it  *
	* the table  						                  *
	******************************************************/
   void ceasercipher(char letter, int key, int keyindex) // Nested Function Implementation
   {
		int j;				
				
		for (j = 0; j < table.length; j++)
		{
			if (letter == table[j])
				ciphertext ~= newtable[keyindex][j];
		}	
		
		writeln("letter = ",letter,"\tciphertext = ",ciphertext, "\t key = ", key);
   }   
   
   writeln("table      = ",table);
   for (i = 0; i < numkeys.length; i++)
   {
     newtable[i] = table[numkeys[i] .. charCount] ~ table[0 .. numkeys[i]];
	 writeln("newtable[",i,"]= ",newtable[i], "\tkey = ",numkeys[i]);
   }
   
   index = 0;
   
   writeln("\nEncrypting Plain text = ", plaintext);
   
   
   while(index < plaintext_inchar.length) 						  // Encrypt the plain text char by char
   {
		for (i = 0; i < numkeys.length; i++)
		{      
			if (index == plaintext_inchar.length)
				break;
			else
			ceasercipher(plaintext_inchar[index++],numkeys[i],i); // Enc each char with the given key 
		}
	}
	
}

/******************************************************
*  gen_key function:								  * 
*	This function generates a private key for each    *
*   person using contracts and stores the key in a    *
*   keylog to ensure unique key everytime			  *
******************************************************/
ulong gen_key(string keyname)
{
  ulong check_key_if_valid (ulong key)					// Another example of Nested Functions
														// Assert - Check if the key between 0 and  long_max
	in { assert (key >= 0 && key <= long_max);}			// contract programming: preconditioner check for key value between 0 - long_max
	body 												// contract programming: execute body which generates a unique key everytime
	  {
	    bool keymatch = false;							// check if the new key exists in the keylog
		
		if (keylog.length == 0)			// The first key is added to keylog without any checks
			keylog[keyname] = key; 
		else							// Check if private key is unique (not in keylog)
		{						
			do
			{
				for (int i = 0; i < numkeys_assign - 1; i++)		// looping over keys in keylog to check if key is unique
				{				
					if (keylog[person_with_key[i]] == key)
						keymatch = true;
				}
				
				if (keymatch == true)								// if there is keymatch in keylog, create new key
				{
					key = uniform (0,long_max);						// create new random key for uniqueness
				}
				
			}
			while (keymatch == true);								// loop if there is a matched key in keylog
			keylog[keyname] = key;
		}
		return key;
	  }
  
  ulong key = uniform (0, long_max);								// create initial key
  ulong userkey = check_key_if_valid(key);							// generate unique private key 
  
  return userkey;
  
}

/*********************************************************
*  Hill_Cipher function:							     * 
*  It is an implementation of Block cipher. Block        *
*  ciphers encrypting a block of letters simultaneously. *
*  The key is an n x n matrix whose				         * 
*  entries are integers in range 0 to 82 (number of      *
*  element in the table)                                 *
*********************************************************/
void Hill_Cipher()
{
	write(" \nEnter Plaintext:");
	string Message  = stdin.readln(); 	 		   // Read the input Message
    
	if (Message.length < 5 )  					   // Since the key block is 2x2 the Message length must be more than 3 when apply it to the Mat * Vec
	{
		write(" \nError the Message length should be more than 3!");
		return;
	}
	char [] charMessage = Message.dup;	           // Define a dynamic array to copy the input Message to it	
	int MessageSize;
	if ((charMessage.length-1) % 2 != 0) 		   // Check the the size if it is dividable by 2 since the key mat is 2x2 the Message size has to be divided by 2
	{ 
		charMessage[charMessage.length-1] = '.';   // if it is odd add . to it end 
		MessageSize = charMessage.length;
	}
	else
		MessageSize = charMessage.length-1;
	
	//////////////////////
	// Var Declarations //
	//////////////////////
	int [] key;
	int [] src;
	int [] sum;
	int KeySize = 4;
	int [string] table;							   // Associate Array - have index that is string type and it map to int number which represent the value
	char EncMssg[] = charMessage.dup;   		   // Doulbecate the content of the array into a dynamic array 
	immutable string s= "abcdefghijklmnopqrstuvxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*(),./;'[]{}:"; 
		
	for(int i; i < KeySize; i++)  				   // The key is 2x2 Mat
	{ 
	  key ~= uniform(0,s.length);		 		   // Init the key Mat with random numbers between the range 
	}  
	writeln("\n The Plaintext Message: ", charMessage);
	
	
foreach(i; 0 .. s.length)  						// 82 char elements in the table
	table[to!string(s[i])] = i;	     		    // Map each char in the table with it correct number
	
	
	for(int i = 0; i < MessageSize; i++){   	// Map and Append each char in the Message with it correct number from the table
		//writeln(" i= ", i ," ",charMessage[i]);
		src ~= table[to!string(charMessage[i])];      // converting values from char type to string 
	}											   	  // This process is done to get the int value from the table
		
	writeln("\n The Matrix values (Message): ", src);
	writeln("\n The Key values: ", key);
	
	foreach(i;0 .. MessageSize)		  				   // Init the sum elements with 0
		sum ~= 0;
		
	writeln("\n The sum values before: ", sum);	
	
	int k=0;
	do{
		int i=0;
				// (Key) Mat * (Message) vec = Vec Enc Message  
				sum[k]    = key[i]   * src[k]; 	  	    // for the first row in the key Mat
				sum[k]   += key[i+1] * src[k+1];
				
				sum[k+1]  = key[i+2] * src[k]; 	        // for the second row in the key Mat
				sum[k+1] += key[i+3] * src[k+1];
		
		k+=2;
		
	}while(k < MessageSize); 			    // loop over all Plain text char
	
	writeln("\n Result += Key * Matrix: ", sum); 
	
	foreach(i;0 .. MessageSize)		   		// Mod the result of the vec * Mat with 28 which is the length of the table elements
		sum[i] = sum[i] % s.length;	
	
	writeln("\n Result = Result % ", s.length,": ", sum);  
	
	foreach(i;0 .. MessageSize) 		    // Map the final result numbers with the table to get its mapped letter
		EncMssg[i] = s[sum[i]];
	
	if(MessageSize == charMessage.length)
	{	
		write("\n The Enc Message: ");
		for (int i = 0; i < EncMssg.length - 1; i++)
			{
				write(EncMssg[i]);
			}
	
	}
	else
		write("\n The Enc Message: ", EncMssg);	
	
}


/******************************************************
*  streamcipher function:							  * 
*  	This function cipher a plaintext in numeric form  * 
*	with a private key. The cipher is performed on a  * 
*   64 bit plaintext by XOR it with private key. This *
*	is an example of a symmetric key encryption.  	  *			                              
******************************************************/
void streamcipher(ulong key)
{
	ulong Packet = 1000000000000000000;
	ulong EncryptedPacket;
	
	EncryptedPacket = Packet ^ key;															// XOR plaintext with private key
	writeln("\tPacket = ", Packet, "\t Key = ", key, "\tEncryptedPacket = ", Packet, " XOR ", key);	
	write("Packet =          "); toBinary(Packet);
	write("Key =             ");   toBinary(key);
	writeln ("------------------------------------------------------------------------------------------------------");
	write("EncryptedPacket = "); toBinary(EncryptedPacket);
	writeln("\n");
}