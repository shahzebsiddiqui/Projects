#include <ctype.h>
	using namespace std;

#include "neural_net.h"

static int iteration = 100;

int main()
{
	bool trainingXs = true;
	bool validinput;
	int input;
	neural_network neural_net;

	do
	{
		cout<<"Welcome to g24's Neural Network Training Program!"<<endl;
	    cout<<"Press '1' to train the neural network as Player X.\nPress '0'  to train the neural network as Player O.\n"<<endl;
		cin >> input;
		
		if (input == 1)
		{
			validinput = true;
			trainingXs = true;
		}
		else if (input == 0)
		{
			validinput = true;
			trainingXs = false;
		}
		else
			validinput = false;

	}	while (validinput == false);

	for (int i = 0; i < iteration; i++)
	{
		FILE * read_in = fopen("g24.dat1", "r");
		neural_net.InitNetwork(&read_in);
		neural_net.InitScore();

		ifstream beforeCorrectPats("before_correct_patterns.txt");
		ifstream correctPatsO("correct_patternsO.txt");
		ifstream correctPatsX("correct_patternsX.txt");

		ifstream oFirstMoveBefore("opening_moves_O_before.txt");
		ifstream oFirstMoveDesired("opening_moves_O_desired.txt");

		ifstream oOffenseBefore("OffensiveO.txt");
		ifstream oOffenseDesired("OffensiveODesired.txt");
		ifstream oDefenseBefore("DefensiveO.txt");
		ifstream oDefenseDesired("DefensiveODesired.txt");

		ifstream xOffenseBefore("OffensiveX.txt");
		ifstream xOffenseDesired("OffensiveXDesired.txt");
		ifstream xDefenseBefore("DefensiveX.txt");
		ifstream xDefenseDesired("DefensiveXDesired.txt");

		if(trainingXs)
		{
			cout<<"So you want to training as 'X', eh?"<<endl;
			for(int i=0;i<3;i++)
			{
				switch(i)
				{
					case 0:
					{
						neural_net.SelectPlayer(beforeCorrectPats, correctPatsX);
						break;
					}
					case 1:
					{
						neural_net.SelectPlayer(xOffenseBefore, xOffenseDesired);
						break;
					}
					case 2:
					{
						neural_net.SelectPlayer(xDefenseBefore, xDefenseDesired);
						break;
					}
				}
				neural_net.GetInput();
			}
		}
		else
		{
			cout<<"So you want to training as 'O', eh?"<<endl;
			for(int i=0;i<4;i++)
			{
				switch(i)
				{
					case 0:
					{
						neural_net.SelectPlayer(beforeCorrectPats, correctPatsO);
						break;
					}
					case 1:
					{
						neural_net.SelectPlayer(oFirstMoveBefore, oFirstMoveDesired);
						break;
					}
					case 2:
					{
						neural_net.SelectPlayer(oOffenseBefore, oOffenseDesired);
						break;
					}
					case 3:
					{
						neural_net.SelectPlayer(oDefenseBefore, oDefenseDesired);
						break;
					}
				}
				neural_net.GetInput();
			}
		}
		cout << "Training set  " << i << " Complete!\n";
	}
	return 0;
}