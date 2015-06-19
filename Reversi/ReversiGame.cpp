#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
using namespace std;

#define BoardSize 8

//#define TEST 1
#define TEST 2

int board_turnpiece(int row, int col, int capturepiece, int playerpiece, int flag); 
void checkUp(int row, int col, int playerpiece);
void checkDown(int row, int col, int playerpiece);
void checkRight(int row, int col, int playerpiece);
void checkLeft(int row, int col, int playerpiece);
void checkpositivediagDown(int row, int col, int playerpiece);
void checkpositivediagUp(int row, int col, int playerpiece);
void checknegativediagDown(int row, int col, int playerpiece);
void checknegativediagUp(int row, int col, int playerpiece);
void computermove_easy(int &x, int &y, int cpu_piece);
void computermove_medium(int &x, int &y, int cpu_piece);
void displayinitgameboard(int sel);
void filewrite_displayactivegameboard();
void displayactivegameboard(int sel);
void getboardinfo(int winner, int simenabled);
void init();
void initboardweights();
void menu();


void playervsplayergame(int);
void playervscomputergame(int);
void computervscomputergame(int,int);

void simulation();


bool boardpositiontaken[BoardSize][BoardSize];
int activepositiontaken[BoardSize][BoardSize];
float Oboardweight[BoardSize][BoardSize];
float Xboardweight[BoardSize][BoardSize];
int Ocount = 2, Xcount = 2;
int nearest_playerpiece_row,nearest_playerpiece_col;
int cpuOsim_diff,cpuXsim_diff;

int computerXwins,computerOwins;
int _1cornergame,_2cornergame,_3cornergame,_4cornergame;
int _1cornerwins,_2cornerwins,_3cornerwins,_4cornerwins;
int _1corner_O,_2corner_O,_3corner_O,_4corner_O;
int _1corner_X,_2corner_X,_3corner_X,_4corner_X;
int _7edgecnt,_14edgecnt,_21edgecnt;
bool gameover = false;
bool pass;
FILE *fp,*fp_game;
int main ()
{
   
   int i,j;
   int seed;
   seed = time(NULL);	
   srand(seed);  
	
   int user_menu_selection;	
   do 
   {
	   //system("clear");
	   menu();
	   cout << "\n\nGame Selection > ";
	   cin >> user_menu_selection;
	   //printf("%d\n",user_menu_selection);
	   if (user_menu_selection == 5)
		exit(1);
	   else if (user_menu_selection >= 1 && user_menu_selection <= 4)
	     break;	
   }
   while (true);      
	
   if (user_menu_selection == 1)
	playervscomputergame(user_menu_selection);	
   else if(user_menu_selection == 2)
    playervsplayergame(user_menu_selection); 
   else if (user_menu_selection == 3)
	 computervscomputergame(user_menu_selection,0);
   else if (user_menu_selection == 4)	
     simulation(); 
	 
cin >> user_menu_selection;
	
}
void simulation()
{
	
	fp = fopen("simoutput.txt","w");
	fp_game = fopen("simgame.txt","w");
	int numgames;
	int i;
	int simenabled = 1;
	char *str1, *str2;
	str1 = (char*)malloc(20*sizeof(char));
	str2 = (char*)malloc(20*sizeof(char));
	printf("Number of Games: ");
	cin >> numgames;
		
	while (true)
	{ 
	   system("clear");
	   cout << setw(40) << "Select Computer O Difficulty\n";
	   cout << setw(30) << "1. Easy\n";
	   cout << setw(33) << "2. Medium\n\n";   
	   cout << "Selection >";
	   cin >> cpuOsim_diff;
	   if (cpuOsim_diff == 1 || cpuOsim_diff == 2)
		 break;
	  }
	   
	while (true)
	{ 
	   system("clear");
	   cout << setw(40) << "Select Computer X Difficulty\n";
	   cout << setw(30) << "1. Easy\n";
	   cout << setw(33) << "2. Medium\n\n";   
	   cout << "Selection >";
	   cin >> cpuXsim_diff;
	   if (cpuXsim_diff == 1 || cpuXsim_diff == 2)
		 break;
	}
	
	if (cpuOsim_diff == 1)
	  str1 = "easy";
	else if(cpuOsim_diff == 2)
	  str1 = "medium";
	
	if (cpuXsim_diff == 1)
	  str2 = "easy";
	else if(cpuXsim_diff == 2)
	  str2 = "medium";
	printf("str1=%s",str1);    
	
	initboardweights();
	   
	computerOwins = computerXwins = 0;	
	_1cornergame = _2cornergame = _3cornergame = _4cornergame = 0;
	_1corner_O = _2corner_O = _3corner_O = _4corner_O = 0;
	_1corner_X = _2corner_X = _3corner_X = _4corner_X = 0;
	_7edgecnt = _14edgecnt = _21edgecnt = 0;
	for (i = 0; i < numgames; i++)
	{		
		printf("Game %d: ",i);		    
		fprintf(fp,"Game %d: ",i);		    
		fprintf(fp_game,"Game %d: ",i);		    
		computervscomputergame(3,simenabled);	
	}
	_1cornerwins = _1corner_O + _1corner_X;
	_2cornerwins = _2corner_O + _2corner_X;
	_3cornerwins = _3corner_O + _3corner_X;
	_4cornerwins = _4corner_O + _4corner_X;
	printf("\n\n Summary: \n Total Games: %d \t Computer O Wins: %d \t Computer X Wins: %d \t Tie Games: %d \n",numgames,computerOwins,computerXwins,numgames-computerOwins-computerXwins);
	printf("%d of %d Games won with at least 7/28 edges with win ratio %f\n",_7edgecnt, numgames,(float)_7edgecnt/numgames);
	printf("%d of %d Games won with at least 14/28 edges with win ratio %f\n",_14edgecnt, numgames,(float)_14edgecnt/numgames);		
	printf("%d of %d Games won with at least 21/28 edges with win ratio %f\n\n",_21edgecnt, numgames,(float)_21edgecnt/numgames);
	
	printf("%d of %d Games won with 1 corner with win ratio %f\n",_1cornerwins,_1cornergame,(float)_1cornerwins/_1cornergame);
	printf("Games with 1 corner - Computer O: %d/%d \t Computer X: %d/%d\n\n",_1corner_O,_1cornerwins,_1corner_X,_1cornerwins);	
	printf("%d of %d Games won with 2 corner with win ratio %f\n",_2cornerwins,_2cornergame,(float)(_2cornerwins)/_2cornergame);
  	printf("Games with 2 corner - Computer O: %d/%d \t Computer X: %d/%d\n\n",_2corner_O,_2cornerwins,_2corner_X,_2cornerwins);  	
  	printf("%d of %d Games won with 3 corner with win ratio %f\n",_3cornerwins,_3cornergame,(float)(_3cornerwins)/_3cornergame);
	printf("Games with 3 corner - Computer O: %d/%d \t Computer X: %d/%d\n\n",_3corner_O,_3cornerwins,_3corner_X,_3cornerwins);	
	printf("%d of %d Games won with 4 corner with win ratio %f\n",_4cornerwins,_4cornergame,(float)(_4cornerwins)/_4cornergame);
	printf("Games with 4 corner - Computer O: %d/%d \t Computer X: %d/%d\n\n",_4corner_O,_4cornerwins,_4corner_X,_4cornerwins);
	
	
	fprintf(fp,"\n\n Summary: \n Total Games: %d \t Computer O Wins: %d \t Computer X Wins: %d \t Tie Games: %d \n",numgames,computerOwins,computerXwins,numgames-computerOwins-computerXwins);
	fprintf(fp,"%d of %d Games won with at least 7/28 edges with win ratio %f\n",_7edgecnt, numgames,(float)_7edgecnt/numgames);
	fprintf(fp,"%d of %d Games won with at least 14/28 edges with win ratio %f\n",_14edgecnt, numgames,(float)_14edgecnt/numgames);		
	fprintf(fp,"%d of %d Games won with at least 21/28 edges with win ratio %f\n\n",_21edgecnt, numgames,(float)_21edgecnt/numgames);
	
	fprintf(fp,"%d of %d Games won with 1 corner with win ratio %f\n",_1cornerwins,_1cornergame,(float)_1cornerwins/_1cornergame);
	fprintf(fp,"Games with 1 corner - Computer O: %d/%d \t Computer X: %d/%d\n\n",_1corner_O,_1cornerwins,_1corner_X,_1cornerwins);	
	fprintf(fp,"%d of %d Games won with 2 corner with win ratio %f\n",_2cornerwins,_2cornergame,(float)(_2cornerwins)/_2cornergame);
  	fprintf(fp,"Games with 2 corner - Computer O: %d/%d \t Computer X: %d/%d\n\n",_2corner_O,_2cornerwins,_2corner_X,_2cornerwins);  	
  	fprintf(fp,"%d of %d Games won with 3 corner with win ratio %f\n",_3cornerwins,_3cornergame,(float)(_3cornerwins)/_3cornergame);
	fprintf(fp,"Games with 3 corner - Computer O: %d/%d \t Computer X: %d/%d\n\n",_3corner_O,_3cornerwins,_3corner_X,_3cornerwins);	
	fprintf(fp,"%d of %d Games won with 4 corner with win ratio %f\n",_4cornerwins,_4cornergame,(float)(_4cornerwins)/_4cornergame);
	fprintf(fp,"Games with 4 corner - Computer O: %d/%d \t Computer X: %d/%d\n\n",_4corner_O,_4cornerwins,_4corner_X,_4cornerwins);
	
	printf("\tComputer O Statistics \t\t\t\t Computer X statistics\n");
	printf("            Games won: %d \t\t\t\t  Games won: %d\n",computerOwins,computerXwins);
	printf("     Difficulty Level: %s \t\t\t\t Difficulty: %s\n",str1,str2);
	printf("1 corner O game ratio: %d/%d = %f \t      1 corner X game ratio: %d/%d = %f\n",_1corner_O,numgames,(float)_1corner_O/numgames,_1corner_X,numgames,(float)_1corner_X/numgames);
	printf("2 corner O game ratio: %d/%d = %f \t      2 corner X game ratio: %d/%d = %f\n",_2corner_O,numgames,(float)_2corner_O/numgames,_2corner_X,numgames,(float)_2corner_X/numgames);
	printf("3 corner O game ratio: %d/%d = %f        3 corner X game ratio: %d/%d = %f\n",_3corner_O,numgames,(float)_3corner_O/numgames,_3corner_X,numgames,(float)_3corner_X/numgames);
	printf("4 corner O game ratio: %d/%d = %f \t      4 corner X game ratio: %d/%d = %f\n",_4corner_O,numgames,(float)_4corner_O/numgames,_4corner_X,numgames,(float)_4corner_X/numgames);

	fprintf(fp,"\tComputer O Statistics \t\t\t\t Computer X statistics\n");
	fprintf(fp,"           Games won: %d \t\t\t\t  Games won: %d\n",computerOwins,computerXwins);
	fprintf(fp,"    Difficulty Level: %s \t\t\t\t Difficulty: %s\n",str1,str2);
	fprintf(fp,"1 corner O game ratio: %d/%d = %f \t      1 corner X game ratio: %d/%d = %f\n",_1corner_O,numgames,(float)_1corner_O/numgames,_1corner_X,numgames,(float)_1corner_X/numgames);
	fprintf(fp,"2 corner O game ratio: %d/%d = %f \t      2 corner X game ratio: %d/%d = %f\n",_2corner_O,numgames,(float)_2corner_O/numgames,_2corner_X,numgames,(float)_2corner_X/numgames);
	fprintf(fp,"3 corner O game ratio: %d/%d = %f        3 corner X game ratio: %d/%d = %f\n",_3corner_O,numgames,(float)_3corner_O/numgames,_3corner_X,numgames,(float)_3corner_X/numgames);
	fprintf(fp,"4 corner O game ratio: %d/%d = %f \t      4 corner X game ratio: %d/%d = %f\n",_4corner_O,numgames,(float)_4corner_O/numgames,_4corner_X,numgames,(float)_4corner_X/numgames);

	
	fclose(fp);
	fclose(fp_game);
}
void playervsplayergame(int gamesel)
{
   
   int turnselect = 0;
   int x = 0,y = 0;
   int validmove;
   int numpieces = 4;
   int playerpiece, capturepiece;
   displayinitgameboard(gamesel);
   init();

   do 
   {
	pass = false;
	
	if (turnselect == 0)
	{
		playerpiece = 1;
		capturepiece = -1;
		cout << "Player O's turn"<< endl;
	}	
	else
	{
		playerpiece = -1;
		capturepiece = 1;
		cout << "Player X's turn"<< endl;	
	}
	do
	{			
		char usermove[10];			
		do 
		{			
			cout << "\nMove(format:'row,col' or 'p' to pass ):";
			cin >> usermove;
			
			x = int(usermove[0]) - 48;
			y = int(usermove[2]) - 48;
			if (((0 <= x) && (x <= 7) && (0 <= y) && (y <= 7))  || x 	== 64)
				break;
		}
		while (1);			
		
		if (x == 64) // asci letter 'p'
		{
			validmove = 1;
			pass = true;
		}
		if (pass == false)
		{			
			validmove = board_turnpiece(x,y,capturepiece,playerpiece,0);			
		
			if (validmove == 1)
			{
				cout << "Invalid Move" << endl;			
			}
		}
	}	while (validmove == false);
	
	if (pass == false)
	{
		validmove = board_turnpiece(x,y,capturepiece,playerpiece,1);			
		
		boardpositiontaken[x][y] = true;
		
		if (turnselect == 0)  
		{
			activepositiontaken[x][y] = 1;
			Ocount++;
		}
		else
		{
			activepositiontaken[x][y] = -1;
			Xcount++;
		}
	}
		
	displayactivegameboard(gamesel);	

	if (turnselect == 0) turnselect = 1;
	else				turnselect = 0;

	numpieces++;
	if (Ocount + Xcount == 64)
		gameover = true;
	
   } while(gameover == false);
	
	if (Ocount > Xcount)
		cout << "Player O wins the game!\n";
	else if (Ocount == Xcount)
		cout << "Tie Game!\n";
	else
		cout << "Player X wins the game!\n";	
		
}
void playervscomputergame(int gamesel)
{
   
   int turnselect = 0;
   int x = 0,y = 0;
   int i,j;
   int numpieces = 4;
   int playerpiece = 1, capturepiece = -1;
   int validmove;
   
   int cpu_difficulty = 0;
   while (true)
   { 
	   system("clear");
	   cout << setw(40) << "Select Computer Difficulty\n";
	   cout << setw(30) << "1. Easy\n";
	   cout << setw(33) << "2. Medium\n\n";   
	   cout << "Selection >";
	   cin >> cpu_difficulty;
	   if (cpu_difficulty == 1 || cpu_difficulty == 2)
	     break;
   }
   initboardweights();      
   displayinitgameboard(gamesel);
   init();

   do 
   {
	pass = false;
	
	
	if (turnselect == 0)
		cout << "Player's turn"<< endl;
	else
		cout << "Computer's turn"<< endl;	
		
	// player's turn
	if (turnselect == 0)
	{	
		do
		{
			char usermove[10];			
			do 
			{			
				cout << "\nMove(format:'row,col' or 'p' to pass ):";
				cin >> usermove;
				
				x = int(usermove[0]) - 48;
				y = int(usermove[2]) - 48;
				if (((0 <= x) && (x <= 7) && (0 <= y) && (y <= 7))  || x == 64)
					break;
			}
			while (1);			
			
			if (x == 64) // asci letter 'p'
			{
				validmove = 1;
				pass = true;
			}
			if (pass == false)
			{
				validmove = board_turnpiece(x,y,capturepiece,playerpiece,0);			
				
				if (validmove == 0)
				{
					cout << "Invalid Move" << endl;			
				}
			}
		} while (validmove == 0);
		
		if (pass == false)
			validmove = board_turnpiece(x,y,capturepiece,playerpiece,1);
	}
	// computer move
	else
	{
		sleep(2);
		if (cpu_difficulty == 1)
		computermove_easy(x,y,-1);
		else if(cpu_difficulty == 2)
		computermove_medium(x,y,-1);
		
		printf ("Computer X move : %d,%d\n",x,y);
	}	
	
	
	if (pass == false)
	{
		boardpositiontaken[x][y] = true;	
		if (turnselect == 0)  
		{
			activepositiontaken[x][y] = 1;			
		}
		else
		{
			activepositiontaken[x][y] = -1;			
		}
	}
	Ocount = Xcount = 0;
	for (i = 0; i < BoardSize; i++)
	{
		for (j = 0; j <  BoardSize; j++)
		{
			if (activepositiontaken[i][j] == 1)
			   Ocount++;
			else if(activepositiontaken[i][j] == -1)
				Xcount++;
		}
	}	
	displayactivegameboard(gamesel);	
	
	if (turnselect == 0) turnselect = 1;
	else				turnselect = 0;

	numpieces++;
	if (Ocount + Xcount == 64)
		gameover = true;
	
   } while(gameover == false);
	
	if (Ocount > Xcount)
		cout << "Player wins the game!\n";
	else if (Ocount == Xcount)
		cout << "Tie Game!\n";
	else
		cout << "Computer wins the game!\n";	
		
}
void initboardweights()
{
	int i,j;
//#if TEST==1
	Oboardweight[0][0] = 0.7 + rand() % 20 / 500.f;
	Oboardweight[0][7] = 0.7 + rand() % 20 / 500.f;
	Oboardweight[7][0] = 0.7 + rand() % 20 / 500.f;
	Oboardweight[7][7] = 0.7 + rand() % 20 / 500.f;
	
	Xboardweight[0][0] = 0.7 + rand() % 20 / 500.f;
	Xboardweight[0][7] = 0.7 + rand() % 20 / 500.f;
	Xboardweight[7][0] = 0.7 + rand() % 20 / 500.f;
	Xboardweight[7][7] = 0.7 + rand() % 20 / 500.f;
#if TEST==2
	Xboardweight[0][0] = 0.80 + rand() % 20 / 500.f;
	Xboardweight[0][7] = 0.80 + rand() % 20 / 500.f;
	Xboardweight[7][0] = 0.80 + rand() % 20 / 500.f;
	Xboardweight[7][7] = 0.80 + rand() % 20 / 500.f;
#endif 	
	for (i = 1; i < BoardSize - 1; i++)
	{
//#if TEST==1	
		Oboardweight[0][i] = 0.40 + rand() % 20 / 500.f;
		Oboardweight[i][0] = 0.40 + rand() % 20 / 500.f;
		Oboardweight[7][i] = 0.40 + rand() % 20 / 500.f;
		Oboardweight[i][7] = 0.40 + rand() % 20 / 500.f;
		
		Xboardweight[0][i] = 0.40 + rand() % 20 / 500.f;
		Xboardweight[i][0] = 0.40 + rand() % 20 / 500.f;
		Xboardweight[7][i] = 0.40 + rand() % 20 / 500.f;
		Xboardweight[i][7] = 0.40 + rand() % 20 / 500.f;
#if TEST==2	
		Xboardweight[0][i] = 0.50 + rand() % 20 / 500.f;
		Xboardweight[i][0] = 0.50 + rand() % 20 / 500.f;
		Xboardweight[7][i] = 0.50 + rand() % 20 / 500.f;
		Xboardweight[i][7] = 0.50 + rand() % 20 / 500.f;
#endif		
	}
	for (i = 1; i < BoardSize -1; i++)
	{
		for (j = 1; j < BoardSize - 1; j++)
		{
//#if TEST==1		
			Oboardweight[i][j] = 0.1 + rand() % 20 / 500.f;			
			Xboardweight[i][j] = 0.1 + rand() % 20 / 500.f;			
//#if TEST==2			
//			Xboardweight[i][j] = 0.05 + rand() % 200 / 500.f;			
//#endif			
		}
	}
//#if TEST==1			
	// top left corner adjacent 
	Oboardweight[0][1] = 0.05 + rand() % 20 / 500.f;
	Oboardweight[1][0] = 0.05 + rand() % 20 / 500.f;
	Oboardweight[1][1] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[0][1] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[1][0] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[1][1] = 0.05 + rand() % 20 / 500.f;
	
	// top right corner adjacent	
	Oboardweight[0][6] = 0.05 + rand() % 20 / 500.f;
	Oboardweight[1][6] = 0.05 + rand() % 20 / 500.f;
	Oboardweight[1][7] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[0][6] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[1][6] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[1][7] = 0.05 + rand() % 20 / 500.f;
	
	// bottom left corner adjacent	
	Oboardweight[6][0] = 0.05 + rand() % 20 / 500.f;
	Oboardweight[6][1] = 0.05 + rand() % 20 / 500.f;
	Oboardweight[7][1] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[6][0] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[6][1] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[7][1] = 0.05 + rand() % 20 / 500.f;
	
	// bottom right corner adjacent
	Oboardweight[6][6] = 0.05 + rand() % 20 / 500.f;
	Oboardweight[6][7] = 0.05 + rand() % 20 / 500.f;
	Oboardweight[7][6] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[6][6] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[6][7] = 0.05 + rand() % 20 / 500.f;
	Xboardweight[7][6] = 0.05 + rand() % 20 / 500.f;
/*
#if TEST==2
	Xboardweight[0][1] = 0.15 + rand() % 20 / 500.f;
	Xboardweight[1][0] = 0.15 + rand() % 20 / 500.f;
	Xboardweight[1][1] = 0.15 + rand() % 20 / 500.f;
	
	Xboardweight[0][6] = 0.15 + rand() % 20 / 500.f;
	Xboardweight[1][6] = 0.15 + rand() % 20 / 500.f;
	Xboardweight[1][7] = 0.15 + rand() % 20 / 500.f;
	
	Xboardweight[6][0] = 0.15 + rand() % 20 / 500.f;
	Xboardweight[6][1] = 0.15 + rand() % 20 / 500.f;
	Xboardweight[7][1] = 0.15 + rand() % 20 / 500.f;
	
	Xboardweight[6][6] = 0.15 + rand() % 20 / 5000.f;
	Xboardweight[6][7] = 0.15 + rand() % 20 / 500.f;
	Xboardweight[7][6] = 0.15 + rand() % 20 / 500.f;
#endif	
*/
}
void computervscomputergame(int gamesel, int simenabled)
{
   
   int turnselect = 0;
   int x = 0,y = 0;
   int i,j;
   int numpieces = 4;
   
   int cpuO_difficulty = 0, cpuX_difficulty = 0;
       
   
   
   if (simenabled == 0)
   {
	initboardweights();   
	   while (true)
	   { 
		   system("clear");
		   cout << setw(40) << "Select Computer O Difficulty\n";
		   cout << setw(30) << "1. Easy\n";
		   cout << setw(33) << "2. Medium\n\n";   
		   cout << "Selection >";
		   cin >> cpuO_difficulty;
		   if (cpuO_difficulty == 1 || cpuO_difficulty == 2)
			 break;
	   }
	   
	   while (true)
	   { 
		   system("clear");
		   cout << setw(40) << "Select Computer X Difficulty\n";
		   cout << setw(30) << "1. Easy\n";
		   cout << setw(33) << "2. Medium\n\n";   
		   cout << "Selection >";
		   cin >> cpuX_difficulty;
		   if (cpuX_difficulty == 1 || cpuX_difficulty == 2)
			 break;
	   }
   
     displayinitgameboard(gamesel);
   }
   else
   {
	   cpuO_difficulty = cpuOsim_diff;
	   cpuX_difficulty = cpuXsim_diff;
   }
   init();
   if (simenabled == 0)
   {
	   printf("O board weights\n");
	   for (i = 0; i < 8; i++)
	   {
		 for (j = 0; j < 8; j++)
		   {
			 printf("%0.3f \t", Oboardweight[i][j]);
		   }
		   printf("\n");
	   }
	   printf("X board weights\n");
	   for (i = 0; i < 8; i++)
	   {
		 for (j = 0; j < 8; j++)
		   {
			 printf("%0.3f \t", Xboardweight[i][j]);
		   }
		   printf("\n");
	   }
   }
   int maxiteration = 100;
   int iter = 0;
   do 
   {
    iter++;
	pass = false;
	
	if (simenabled == 0)
	{
		if (turnselect == 0)
			cout << "Computer O's turn"<< endl;
		else
			cout << "Computer X's turn"<< endl;	
	}	
	// Computer O turn
	if (turnselect == 0)
	{	
		if (simenabled == 0)
		  sleep(1);
		//int ch;
		//cin >> ch;
		
		if (cpuO_difficulty == 1)		
		computermove_easy(x,y,1);
		else if (cpuO_difficulty == 2)
		computermove_medium(x,y,1);		
		
		if (simenabled == 0 && pass == false)
		printf("Computer O move %d,%d\n",x,y);
	}
	// Computer X turn
	else
	{
		//int ch;
		//cin >> ch;
		if (simenabled == 0)
			sleep(1);		
		
		if (cpuX_difficulty == 1)		
		computermove_easy(x,y,-1);
		else if (cpuX_difficulty == 2)
		computermove_medium(x,y,-1);		
		
		if (simenabled == 0 && pass == false)
		printf("Computer X move %d,%d\n",x,y);
	}	
	
	if (pass == false)
	{		
		boardpositiontaken[x][y] = true;	
		if (turnselect == 0)  
		{
			activepositiontaken[x][y] = 1;			
		}
		else
		{
			activepositiontaken[x][y] = -1;			
		}
	}
	else	
	{
		if (simenabled == 0)
			printf("PASS!!\n");
	}  
	
	Ocount = Xcount = 0;
	for (i = 0; i < BoardSize; i++)
	{
		for (j = 0; j <  BoardSize; j++)
		{
			if (activepositiontaken[i][j] == 1)
			   Ocount++;
			else if(activepositiontaken[i][j] == -1)
			   Xcount++;
		}
	}	
	if (simenabled == 0)
	displayactivegameboard(gamesel);	
	
	if (turnselect == 0) turnselect = 1;
	else				turnselect = 0;

	numpieces++;
	if (Ocount + Xcount == 64)
	{
		gameover = true;
		break;
	}
		
	
	
   } while(gameover == false || iter < maxiteration);
	
	if (simenabled == 1)
		filewrite_displayactivegameboard();
		
	int winner;	
	if (Ocount > Xcount)
	{ 	
		computerOwins++;
		printf("Computer O wins: %d-%d ",Ocount,Xcount);
		winner = 1;
	}	
	else if (Ocount == Xcount)
	{
		printf("Tie Game!\n");
		winner = 0;
	}	
	else
	{
		computerXwins++;
		printf("Computer X wins: %d-%d ",Xcount,Ocount);	
		winner = -1;
	}
	getboardinfo(winner,simenabled);
	
}
void computermove_easy(int &x, int &y, int cpu_piece)
{
	int i,j,k;
	int row,col;
	int turn_piece;
	int capturepiece;
	float cpuboard[BoardSize][BoardSize];
	float score_pertubate;
	int turnpiece_matrix[BoardSize][BoardSize];
	if (cpu_piece == -1)
	  capturepiece = 1;
	else if(cpu_piece == 1)
	  capturepiece = -1;
			

	for (i = 0; i < BoardSize; i++)
	{
		for (j = 0; j < BoardSize; j++)
		{
			if (activepositiontaken[i][j] != 0)
			  turn_piece = 0;
			else
			  turn_piece = board_turnpiece(i,j,capturepiece,cpu_piece,0);
			
			score_pertubate = rand() % 100 / 1000.f;
			
			cpuboard[i][j] = (float)turn_piece + score_pertubate;
			turnpiece_matrix[i][j] = turn_piece;
		}		
	}
	
	float maxweightvalue = 0;
	int maxturnvalue = 0;
	int maxvalue_row, maxvalue_col;
	for (i = 0; i < BoardSize; i++)
	{
		for (j = 0; j < BoardSize; j++)
		{
			if (cpuboard[i][j] > maxweightvalue)
			{
				maxweightvalue = cpuboard[i][j];
				maxvalue_row = i;
				maxvalue_col = j;
			}
			if (turnpiece_matrix[i][j] > maxturnvalue)
			{
				maxturnvalue = turnpiece_matrix[i][j];
			}
		}		
	}
	//printf("maxturnvalue: %d\n",maxturnvalue);
	 if(maxturnvalue == 0)
	 {
		pass = true;		
	 }	
	 else  
	 {
	   x = maxvalue_row;
	   y = maxvalue_col;
	   board_turnpiece(x,y,capturepiece,cpu_piece,1);	
	 }
}
void computermove_medium(int &x, int &y, int cpu_piece)
{
	int i,j,k;
	int row,col;
	int turn_piece;
	int capturepiece;
	float cpuboard[BoardSize][BoardSize];
	float score_pertubate;	
	int turnpiece_matrix[BoardSize][BoardSize];
	
	if (cpu_piece == -1)
	  capturepiece = 1;
	else if(cpu_piece == 1)
	  capturepiece = -1;

	
	
	for (i = 0; i < BoardSize; i++)
	{
		for (j = 0; j < BoardSize; j++)
		{			
			if (activepositiontaken[i][j] != 0)
			{
				  turn_piece = 0;
				  cpuboard[i][j] = 0.f;
			}
			else
			{
				turn_piece = board_turnpiece(i,j,capturepiece,cpu_piece,0);			
				score_pertubate = rand() % 100 / 1000.f;			
				cpuboard[i][j] = (float)turn_piece + score_pertubate;
				
			}
			turnpiece_matrix[i][j] = turn_piece;
		}		
	}
	
	float maxvalue = 0;
	int maxturnvalue = 0;
	int maxvalue_row, maxvalue_col;
	for (i = 0; i < BoardSize; i++)
	{
		for (j = 0; j < BoardSize; j++)
		{
			// O 
			if (cpu_piece == 1)
			cpuboard[i][j] = cpuboard[i][j] * Oboardweight[i][j];
			// X
			else
			cpuboard[i][j] = cpuboard[i][j] * Xboardweight[i][j];
			
			if (cpuboard[i][j] > maxvalue)
			{
				maxvalue = cpuboard[i][j];
				maxvalue_row = i;
				maxvalue_col = j;
			}
			if (turnpiece_matrix[i][j] > maxturnvalue)
			{
				maxturnvalue = turnpiece_matrix[i][j];
			}			
		}	
	}
	// no move available
	if (maxturnvalue == 0)
	{
		pass = true;	
	}
	else
	{
		x = maxvalue_row;
		y = maxvalue_col;
		board_turnpiece(x,y,capturepiece,cpu_piece,1);	
	}
}

int board_turnpiece(int row, int col, int capturepiece, int playerpiece, int flag) 
{
	int i,j;
	int turn_piece = 0;
	
	if (row >= BoardSize || col >= BoardSize || row < 0 || col < 0)
	{
		cout << "Move outside game boundary\n";	
		return 0;
	}
	// move to occupied location
	if (boardpositiontaken[row][col] == true)
	{
		cout << "Board piece (" << row << "," << col << ") is taken\n";	
		return 0;
	} 
  
	nearest_playerpiece_row = nearest_playerpiece_col = -1;
	checkUp(row,col,playerpiece);
//	if (playerpiece == 1 && flag == 1)
//	  cout << "nearrow: " << nearest_playerpiece_row << "\t nearcol: " << nearest_playerpiece_col << endl;
	if (nearest_playerpiece_row != -1)
	{
		i = row; j = col;			
		int curr_turnpiece = turn_piece;
		// from move (x,y) go Up and flip all piece till nearest player piece or till end of boundary
		do
		{
			if(activepositiontaken[--i][j] == capturepiece)
			{
				turn_piece++;
				if (flag == 1)
				activepositiontaken[i][j] = playerpiece;				
			}
			else
			{
				if(activepositiontaken[i][j] == 0)
				{
					turn_piece = curr_turnpiece;
					if (flag == 1)
					{	
						i++;					
						// undo flipping of piece if encounter a empty cell						
						while (i != row)
						{
							if (activepositiontaken[i][j] == playerpiece)
								activepositiontaken[i][j] = capturepiece;
							else if (activepositiontaken[i][j] == 0)
								break;
							i++;
							if (i == BoardSize)
								break;
						}
					}	
				}	
				break;
			}
			if (i < 0)
				break;
		}
		while (i > nearest_playerpiece_row || activepositiontaken[i][j] == capturepiece);
	}
//	if (playerpiece == 1 && flag == 1)
//	  cout << "CheckUp: piece turned" << turn_piece << endl;
	nearest_playerpiece_row = nearest_playerpiece_col = -1;
	checkDown(row,col,playerpiece);
//	if (playerpiece == 1 && flag == 1)
//	cout << "nearrow: " << nearest_playerpiece_row << "\t nearcol: " << nearest_playerpiece_col << endl;
	if (nearest_playerpiece_row != -1)
	{
		i = row; j = col;			
		int curr_turnpiece = turn_piece;
		do
		{
			if(activepositiontaken[++i][j] == capturepiece)
			{
				turn_piece++;
				if (flag == 1)
				activepositiontaken[i][j] = playerpiece;
			}
			else
			{
				if(activepositiontaken[i][j] == 0)
				{
				  turn_piece = curr_turnpiece;
				  if (flag == 1)
					{	
						i--;
						// undo flipping of piece if encounter a empty cell												
						while (i != row)
						{
							if (activepositiontaken[i][j] == playerpiece)
								activepositiontaken[i][j] = capturepiece;
							else if (activepositiontaken[i][j] == 0)
								break;	
							i--;
							if (i < 0)
							  break;
						}
					}
				break;
				}
			}
			if (i >= BoardSize)
				break;
		}
		while (i < nearest_playerpiece_row || activepositiontaken[i][j] == capturepiece);
	}
//	if (playerpiece == 1 && flag == 1)
//	cout << "CheckDown: piece turned" << turn_piece << endl;

	nearest_playerpiece_row = nearest_playerpiece_col = -1;
	checkRight(row,col,playerpiece);
	
//	if (playerpiece == 1 && flag == 1)
//	cout << "nearrow: " << nearest_playerpiece_row << "\t nearcol: " << nearest_playerpiece_col << endl;
	if (nearest_playerpiece_row != -1)
	{
		i = row; j = col;
		int curr_turnpiece = turn_piece;
		do
		{
			if(activepositiontaken[i][++j] == capturepiece)
			{
				turn_piece++;
				if (flag == 1)
				activepositiontaken[i][j] = playerpiece;
			}
			else
			{
				if(activepositiontaken[i][j] == 0)
				{
				  turn_piece = curr_turnpiece;
				  if (flag == 1)
					{		
						j--;
						// undo flipping of piece if encounter a empty cell						
						while (j != col)
						{
							if (activepositiontaken[i][j] == playerpiece)
								activepositiontaken[i][j] = capturepiece;
							else if (activepositiontaken[i][j] == 0)
								break;	
							j--;
							if (j < 0)
							  break;
						}
					}
				}	
				break;
			}
			if (j >= BoardSize)
				break;
		}
		while (j < nearest_playerpiece_col || activepositiontaken[i][j] == capturepiece);
	}
//	if (playerpiece == 1 && flag == 1)
//	cout << "CheckRight: piece turned" << turn_piece << endl;

	nearest_playerpiece_row = nearest_playerpiece_col = -1;
	checkLeft(row,col,playerpiece);
//	if (playerpiece == 1 && flag == 1)
//	  cout << "nearrow: " << nearest_playerpiece_row << "\t nearcol: " << nearest_playerpiece_col << endl;
	if (nearest_playerpiece_row != -1)
	{
		i = row; j = col;
		int curr_turnpiece = turn_piece;
		do
		{
			if(activepositiontaken[i][--j] == capturepiece)
			{				
				turn_piece++;
				if (flag == 1)
				activepositiontaken[i][j] = playerpiece;
				//cout << "(" << i << "," << j << "):" << activepositiontaken[i][j] << endl;
			}			
			else
			{
				if(activepositiontaken[i][j] == 0)
				{
					turn_piece = curr_turnpiece;
					if (flag == 1)
					{	
						j++;
						// undo flipping of piece if encounter a empty cell					
						while (j != col)
						{
							if (activepositiontaken[i][j] == playerpiece)
								activepositiontaken[i][j] = capturepiece;
							else if (activepositiontaken[i][j] == 0)
								break;	
							j++;
							if (j == BoardSize)
							  break;
						}
					}
				}
				break;
			}	
			if (j < 0)
				break;
		}
		while (j > nearest_playerpiece_col || activepositiontaken[i][j] != capturepiece);
	}

//	if (playerpiece == 1 && flag == 1)
//	cout << "CheckLeft: piece turned: " << turn_piece << endl;
	nearest_playerpiece_row = nearest_playerpiece_col = -1;
	checknegativediagUp(row,col,playerpiece);
	
//	if (playerpiece == 1 && flag == 1)
//	cout << "nearrow: " << nearest_playerpiece_row << "\t nearcol: " << nearest_playerpiece_col << endl;
	if (nearest_playerpiece_row != -1)
	{
		i = row; j = col;
		int curr_turnpiece = turn_piece;
		do
		{
			if(activepositiontaken[--i][--j] == capturepiece)
			{
				turn_piece++;
				
				if (flag == 1)
				activepositiontaken[i][j] = playerpiece;
			}
			else
			{
				if(activepositiontaken[i][j] == 0)
				{
					turn_piece = curr_turnpiece;
					if (flag == 1)
					{	
						i++; j++;
						while (i != row || j != col)
						{
							if (activepositiontaken[i][j] == playerpiece)
							  activepositiontaken[i][j] = capturepiece;							
							else if (activepositiontaken[i][j] == 0)
								break; 
							i++; j++;
							if (i == BoardSize || j == BoardSize)
								break;
						}
					}
				}	  
				break;
			}	
			if (i == 0 || j == 0)
				break;
		}	
		while (((i > nearest_playerpiece_row) && (j > nearest_playerpiece_col)) || activepositiontaken[i][j] == capturepiece);
	}
//	if (playerpiece == 1 && flag == 1)
//	cout << "CheckNegDiagUp: piece turned" << turn_piece << endl;
	nearest_playerpiece_row = nearest_playerpiece_col = -1;
	checknegativediagDown(row,col,playerpiece);
//	if (playerpiece == 1 && flag == 1)
//	cout << "nearrow: " << nearest_playerpiece_row << "\t nearcol: " << nearest_playerpiece_col << endl;
	if (nearest_playerpiece_row != -1)
	{
		i = row; j = col;
		int curr_turnpiece = turn_piece;
		do
		{
			if(activepositiontaken[++i][++j] == capturepiece)
			{
				turn_piece++;
				
				if (flag == 1)
				activepositiontaken[i][j] = playerpiece;
			}
			else
			{
				if(activepositiontaken[i][j] == 0)
				{
					turn_piece = curr_turnpiece;
					if (flag == 1)
					{
						i--; j--;
						while (i != row || j != col)
						{
							if (activepositiontaken[i][j] == playerpiece)
							  activepositiontaken[i][j] = capturepiece;							
							else if (activepositiontaken[i][j] == 0)
								break;  
							  i--; j--;
							  if (i < 0 || j < 0)
							    break;
						}
					}	
				}
				break;
			}	
			if ( i == BoardSize-1 || j == BoardSize-1)
				break;
		}
		while (((i < nearest_playerpiece_row) && (j < nearest_playerpiece_col)) || activepositiontaken[i][j] == capturepiece);
	}
//	if (playerpiece == 1 && flag == 1)
//	cout << "CheckNegDiagDown: piece turned" << turn_piece << endl;
	nearest_playerpiece_row = nearest_playerpiece_col = -1;
	checkpositivediagDown(row,col,playerpiece);
//	if (playerpiece == 1 && flag == 1)
//	cout << "nearrow: " << nearest_playerpiece_row << "\t nearcol: " << nearest_playerpiece_col << endl;
	if (nearest_playerpiece_row != -1)

	{
		i = row; j = col;
		int curr_turnpiece = turn_piece;
		do
		{
			if(activepositiontaken[++i][--j] == capturepiece)
			{
				turn_piece++;
				
				if (flag == 1)
				activepositiontaken[i][j] = playerpiece;
			}
			else
			{
				if(activepositiontaken[i][j] == 0)
				{
					 turn_piece = curr_turnpiece;
					 if (flag == 1)
					 {
						i--; j++;
						 while (i != row || j != col)
						{					
							if (activepositiontaken[i][j] == playerpiece)
								activepositiontaken[i][j] = capturepiece;							
							else if (activepositiontaken[i][j] == 0)
								break;	
							i--; j++;
							if (i < 0 || j == BoardSize)
							  break;
						}
					 }	
				} 
				break;
			}	
		}
		while (((i < nearest_playerpiece_row) && (j > nearest_playerpiece_col)) || activepositiontaken[i][j] == capturepiece);
	}
//	if (playerpiece == 1 && flag == 1)
//	cout << "CheckPosDiagDown: piece turned" << turn_piece << endl;
	nearest_playerpiece_row = nearest_playerpiece_col = -1;
	checkpositivediagUp(row,col,playerpiece);
//	if (playerpiece == 1 && flag == 1)
//	cout << "nearrow: " << nearest_playerpiece_row << "\t nearcol: " << nearest_playerpiece_col << endl;
	if (nearest_playerpiece_row != -1)
	{
		i = row; j = col;
		int curr_turnpiece = turn_piece;
		do
		{
			if(activepositiontaken[--i][++j] == capturepiece)
			{
				turn_piece++;
				
				if (flag == 1)
				activepositiontaken[i][j] = playerpiece;
			}
			else
			{
				if(activepositiontaken[i][j] == 0)
				{
				  turn_piece = curr_turnpiece;
				  if (flag == 1)
				  {
					  i++; j--;
					  while (i != row || j != col)
					  {
						if (activepositiontaken[i][j] == playerpiece)
							activepositiontaken[i][j] = capturepiece;							
						else if (activepositiontaken[i][j] == 0)
								break;	
						i++; j--;
						if (i == BoardSize || j < 0)
						  break;
					  }
				  }
				}
				break;
			}	
		}
		while (((i > nearest_playerpiece_row) && (j < nearest_playerpiece_col)) || activepositiontaken[i][j] == capturepiece);
	}
	//if (flag == 1)
	//  printf("Total Pieces Switched: %d\n",turn_piece);
	
	return turn_piece;
}
void displayinitgameboard(int sel)
{
	int i;
	
	
	if (sel == 1)
	cout << "\t Score Count: Player O = " << Ocount << "\tComputer X = " << Xcount << "\n\n";		
	else if (sel == 2)
	cout << "\t Score Count: Player O = " << Ocount << "\tPlayer X = " << Xcount << "\n\n";	
	else if (sel == 3)
	cout << "\t Score Count: Computer O = " << Ocount << "\tComputer X = " << Xcount << "\n\n";	
		
	cout  << "        -------------------------------------"<<endl; 
	cout  << "        |   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |"<< endl; 
     for (i = 	0 ; i < BoardSize; i++)
	{
	cout << setw(9);
	cout << "        ------------------------------------\n";
	
	
	if (i+1 == 4)
	{
		cout << setw(9) <<  "|" << setw(2) << i << setw(2) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << setw(2); 				
		printf("\033[0;31m O\033[0m");
		cout << setw(2) << "|" << setw(2);
		printf("\033[0;34m X\033[0m"); 
		cout <<  setw(2) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << endl;
	}
	else if (i+1 == 5)
	{
		cout << setw(9) <<  "|" << setw(2) << i << setw(2) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << setw(2);
		printf("\033[0;34m X\033[0m");
		cout << setw(2) << "|" << setw(2);
		printf("\033[0;31m O\033[0m"); 
		cout <<  setw(2) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << endl;		
	}
	else
	cout << setw(9) <<  "|" << setw(2) << i << setw(2) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << setw(4) << "|" << endl;	

}
	cout  << "        -------------------------------------"<<endl; 
}
void init()
{
	int i,j;
	for (i = 0; i < BoardSize; i++)
	{
		for (j = 0; j < BoardSize; j++)
		{
			boardpositiontaken[i][j] = false;
			activepositiontaken[i][j] = 0;
		}
	}
	boardpositiontaken[3][3] = boardpositiontaken[3][4] = boardpositiontaken[4][3] = boardpositiontaken[4][4] = true;
	activepositiontaken[3][3] = activepositiontaken[4][4] = 1;
	activepositiontaken[3][4] = activepositiontaken[4][3] = -1;	
	
}
void checkUp(int row, int col, int playerpiece)
{
	int i;
	
	for (i = row; i >= 0; i--)
	{
		if (activepositiontaken[i][col] == playerpiece)
		{
			nearest_playerpiece_row = i;
			nearest_playerpiece_col = col;				
		}
	}
	
} 
void checkDown(int row, int col, int playerpiece)
{
	int i;

	for (i = row; i < BoardSize; i++)
	{
		if (activepositiontaken[i][col] == playerpiece)
		{
			nearest_playerpiece_row = i;
			nearest_playerpiece_col = col;
			break;
		}
	}

} 
void checkRight(int row, int col, int playerpiece)
{
	int i;	
	for (i = col; i < BoardSize; i++)
	{
		if (activepositiontaken[row][i] == playerpiece)
		{
			nearest_playerpiece_row = row;
			nearest_playerpiece_col = i;
			break;
		}
	}

} 
void checkLeft(int row, int col, int playerpiece)
{
	int i;	
	for (i = col; i >= 0; i--)
	{
		if (activepositiontaken[row][i] == playerpiece)
		{
			nearest_playerpiece_row = row;
			nearest_playerpiece_col = i;
			break;
		}
	}

} 
void checkpositivediagUp(int row,int col, int playerpiece)
{
	int i,j;	
	j = col;
	for (i = row; i >= 0; i--)
	{
		if (activepositiontaken[i][j] == playerpiece)
		{
			nearest_playerpiece_row = i;
			nearest_playerpiece_col = j;
			break;
		}	
		j++;
		if (j == BoardSize)
			break;
	}
	
}
void checkpositivediagDown(int row,int col, int playerpiece)
{
	int i,j;	
	j = col;
	for (i = row; i < BoardSize; i++)
	{
		if (activepositiontaken[i][j] == playerpiece)
		{
			nearest_playerpiece_row = i;
			nearest_playerpiece_col = j;
			break;
		}	
		j--;
		if (j < 0)
			break;
	}
	
}
void checknegativediagDown(int row,int col, int playerpiece)
{
	int i,j;	
	j = col;		
	for (i = row; i < BoardSize; i++)
	{
		if (activepositiontaken[i][j] == playerpiece)
		{
			nearest_playerpiece_row = i;
			nearest_playerpiece_col = j;
			break;
		}
		j++;
		if (j == BoardSize)
			break;
	}
	
}
void checknegativediagUp(int row,int col, int playerpiece)
{
	int i,j;	
	j = col;		
	for (i = row; i >= 0; i--)
	{
		if (activepositiontaken[i][j] == playerpiece)
		{
			nearest_playerpiece_row = i;
			nearest_playerpiece_col = j;
			break;
		}
		j--;
		if (j < 0)
			break;
	}
	
}
void getboardinfo(int winner, int simflag)
{
  int i;
  int Oedgecnt = 0,Xedgecnt = 0;
  int Ocornercnt = 0, Xcornercnt = 0;
  for (i = 0; i < BoardSize; i++)
  {
    if (activepositiontaken[i][0] == 1)
	  Oedgecnt++;
	else
	 Xedgecnt++;		 
	 
	if (activepositiontaken[i][7] == 1)
		Oedgecnt++;
	else
		Xedgecnt++;
  }
  for (i = 1; i < BoardSize-1;i++)
  {
    if (activepositiontaken[0][i] == 1)
	  Oedgecnt++;
	else 
	  Xedgecnt++;
	if (activepositiontaken[7][i] == 1)
		Oedgecnt++;
	else
		Xedgecnt++;
  }
  if (activepositiontaken[0][0] == 1)
    Ocornercnt++;
  else
	Xcornercnt++;
	
	if (activepositiontaken[0][7] == 1)
    Ocornercnt++;
  else
	Xcornercnt++;
	
	if (activepositiontaken[7][0] == 1)
    Ocornercnt++;
  else
	Xcornercnt++;
	
	if (activepositiontaken[7][7] == 1)
    Ocornercnt++;
  else
	Xcornercnt++;
	
   printf("\t O edge capture: %d/28, X edge capture: %d/28 \t O corner capture: %d/4 X corner capture %d/4\n",Oedgecnt,Xedgecnt,Ocornercnt,Xcornercnt);	
   
   if (simflag == 1)
   {
   fprintf(fp,"\t O edge capture: %d/28, X edge capture: %d/28 \t O corner capture: %d/4 X corner capture %d/4\n",Oedgecnt,Xedgecnt,Ocornercnt,Xcornercnt);	
		if(winner == 1)
		{
		  if (Oedgecnt >= 21)
		  {  
			_21edgecnt++;
		  }
		  if (Oedgecnt >= 14)
		  {
			_14edgecnt++;			
		  }
		  if (Oedgecnt >= 7)
		  {
			_7edgecnt++;
		  }
		  if (Ocornercnt == 1)
		  {
			_1corner_O++;	
			_1cornergame++;
			_3cornergame++;
		  }	
		  else if(Ocornercnt == 2)
		  {
			_2corner_O++;
			_2cornergame++;
		  }
		  else if(Ocornercnt == 3)
		  {
			_3corner_O++;
			_3cornergame++;
			_1cornergame++;
		  }
		  else if(Ocornercnt == 4)
		  {
		     _4corner_O++;
			 _4cornergame++;
		  }	 
		}
		else
		{
		  if (Xedgecnt >= 21)
		  {  
			_21edgecnt++;
		  }
		  if (Xedgecnt >= 14)
		  {
			_14edgecnt++;			
		  }
		  if (Xedgecnt >= 7)
		  {
			_7edgecnt++;
		  }
		  if (Xcornercnt == 1)
		  {
			_1corner_X++;
			_1cornergame++;
			_3cornergame++;
		  }	
		  else if(Xcornercnt == 2)
		  {
			_2corner_X++;
			_2cornergame++;
		  }	
		  else if(Xcornercnt == 3)
		  {
			_3corner_X++;
			_3cornergame++;
			_1cornergame++;
		  }	
		  else if(Xcornercnt == 4)
		  {
			_4corner_X++;		  
			_4cornergame++;
		  }	
		}
	}
   
}
void displayactivegameboard(int sel)
{
	int i,j;
	
	if (sel == 1)
	cout << "\t Score Count: Player O = " << Ocount << "\tComputer X = " << Xcount << "\n\n";	
	else if (sel == 2)
	cout << "\t Score Count: Player O = " << Ocount << "\tPlayer X = " << Xcount << "\n\n";		
	else if (sel == 3)
	cout << "\t Score Count: Computer O = " << Ocount << "\tComputer X = " << Xcount << "\n\n";	
	
	cout  << "        -------------------------------------"<<endl; 
	cout  << "        |   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |"<< endl; 
	cout  << "        -------------------------------------"<<endl; 
	for (i = 0; i < BoardSize; i++)
	{
		cout << setw(9) << "|" << setw(2) << i << setw(2) << "|";
		for (j = 0; j < BoardSize; j++)
		{
			if (activepositiontaken[i][j] == -1)
			{
				cout << setw(2);
				printf("\033[0;34m X\033[0m");
				cout << setw(2) << "|";
			}
			else if(activepositiontaken[i][j] == 0)
				cout << setw(2) << " " << setw(2) << "|";
			else if(activepositiontaken[i][j] == 1)
			{
				cout << setw(2);
				printf("\033[0;31m O\033[0m");
				cout << setw(2) << "|";			 
			}
		}
		cout << endl;
		cout  << "        -------------------------------------"<<endl;
	}

}
void filewrite_displayactivegameboard()
{
	int i,j;
	
	fprintf(fp_game,"Score Count: Computer O = %d \t Computer X = %d \n\n",Ocount, Xcount);	
		
	fprintf(fp_game,"        -------------------------------------\n");
	fprintf(fp_game,"        |   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");	 
	fprintf(fp_game,"        -------------------------------------\n");
	
	for (i = 0; i < BoardSize; i++)
	{
		fprintf(fp_game,"        | %d |",i);		
		for (j = 0; j < BoardSize; j++)
		{
			if (activepositiontaken[i][j] == -1)
			{				
				fprintf(fp_game," X |");														
			}
			else if(activepositiontaken[i][j] == 0)
			{
				fprintf(fp_game,"    |");				
			}		
			else if(activepositiontaken[i][j] == 1)
			{				
				fprintf(fp_game," O |");						
			}
		}
		fprintf(fp_game,"\n");
		fprintf(fp_game,"        -------------------------------------\n");	
	}
}

void menu()
{
	cout << setw(80) << "--------------------------------------\n";
	cout << setw(80) << "|            Reversi                 |\n";
	cout << setw(80) << "|     1. 1 Player                    |\n";
	cout << setw(80) << "|     2. 2 Players                   |\n";
	cout << setw(80) << "|     3. Computer vs Computer        |\n";
	cout << setw(80) << "|     4. Simulation                  |\n";
	cout << setw(80) << "|     5. Exit                        |\n";
	cout << setw(80) << "--------------------------------------\n";
}
