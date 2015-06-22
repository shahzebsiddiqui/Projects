#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <vector>

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
const int count = 10000;
#define DIMENSION 5

class neural_network 
{
	private:
		double learningrate;	
		double InputHiddenWeights[NUM_INPUT_NEURONS][NUM_HIDDEN_NEURONS]; 
		double HiddenOutputWeights[NUM_HIDDEN_NEURONS][NUM_OUTPUT_NEURONS]; 
		double wts[2*NUMLINKS_IN_LAYER];

		double InputThreshold[NUM_INPUT_NEURONS];
		double HiddenThreshold[NUM_HIDDEN_NEURONS];
		double OutputThreshold[NUM_OUTPUT_NEURONS];
		
		double InputAccHidden[NUM_HIDDEN_NEURONS];
		double InputAccOutput[NUM_OUTPUT_NEURONS];		
		int DesiredValue[NUM_OUTPUT_NEURONS];

		int InputNN[NUM_INPUT_NEURONS];
		
		int boardEncoding[DIMENSION][DIMENSION][DIMENSION];		
		int boardEncodingInput[DIMENSION][DIMENSION];
		int boardScore[DIMENSION][DIMENSION][DIMENSION];
		int WINPATTERN[count];
		int WINPATTERN2D[121][DIMENSION][DIMENSION];		
		int TileWinLossScore[DIMENSION*DIMENSION*DIMENSION];		

		bool Position[DIMENSION][DIMENSION][DIMENSION];
		
		int sliceIndex;
		int move[3];
	public:	
		neural_network()
		{
			SetLearningRate(0.6);
		}
		neural_network(double learningRate)	
		{			
			srand ( time(NULL) );
			SetLearningRate(learningRate);
		}
		void SetLearningRate(double learningRate)
		{
			learningrate = learningRate;
		}
		void InitializeTile()
		{
			for (int i = 0; i < DIMENSION*DIMENSION*DIMENSION; i++)
			{
				TileWinLossScore[i] = 0;
				//cout << TileWinLossScore[i] << endl;
			}
			//cin.get();
		}
		void initializeScore()
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
		void Build_NeuralNetwork()
		{
			ofstream fout;
			fout.open("weights.txt");

			  for(int N = 1; N < NUMLAYERS; N++)
			  {
	
				  for (int i = 0; i < NUM_INPUT_NEURONS; i++)	  
				  {
					  for (int h = 0; h < NUM_HIDDEN_NEURONS; h++)
					  {
						  double weight = randomweight();
						  if (N == 1)
						  {
							  InputHiddenWeights[i][h] = weight;
							  fout << InputHiddenWeights[i][h] << endl;
						  }
						  else
						  {
							  HiddenOutputWeights[i][h] = weight;
							  fout << InputHiddenWeights[i][h] << endl;
						  }
					  }
				  }
			  }
			  /*for (int i = 0; i < DIMENSION; i++)
			  {
				  for (int j = 0; j < DIMENSION; j++)
				  {
					  for (int k = 0; k < DIMENSION; k++)
					  {
						  fout << boardScore[i][j][k] << endl;
					  }
				  }
			  }*/
			  
			  for (int i = 0; i < NUM_INPUT_NEURONS;i++)
			  {				  
				  InputThreshold[i] = randomthresh();
			  }
			  for (int i = 0; i < NUM_HIDDEN_NEURONS;i++)
			  {				  
				  HiddenThreshold[i] = randomthresh();
			  }
			  for (int i = 0; i < NUM_OUTPUT_NEURONS;i++)
			  {				  
				  OutputThreshold[i] = randomthresh();
			  }

			  fout.close();
		}
		void ReadWeights(ifstream & read, const char * fileName)
		{
			
			int linecnt = 0;
			string line = "";
			if(!read)
			{
				cout<<"Error: File Not Found!"<<endl;
			}
			else
			{								
				while(!read.eof())
				{					
					getline(read,line,'\n');				
					
					if (linecnt == 1250)							
					{
						continue;
							
					}
					else
					{
						if (linecnt < 1250)
						wts[linecnt] = atof(line.c_str());
						else
						{
							TileWinLossScore[linecnt-1250] = atoi(line.c_str());
							//cout << "TileWinLossScore[" << linecnt-1250 << "]:" << TileWinLossScore[linecnt-1250] << endl;
						}

					}				
					
					linecnt++;
				}
			}

			int row = 0, col = 0;
			
			for (int i = 0; i < NUMLINKS_IN_LAYER;i++)
			{
				InputHiddenWeights[row][col] = wts[i];				
				if (i % 25 == 0)
				{
					row++;
					col = 0;
				}
				col++;
			}			
			
			row = 0;
			col = 0;
			
			for (int i = NUMLINKS_IN_LAYER; i < 2*NUMLINKS_IN_LAYER;i++)
			{
				HiddenOutputWeights[row][col] = wts[i];				
				if (i % 25 == 0)
				{
					row++;
					col = 0;
				}
				col++;
			}
			
			//cin.get();
			
			/*for (int i = 0; i < 125; i++)
			{
				cout << "TileWinLossScore[" << i << "]:" << TileWinLossScore[i] << endl;
			}
			cin.get();*/
			return;
		}
		void ReWriteWeights()
		{
			ofstream fout;
			fout.open("weights.txt");
			for (int i = 0; i < NUM_INPUT_NEURONS; i++)
			{
				for (int j = 0; j < NUM_HIDDEN_NEURONS; j++)
				{
					fout << InputHiddenWeights[i][j] << endl;
				}
			}

			for (int i = 0; i < NUM_HIDDEN_NEURONS; i++)
			{
				for (int j = 0; j < NUM_OUTPUT_NEURONS; j++)
				{
					fout << HiddenOutputWeights[i][j] << endl;
				}
			}
			for (int i = 0; i < DIMENSION*DIMENSION*DIMENSION; i++)
			{
				fout << TileWinLossScore[i] << endl;				
			}
			
			fout.close();
		}
		void ReadPatterns(ifstream & read, const char * fileName)
		{
			int count = 0;
			int index = 0;
			if (!read)
			{
				cout<<"Error: File Not Found!"<<endl;
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
		}
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
				col++;
			}
						
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
							Position[z][y][x] = false;
							boardEncoding[z][y][x] = 1;							
						}
						else if(gameboard[z][y][x] == 'O')
						{
							Position[z][y][x] = false;
							boardEncoding[z][y][x] = -1;							
						}
						else if(gameboard[z][y][x] == ' ')
						{
							Position[z][y][x] = true;
							boardEncoding[z][y][x] = 0;							
						}
						else
						{
							cout<<"Error in ReadBoard"<<endl;
							return;
						}
					}
				}
			}
			return;
		}
		
		void SelectBoardConfig()
		{											
			for (int i  = 0; i < DIMENSION; i++)
			{
				ZPlane(i);
				YPlane(i);
				XPlane(i);
		    }
			ComputeSpecialScore();
			/*for (int i = 0; i < DIMENSION; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{
					for (int k = 0; k < DIMENSION; k++)
					{
						cout << boardScore[i][j][k];
					}
					cout << endl;
				}
				cout  << endl;
			}*/
			//cin.get();
		}
		
		int GetXCoor()
		{
			return move[2];
		}
		int GetYCoor()
		{
			return move[1];
		}
		int GetZCoor()
		{
			return move[0];
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
			
			//cout << "sumDiagTopLeft:" << sumDiagTopLeft << "sumDiagTopRight:" << sumDiagTopRight << "sumHorz" << sumHorz << "sumVert" << sumVert << endl;
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
			/*cout << "1TopLeftCorner = " << sumTopLeftCorner << endl;
			cout << "1TopRightCorner = " << sumTopRightCorner << endl;
			cout << "1BottomLeftCorner = " << sumBottomLeftCorner << endl;
			cout << "1BottomRightCorner = " << sumBottomRightCorner << endl;*/
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

		void DefineMove()
		{						
			int maxScore = 0, cnt = 0;

			for (int i = 0; i < DIMENSION; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{					
					for (int k = 0; k < DIMENSION; k++)
					{						
						if (maxScore < abs(boardScore[i][j][k]) && Position[i][j][k] == true && TileWinLossScore[cnt] >= 0)
						{
							maxScore = boardScore[i][j][k];
							move[0] = i;
							move[1] = j;
							move[2] = k;
						}
						cnt++;
					}					
				}				
			}				
		}
		
		double randomweight()
		{  
		  double weight;
		  int randnum;
  
		  // generate random number between 0 - 0.5    
		  randnum = rand();
		  weight = (double)((double)0.5*randnum/RAND_MAX);  
  
		  return weight;
    
		}
		double randomthresh()
		{
			return randomweight();
		}
		double sigmoid (double x)
		{
			  double y;
			  y = 1 / (1 + exp(-x));
			  return y;
		}
		void SelectPattern(int z, int y, int x)
		{
			
			for (int i = 0; i < DIMENSION; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{										
					boardEncodingInput[i][j] = boardEncoding[z][i][j];		
					cout << boardEncodingInput[i][j];
				}
				cout << endl;
				//cin.get();
			}								
			training(boardEncodingInput);			
		}
		void SelectDesiredValue(int Pattern[DIMENSION][DIMENSION])
		{
			int matchcnt = 0, index = 0, max_match_cnt = 0;
			// select best desired pattern from text file
			for (int h = 1; h < 121; h++)
			{
				for (int i = 0; i < DIMENSION; i++)
				{
					for (int j = 0; j < DIMENSION; j++)
					{
						if (Pattern[i][j] == WINPATTERN2D[h][i][j])
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
			ofstream fout;
			fout.open("desiredvalue.txt",fstream::app);

			// sets Output Layer desired values
			int cnt = 0;
			for (int i = 0; i < DIMENSION; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{					
					DesiredValue[cnt] = WINPATTERN2D[index][i][j];
					fout << WINPATTERN2D[index][i][j] << " ";
					cnt++;
				}
				fout << endl;
			}
			fout << endl;
			fout.close();
			
		}
		void GetInput(int Pattern[DIMENSION][DIMENSION])
		{
			ofstream fout;			
			fout.open("inputs.txt",fstream::app);
			int count = 0;
			for (int i = 0; i < DIMENSION; i++)  
			{
				for(int j = 0;j < DIMENSION; j++)
				{
					InputNN[count] = Pattern[i][j];					
					fout << InputNN[count]  << " ";					
					count++;
				}
				fout << endl;
			}
			fout << endl;
			fout.close();
			SelectDesiredValue(Pattern);
			
		}
		void ActivateNetwork()
		{
			//ofstream fout;
			//fout.open("output.txt",fstream::app);
			// loop through all hidden neurons and calculate weighted input
			for (int h = 0; h < NUM_HIDDEN_NEURONS; h++)
			{
					double weighted_input = 0;
					for (int i = 0; i < NUM_INPUT_NEURONS; i++)
					{
					  weighted_input += InputHiddenWeights[h][i] * InputNN[i];
					}
					weighted_input += -1 * InputThreshold[h];
					InputAccHidden[h] = sigmoid(weighted_input);						
			}

			// loop through all output neurons and calculate weighted input
			for (int o = 0; o < NUM_OUTPUT_NEURONS; o++)
			{
					double weighted_input = 0;
					for (int h = 0; h < NUM_HIDDEN_NEURONS; h++)
					{
					  weighted_input += InputHiddenWeights[o][h] * InputAccHidden[h];
					}
					weighted_input += -1 * OutputThreshold[o];
					InputAccOutput[o] = sigmoid(weighted_input);	
					
					//fout << "thresh = " << OutputThreshold[o] << endl;
					//fout << "Output Neuron #" << o << "\t" << InputAccOutput[o] << endl;
			}
			//cin.get();
			//fout << endl;
			//fout.close();

		}		
		
		double ModifyWeight ()
		{
			/*double DesiredValue[25] = {0,0,0,0,0,
					   0,0,0,0,0,
					   0,0,0,0,0,
					   0,0,0,0,0,
					   0,0,0,0,0};*/
			double sum_square_error = 0;
			// loop through output neurons for Backward propogation 
			for (int o = 0; o < NUM_OUTPUT_NEURONS; o++)
			{
				double abs_error = DesiredValue[o] - InputAccOutput[o];
				sum_square_error += pow(abs_error,2);
				double output_dev = (InputAccOutput[o]) * (1 - InputAccOutput[o]) * abs_error;
	
				// loop through hidden neurons
				for (int h = 0; h < NUM_HIDDEN_NEURONS; h++)
				{
					  double delta_hidden = learningrate * InputAccHidden[h] * output_dev;
					  InputHiddenWeights[o][h] += delta_hidden;
					  double hidden_dev = (InputAccHidden[h]) * (1 - InputAccHidden[h]) * (output_dev) * (InputHiddenWeights[o][h]);
	  
					  for (int i = 0; i < NUM_INPUT_NEURONS; i++)
					  {
						double delta_input = learningrate * InputNN[i] * hidden_dev;
						InputHiddenWeights[h][i] = delta_input;
					  }
					  double thetaDelta =  -1 * learningrate * hidden_dev;
					  HiddenThreshold[h] += thetaDelta;
				}
				double thetaDelta =  -1 * learningrate * output_dev;
				OutputThreshold[o] += thetaDelta;
			}
			  return sum_square_error;
		}
		void UpdateTileScore(char winner)
		{
			bool win;
			if (winner == 'X')
			{
				win = false;				
			}
			else
			{
				win = true;
			}
			
			int index = 0;
			
			for (int i = 0; i < DIMENSION; i++)
			{
				for (int j = 0; j < DIMENSION; j++)
				{
					for (int k = 0; k < DIMENSION; k++)
					{
						if (win == false)
						{
							if (boardEncoding[i][j][k] == -1)
							  TileWinLossScore[index] -= 2;
						}
						else
						{
							if (boardEncoding[i][j][k] == -1)
							TileWinLossScore[index] += 1;
						}
						index++;
						//cout << TileWinLossScore[i] << endl;
					}
				}
			}
			//cin.get();				
				
		}
		
		void training(int Pattern[DIMENSION][DIMENSION])
		{
			double rmsError = 0;
			GetInput(Pattern);
			do
			{
				ActivateNetwork();
				rmsError = ModifyWeight();
				//cout << rmsError << endl;				
			}			
			while (rmsError > 4.05);
			//cin.get();
			
		}
};


class sms5713
{
	private:
		char board[DIMENSION][DIMENSION][DIMENSION];
		char xORo;
		string uid;
	
	public:

		// THIS FUNCTION WILL BE CALLED ONCE AT THE
		// BEGINNING OF THE GAME. MAKE SURE YOU CORRECTLY
		// SET THE "xORo" and "uid" variables.
		// ANYTHING ELSE YOU WANT TO DO IS UP TO YOU.
		neural_network NN;
		void setUp(char letter)
		{
			init(board);
			xORo = letter;
			uid = "SMS5713";				
		}
		string myUID()
		{
			return uid;
		}
	
		sms5713()
		{			
			double learningrate = 0.2;
			int num_iter = 10000;
			NN.SetLearningRate(learningrate);
			NN.initializeScore();
			NN.Build_NeuralNetwork();
			//NN.training();
		}
	
		void selectMove(int &moveZ, int &moveY, int &moveX)
		{			
			// OBVIOUSLY, YOUR CODE WILL HAVE TO DO SOMETHING MORE
			// INTERESTING THAN THIS		
			//int x;
			/*do
			{
				cout << "*"; // Just indicates "thinking..."
				moveZ = rand() % DIMENSION;
				moveY = rand() % DIMENSION;
				moveX = rand() % DIMENSION;
			} while (board[moveZ][moveY][moveX] != ' ');*/
			
			board[moveZ][moveY][moveX] = xORo;
		}

		// WHEN THE OPPONENT MOVES, THIS FUNCTION WILL BE CALLED
		// YOU CAN MODIFY IT ANY WAY YOU'D LIKE
		void notifyMove(int moveZ, int moveY, int moveX)
		{
			board[moveZ][moveY][moveX] = (xORo == 'X' ? 'O' : 'X');
		}
};
