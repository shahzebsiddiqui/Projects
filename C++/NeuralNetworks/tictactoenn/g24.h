#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h>
#include <fstream>

using namespace std;

const int NUMLINKS_IN_LAYER = 625;
const int NUMLAYERS = 3;
const int NUM_INPUT_NEURONS = 25;
const int NUM_HIDDEN_NEURONS = 25;
const int NUM_OUTPUT_NEURONS = 25;
const int NUMNODES = NUM_INPUT_NEURONS + NUM_HIDDEN_NEURONS + NUM_OUTPUT_NEURONS;
const int TOTAL_GAME_BOARDS = 15;
const int size = 10000;
int PATTERN_LENGTH;
#define DIMENSION 5

class neural_network 
{
	private:
		double learningrate;	
		double inputToHiddenWeights[NUM_INPUT_NEURONS][NUM_HIDDEN_NEURONS]; 
		double hiddenToOutputWeights[NUM_HIDDEN_NEURONS][NUM_OUTPUT_NEURONS]; 
		double wts[2*NUMLINKS_IN_LAYER];

		double inputThreshold[NUM_INPUT_NEURONS];
		double hiddenThreshold[NUM_HIDDEN_NEURONS];
		double outputThreshold[NUM_OUTPUT_NEURONS];
		
		double inputAccHidden[NUM_HIDDEN_NEURONS];
		double inputAccOutput[NUM_OUTPUT_NEURONS];		
		int DesiredValue[NUM_OUTPUT_NEURONS];

		int inputLayer[NUM_INPUT_NEURONS];
		
		int boardEncoding[DIMENSION][DIMENSION][DIMENSION];		
		int boardEncodingInput[DIMENSION][DIMENSION];
		int boardScore[DIMENSION][DIMENSION][DIMENSION];
		int PATTERN[DIMENSION][DIMENSION];
		int WINPATTERN[size];
		int WINPATTERN2D[121][DIMENSION][DIMENSION];			
		int BEFOREPATTERN[size];
		int BEFOREPATTERN2D[121][DIMENSION][DIMENSION];
		int optimalPlane;

		bool openPosition[DIMENSION][DIMENSION][DIMENSION];
	public:	
		neural_network()
		{
			SetLearningRate(0.6);
		}

		neural_network(double learningRate)	
		{			
			SetLearningRate(learningRate);
		}

		void SetLearningRate(double learningRate)
		{
			learningrate = learningRate;
		}

		void InitScore()
		{
			for (int i = 0; i < DIMENSION; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{
					for (int k = 0; k < DIMENSION; k++)
					{						
						boardScore[i][j][k] = 0;						
					}
				}
			}
		}

		void SetWeights()
		{
			int row = 0, col = 0;
			for (int i = 0; i < NUMLINKS_IN_LAYER;i++)
			{
				inputToHiddenWeights[row][col] = wts[i];				
				if (i % 25 == 0)
				{
					row++;
					col = 0;
				}
				col++;
			}			
			
			row = col = 0;
			
			for (int i = NUMLINKS_IN_LAYER; i < 2*NUMLINKS_IN_LAYER;i++)
			{
				hiddenToOutputWeights[row][col] = wts[i];				
				if (i % 25 == 0)
				{
					row++;
					col = 0;
				}
				col++;
			}

			return;
		}

		void InitNetwork(FILE ** fptr)
		{
			ReadWeights(fptr);
			SetWeights();
			SetThresholds();
		}

		void SetThresholds()
		{
			for (int i = 0; i < NUM_INPUT_NEURONS;i++)
			{				  
				inputThreshold[i] = RandomizeThreshold();
			}
			for (int i = 0; i < NUM_HIDDEN_NEURONS;i++)
			{				  
				hiddenThreshold[i] = RandomizeThreshold();
			}
			for (int i = 0; i < NUM_OUTPUT_NEURONS;i++)
			{				  
				outputThreshold[i] = RandomizeThreshold();
			}
		}

		void ReadWeights(FILE ** fptr)
		{
			int linecnt = 0;
			char line [100];

			if ((*fptr) == NULL) 
					printf("Error opening file\n"); 
			else
			{ 
				while (fgets(line, 100, (*fptr))!= NULL)
				{
					string value(line);
					wts[linecnt] = atof(value.c_str());
					linecnt++;
				}
				fclose (*fptr); 
			} 
		}

		void UpdateWeights()
		{
			ofstream fout;
			fout.open("g24.dat1");
			for (int i = 0; i < NUM_INPUT_NEURONS; i++)
			{
				for (int j = 0; j < NUM_HIDDEN_NEURONS; j++)
				{
					fout << inputToHiddenWeights[i][j] << endl;
				}
			}

			for (int i = 0; i < NUM_HIDDEN_NEURONS; i++)
			{
				for (int j = 0; j < NUM_OUTPUT_NEURONS; j++)
				{
					fout << hiddenToOutputWeights[i][j] << endl;
				}
			}
			fout.close();
		}

		/*void ReadCorrectPatterns(ifstream & read)
		{
			int count = 0;
			int index = 0;
			if (!read)
			{
				cout<<"Error in ReadCorrectPatterns: File Not Found!"<<endl;
				return;
			}
			else
			{				
				string line = "";
				while (!read.eof())
				{	
					count++;
					getline(read,line,'\n');
					for(unsigned int i=0; i < line.length();i++)
					{
						if(line.at(i) != ',')
						{
							if(line.at(i) == '-')
							{
								WINPATTERN[index] = -1;
								i++;
							}
							else
							{					
									if(line.at(i) == '0')
									WINPATTERN[index] = 0;
									else if(line.at(i) == '1')
									WINPATTERN[index] = 1;					
							}
							index++;
						}
					}
				}
			}			
			ConvertPatternForm(index);
		  }*/

		void ConvertPatternForm(int count)
		{
			int CT25 = 0, row = 0, col = 0;
			
			for (int i = 0; i < count; i++)
			{								
				if (i % 5 == 0)
				{
					row++;
					col = 0;
					if (i % 25 == 0)
					{
						CT25++;
						row = 0;
						col = 0;											
					}
				}
				WINPATTERN2D[CT25][row][col] = WINPATTERN[i];	
				//cout << WINPATTERN2D[CT25][row][col] << endl;
				col++;
			}
			//cin.get();			
		 }

		void ReadBoard(char gameboard[DIMENSION][DIMENSION][DIMENSION])
		{		
			for (int z = 0; z < DIMENSION; z++)
			{
				for (int y = 0; y < DIMENSION; y++)
				{
					for (int x = 0; x <= DIMENSION-1; x++)
					{
						if(gameboard[z][y][x] == 'X')
						{
							openPosition[z][y][x] = false;
							boardEncoding[z][y][x] = 1;							
						}
						else if(gameboard[z][y][x] == 'O')
						{
							openPosition[z][y][x] = false;
							boardEncoding[z][y][x] = -1;							
						}
						else if(gameboard[z][y][x] == ' ')
						{
							openPosition[z][y][x] = true;
							boardEncoding[z][y][x] = 0;							
						}
						else
						{
							cout<<"Error in ReadBoard: Invalid Board Character"<<endl;
							return;
						}
					}
				}
			}
			return;
		}
		
		void FindOptimalSlice()
		{
			for (int i  = 0; i < DIMENSION; i++)
			{
				ZPlane(i);
				YPlane(i);
				XPlane(i);
		    }

			ComputeSpecialScore();
			FeedSlice();
		}

		void FeedSlice()
		{
			int sliceScore[DIMENSION] = {0, 0, 0, 0, 0};
			int count, topScore;
			optimalPlane = topScore = count = 0;

			for (int z = 0; z < DIMENSION; z++)
			{
				for (int y = 0; y < DIMENSION; y++)
				{
					for (int x = 0; x <= DIMENSION-1; x++)
					{
						sliceScore[z] += boardScore[z][y][x];
					}
				}
			}

			for(int i=0;i<DIMENSION;i++)
			{
				if(topScore < sliceScore[i])
				{
					topScore = sliceScore[i];
					optimalPlane = i;
				}
			}
		}

		void ZPlane(int index)
		{			
			for (int y = 0; y < DIMENSION; y++)
			{
				for (int x = 0; x < DIMENSION; x++)
				{
					if ((y == 0  && x == 0) || (y == 1 && x == 1) || (y == 3 && x == 3))					
						ComputeScore(index,y,x,"TopLeft", "Z");	

					else if ((y == 0 && x == 4) || (y == 1 && x == 3) || (y == 3 && x == 1))
						ComputeScore(index,y,x,"TopRight","Z");

					else if (y == 4 && x == 0 || (y == 1 && x == 3) || (y == 3 && x == 1))
						ComputeScore(index,y,x,"BottomLeft","Z");

					else if ((y == 4 && x == 4)|| (y == 1 && x == 1) || (y == 3 && x == 3))
						ComputeScore(index,y,x,"BottomRight","Z");

					else if (y == 2 && x == 2)
						ComputeScore(index,y,x,"Center","Z");

					else
						ComputeScore(index,y,x,"","Z");	
				}
			}			
		}
		
		void YPlane(int index)
		{			
			for (int z = 0; z < DIMENSION; z++)
			{
				for (int x = 0; x < DIMENSION; x++)
				{
					if ((z == 0  && x == 0) || (z == 1 && x == 1) || (z == 3 && x == 3))					
						ComputeScore(z,index,x,"TopLeft","Y");	

					else if ((z == 0 && x == 4) || (z == 1 && x == 3) || (z == 3 && x == 1))
						ComputeScore(z,index,x,"TopRight","Y");

					else if ((z == 4 && x == 0) || (z == 1 && x == 3) || (z == 3 && x == 1))
						ComputeScore(z,index,x,"BottomLeft","Y");

					else if ((z == 4 && x == 4) || (z == 1 && x == 1) || (z == 3 && x == 3))
						ComputeScore(z,index,x,"BottomRight","Y");

					else if (x == 2 && x == 2)
						ComputeScore(z,index,x,"Center","Y");

					else
						ComputeScore(z,index,x,"","Y");	
				}
			}			
		}
		
		void XPlane(int index)
		{			
			for (int z = 0; z < DIMENSION; z++)
			{
				for (int y = 0; y < DIMENSION; y++)
				{					
					if ((z == 0  && y == 0) || (z == 1 && y == 1) || (z == 3 && y == 3))					
						ComputeScore(z,y,index,"TopLeft","X");	

					else if ((z == 0 && y == 4) || (z == 1 && y == 3) || (z == 3 && y == 1))
						ComputeScore(z,y,index,"TopRight","X");

					else if ((z == 4 && y == 0) || (z == 1 && y == 3) || (z == 3 && y == 1))
						ComputeScore(z,y,index,"BottomLeft","X");

					else if ((z == 4 && y == 4) || (z == 1 && y == 1) || (z == 3 && y == 3))
						ComputeScore(z,y,index,"BottomRight","X");

					else if (z == 2 && y == 2)
						ComputeScore(z,y,index,"Center","X");

					else
						ComputeScore(z,y,index,"","X");										
				}
			}			
		}
		
		void ComputeScore(int z, int y, int x, string Type, string PlaneSlice)
		{
			int sumDiagTopLeft = 0,sumDiagTopRight = 0, sumVert = 0, sumHorz = 0;
			
			if (Type == "TopLeft" || Type == "Center" || Type == "BottomRight" )
			{
				if (PlaneSlice == "Z")
				{
					// diagonal sum
					for (int i = 0; i < DIMENSION; i++)
					{
						for (int j = 0; j < DIMENSION; j++)
						{
							if (i == j)
							{
								sumDiagTopLeft+= boardEncoding[z][i][j];	
							}
						}
					}
				}
				else if (PlaneSlice == "Y")
				{
					// diagonal sum
					for (int i = 0; i < DIMENSION; i++)
					{
						for (int j = 0; j < DIMENSION; j++)
						{
							if (i == j)
							{
								sumDiagTopLeft+= boardEncoding[i][y][j];	
							}
						}
					}
				}
				else if (PlaneSlice == "X")
				{
					// diagonal sum
					for (int i = 0; i < DIMENSION; i++)
					{
						for (int j = 0; j < DIMENSION; j++)
						{
							if (i == j)
							{
								sumDiagTopLeft+= boardEncoding[i][j][x];	
							}
						}
					}
				}
			}
			else if (Type == "TopRight" || Type == "Center" || Type == "BottomLeft")
			{
				if (PlaneSlice == "Z")
				{
					// diagonal sum
					for (int i = DIMENSION; i > 0; i--)
					{
						for (int j = 0; j < DIMENSION; j++)
						{
							if ((i + j) == 4)
								sumDiagTopRight+= boardEncoding[z][i][j];
						}
					}
				}
				else if (PlaneSlice == "Y")
				{
					// diagonal sum
					for (int i = DIMENSION; i > 0; i--)
					{
						for (int j = 0; j < DIMENSION; j++)
						{
							if ((i + j) == 4)
								sumDiagTopRight+= boardEncoding[i][y][j];
						}
					}
				}
				else if (PlaneSlice == "X")
				{
					// diagonal sum
					for (int i = DIMENSION; i > 0; i--)
					{
						for (int j = 0; j < DIMENSION; j++)
						{
							if ((i + j) == 4)
								sumDiagTopRight+= boardEncoding[i][j][x];
						}
					}
				}
			}
			if (PlaneSlice == "Z")
			{
				// vertical sum
				for (int i = 0; i < DIMENSION; i++)
				{				
					sumVert+= boardEncoding[z][i][x];
				}
				
				// horizontal sum
				for (int i = 0; i < DIMENSION; i++)
				{
					sumHorz+= boardEncoding[z][y][i];
				}	
			}
			else if (PlaneSlice == "Y")
			{
				// vertical sum
				for (int i = 0; i < DIMENSION; i++)
				{									
					sumVert+= boardEncoding[z][y][i];					
				}
				
				// horizontal sum
				for (int i = 0; i < DIMENSION; i++)
				{
					sumHorz+= boardEncoding[i][y][x];
				}	
			}
			else if (PlaneSlice == "X")
			{
				// vertical sum
				for (int i = 0; i < DIMENSION; i++)
				{				
					sumVert+= boardEncoding[i][y][x];
				}
				
				// horizontal sum
				for (int i = 0; i < DIMENSION; i++)
				{
					sumHorz+= boardEncoding[z][i][x];
				}	
			}						
			
			if (sumDiagTopLeft > sumVert && sumDiagTopLeft > sumHorz && sumDiagTopLeft > sumDiagTopRight)
			{
				if (abs(boardScore[z][y][x]) < abs(sumDiagTopLeft))
					boardScore[z][y][x] = sumDiagTopLeft;
			}
			else if (sumVert > sumDiagTopLeft && sumVert > sumHorz && sumVert > sumDiagTopRight)
			{
				if (abs(boardScore[z][y][x]) < abs(sumVert))
				{
					boardScore[z][y][x] = sumVert;				
				}
			}
			else if (sumHorz > sumDiagTopLeft && sumHorz > sumVert && sumHorz > sumDiagTopRight)
			{
				if (abs(boardScore[z][y][x]) < abs(sumHorz))
					boardScore[z][y][x] = sumHorz;
			}
			else
			{
				if (abs(boardScore[z][y][x]) < abs(sumDiagTopRight))
					boardScore[z][y][x] = sumDiagTopRight;
			}
		}
		
		void ComputeSpecialScore()
		{
			int sumTopLeftCorner = 0, sumTopRightCorner = 0,sumBottomLeftCorner = 0,sumBottomRightCorner = 0;
			
			sumTopLeftCorner = boardEncoding[0][0][0] + boardEncoding[1][1][1] + boardEncoding[2][2][2] + boardEncoding[3][3][3] + boardEncoding[4][4][4];
			sumTopRightCorner = boardEncoding[0][0][4] + boardEncoding[1][1][3] + boardEncoding[2][2][2] + boardEncoding[3][3][1] + boardEncoding[4][4][0];
			sumBottomLeftCorner = boardEncoding[0][4][4] + boardEncoding[1][3][3] + boardEncoding[2][2][2] + boardEncoding[3][1][1] + boardEncoding[4][0][0];
			sumBottomRightCorner = boardEncoding[0][4][0] + boardEncoding[1][3][1] + boardEncoding[2][2][2] + boardEncoding[3][1][3] + boardEncoding[4][0][4];

			for (int i = 0; i < DIMENSION; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{
					for (int k = 0; k < DIMENSION; k++)
					{
						if ((i == 0 && j == 0 && k == 0) || (i == 1 && j == 1 && k == 1) || (i == 2 && j == 2 && k == 2) || (i == 3 && j == 3 && k == 3) || (i == 4 && j == 4 && k == 4))
						{
							if (abs(boardScore[i][j][k]) < abs(sumTopLeftCorner))
							{								
								boardScore[i][j][k] = sumTopLeftCorner;								
							}
						}
						else if ((i == 0 && j == 0 && k == 4) || (i == 1 && j == 1 && k == 3) || (i == 2 && j == 2 && k == 2) || (i == 3 && j == 3 && k == 1) || (i == 4 && j == 4 && k == 0))
						{
							if (abs(boardScore[i][j][k]) < abs(sumTopRightCorner))
							{								
								boardScore[i][j][k] = sumTopRightCorner;							
							}
						}
						else if ((i == 0 && j == 4 && k == 4) || (i == 1 && j == 3 && k == 3) || (i == 2 && j == 2 && k == 2) || (i == 3 && j == 1 && k == 1) || (i == 4 && j == 0 && k == 0))
						{
							if (abs(boardScore[i][j][k]) < abs(sumBottomLeftCorner))
							{								
								boardScore[i][j][k] = sumBottomLeftCorner;							
							}
						}
						else if ((i == 0 && j == 4 && k == 0) || (i == 1 && j == 3 && k == 1) || (i == 2 && j == 2 && k == 2) || (i == 3 && j == 1 && k == 3) || (i == 4 && j == 0 && k == 4))
						{
							if (abs(boardScore[i][j][k]) < abs(sumBottomRightCorner))
							{								
								boardScore[i][j][k] = sumBottomRightCorner;	
							}
						}
					}
				}
			}
		}

		void DefineMove(int &moveZ, int &moveY, int &moveX)
		{
			double maxScore = 0.0;
			int count = 0;
			for (int j = 0; j < DIMENSION; j++)
			{					
				for (int k = 0; k < DIMENSION; k++)
				{
					if (maxScore < inputAccOutput[count] && openPosition[optimalPlane][j][k] == true)
					{
						maxScore = inputAccOutput[count];
						moveY = j;
						moveX = k;
					}
					count++;
				}					
			}
			moveZ = optimalPlane;
		}
		
		double RandomizeThreshold()
		{
			double thres;
			int randnum;
  
			// generates random number between 0 - 0.5    
			randnum = rand();
			thres = (double)((double)0.5*randnum/RAND_MAX);  
  
			return thres;
		}

		double GetSigmoidValue(double x)
		{
			  double y;
			  y = 1 / (1 + exp(-x));
			  return y;
		}

		void SelectDesiredValue()
		{
			int matchcnt = 0, index = 0, max_match_cnt = 0;

			// select best desired pattern from text file
			for (int h = 1; h < 121; h++)
			{
				for (int i = 0; i < DIMENSION; i++)
				{
					for (int j = 0; j < DIMENSION; j++)
					{
						if (boardEncoding[optimalPlane][i][j] == WINPATTERN2D[h][i][j])
							matchcnt++;
					}
				}
				if (max_match_cnt < matchcnt)
				{
					index = h;
					max_match_cnt = matchcnt;					
				}
				matchcnt = 0;
			}

			// sets Output Layer desired values
			int cnt = 0;
			for (int i = 0; i < DIMENSION; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{					
					DesiredValue[cnt] = WINPATTERN2D[index][i][j];
					cnt++;
				}
			}
			
		}

		void ActivateNetwork()
		{
			// loop through all hidden neurons and calculate weighted input
			for (int h = 0; h < NUM_HIDDEN_NEURONS; h++)
			{
					double weighted_input = 0;
					for (int i = 0; i < NUM_INPUT_NEURONS; i++)
					{
						weighted_input += inputToHiddenWeights[h][i] * inputLayer[i];
					}
					weighted_input += -1 * inputThreshold[h];
					inputAccHidden[h] = GetSigmoidValue(weighted_input);
			}

			// loop through all output neurons and calculate weighted input
			for (int o = 0; o < NUM_OUTPUT_NEURONS; o++)
			{
					double weighted_input = 0;
					for (int h = 0; h < NUM_HIDDEN_NEURONS; h++)
					{
						weighted_input += inputToHiddenWeights[o][h] * inputAccHidden[h];
					}
					weighted_input += -1 * outputThreshold[o];
					inputAccOutput[o] = GetSigmoidValue(weighted_input);	
			}
		}
		
		double ModifyWeight ()
		{			
			double sum_square_error = 0;
			
			// loop through output neurons for Backward propogation 
			for (int o = 0; o < NUM_OUTPUT_NEURONS; o++)
			{
				double abs_error = abs((double)DesiredValue[o] - inputAccOutput[o]);				
				sum_square_error += pow(abs_error,2);
				double output_dev = (inputAccOutput[o]) * (1 - inputAccOutput[o]) * abs_error;

				// loop through hidden neurons
				for (int h = 0; h < NUM_HIDDEN_NEURONS; h++)
				{
					  double delta_hidden = learningrate * inputAccHidden[h] * output_dev;
					  
					  if (abs(hiddenToOutputWeights[o][h] + delta_hidden) > 0 && abs(hiddenToOutputWeights[o][h] + delta_hidden) < 1)
						  hiddenToOutputWeights[o][h] += delta_hidden;

					  double hidden_dev = (inputAccHidden[h]) * (1 - inputAccHidden[h]) * (output_dev) * (inputToHiddenWeights[o][h]);
	  
					  for (int i = 0; i < NUM_INPUT_NEURONS; i++)
					  {
						double delta_input = learningrate * inputLayer[i] * hidden_dev;
						if(abs(inputToHiddenWeights[h][i] + delta_input) > 0 && abs(inputToHiddenWeights[h][i] + delta_input) < 1)
							inputToHiddenWeights[h][i] += delta_input;
					  }
					  double thetaDelta =  -1 * learningrate * hidden_dev;
					  hiddenThreshold[h] += thetaDelta;
					  
				}
				double thetaDelta = -1 * learningrate * output_dev;
				outputThreshold[o] += thetaDelta;
			}
			  return sum_square_error;
		}

		void TrainingAlg()
		{
			double curr_rmsError = 0, prev_rmsError = 100;			
			
			do
			{
				ActivateNetwork();
				curr_rmsError = ModifyWeight();
							
				if (prev_rmsError < curr_rmsError + .001)
					break;
				else
					prev_rmsError = curr_rmsError;				
			}   while (curr_rmsError > 2.20);	
		}

		/*void SelectPlayer(ifstream & file1, ifstream & file2)
		{
			int count = 0;
			int index = 0;
			if (!file1)
			{
				cout<<"Error in SelectPlayer: File Not Found!"<<endl;
				return;
			}
			else
			{				
				string line = "";
				while (!file1.eof())
				{
					
					count++;
					getline(file1,line,'\n');
					for(unsigned int i=0; i < line.length();i++)
					{
						if(line.at(i) != ',')
						{
							if(line.at(i) == '-')
							{
								BEFOREPATTERN[index] = -1;
								i++;
							}
							else
							{					
									if(line.at(i) == '0')
									BEFOREPATTERN[index] = 0;
									else if(line.at(i) == '1')
									BEFOREPATTERN[index] = 1;					
							}
							index++;
						}
					}
		
				}
			}
			ConvertBeforePattern(index);
			ReadCorrectPatterns(file2);
		}*/

		void ConvertBeforePattern(int count)
		{
			int CT25 = 0, row = 0, col = 0;
			
			for (int i = 0; i < count; i++)
			{								
				if (i % 5 == 0)
				{
					row++;
					col = 0;
					if (i % 25 == 0)
					{
						CT25++;
						row = 0;
						col = 0;											
					}
				}
				BEFOREPATTERN2D[CT25][row][col] = BEFOREPATTERN[i];	
				col++;
			}
			PATTERN_LENGTH = CT25;			
		 }

		void GetInput ()
		{
			int count = 0;
			for (int i = 0; i < PATTERN_LENGTH; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{
					for (int k = 0; k < DIMENSION; k++)
					{
						inputLayer[count] = BEFOREPATTERN2D[i][j][k];
						count++;
					}
				}
				count = 0;
				ChooseDesiredPattern(i);
				TrainingAlg();
				cout<<"Pattern "<<i<<" Trained..."<<endl;
				UpdateWeights();
				cout<<"Updating Weights for Pattern "<<i<<endl;
			}
		}

		void ChooseDesiredPattern (int i)
		{
			int matchcnt = 0, max_match = 0, index = 0;
			for (int h = 0; h < DIMENSION; h++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{
					for (int k = 0; k < DIMENSION; k++)
					{
						if (BEFOREPATTERN2D[i][j][k] == WINPATTERN2D[h][j][k])
							matchcnt++;
					}
				}
				if (max_match < matchcnt)
				{
					max_match = matchcnt;
					index = h;
				}
				matchcnt = 0;
			}
			
			int cnt = 0;
			for (int h = 0; h < DIMENSION; h++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{
					DesiredValue[cnt] = WINPATTERN2D[index][h][j];
					cnt++;
				}
			}
		}
};


class g24
{
	private:
		char board[DIMENSION][DIMENSION][DIMENSION];
		char xORo;
		string uid;
		neural_network NN;
	public:
		void setUp(char letter)
		{
			init(board);
			xORo = letter;
			uid = "g24";				
		}

		string myUID()
		{
			return uid;
		}
	
		g24()
		{
			//FILE *pFile = fopen("./files/g24.dat1", "r");
			FILE *pFile = fopen("g24.dat1", "r"); 	
			NN.InitNetwork(&pFile);
			NN.InitScore();
		}
	
		void selectMove(int &moveZ, int &moveY, int &moveX)
		{
			NN.ReadBoard(board);
			NN.FindOptimalSlice();
			NN.ActivateNetwork();
			NN.DefineMove(moveZ, moveY, moveX);
			board[moveZ][moveY][moveX] = xORo;
		}

		void getFinalWeights()
		{
			NN.UpdateWeights();
		}

		void notifyMove(int moveZ, int moveY, int moveX)
		{
			board[moveZ][moveY][moveX] = (xORo == 'X' ? 'O' : 'X');
		}
};
