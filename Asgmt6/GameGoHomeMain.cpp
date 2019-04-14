#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include "Player.h"
using namespace std;

// Function Prototype Declaration.
void programmerInfo(ofstream&);
void gameHomeScreen(ofstream& );
int userResponseToHomeScreen(int&, int&, const string, ofstream&);
void gameRules(ofstream& );
void credit (ofstream&);
void initPlayerNames(const string, const string);
void sortTurn();
int* createsBoard(const int, const string, ofstream&);
int generatesRandomCard(const int, const int );

void switchPlayersTurn (int*, ofstream&);
void switchPlayersPosition(int);
void playerPositionCheck(int&);
void showState(ofstream&);
bool checkForObstacle(int[], int, ofstream&);

const int MAX_PLAYERS = 6; 
int numPlayers = 0, userInputBoardSize = 0;
static Player players[MAX_PLAYERS];

int main()
{	
	srand( static_cast<unsigned int>(time(NULL)));
    //declaring the outfile and then opening it.
    ofstream outFile ("GameGoHome.txt");
    //uncomment below for outputting the program on screen for debugging purposes.
    //ofstream outFile("/dev/stdout");

	string fileName1 = "UserInterface1.txt", fileName2 = "UserInterface2.txt", fileName3 = "PlayerNames.txt", fileName4 = "UserInterface3.txt", fileName5 = "UserInterface4.txt"; 
    programmerInfo(outFile); // Calling function funcProgrammerInfo() to print my info.
    gameHomeScreen(outFile);

    int gameStatus = 0, fileNoCount = 0; 
    userResponseToHomeScreen(gameStatus, fileNoCount, fileName1, outFile);
    
    while (gameStatus != 1)   {
		outFile.width(65);  // Centers the below cout string to display it as a header.
		outFile << endl << "The GOHOME! Game has begun." << endl << endl;

		initPlayerNames(fileName2, fileName3);
		//STEP 1 -  Below function sortes players according to shorter first name
		sortTurn();

		const int GO_AGAIN = -2; 
		int* boardReturn = createsBoard(GO_AGAIN, fileName4, outFile);
		
		// STEP 3 - This loop prints all the players name in the game and states they are all at 0 position which is the starting position on the board game.
		for(int j = 0; j < numPlayers; j++)  {
			if(j == numPlayers-1)
				outFile << " and ";
			else if(j > 0)
				outFile << ", ";
			outFile << players[j].getName();
		}
   		outFile << " is at the starting position, position 0 of the GoHome! Game." << endl;	
		 
		// STEP 6 - Calls the switchPlayersTurn() to get the GAME rolling.
		switchPlayersTurn (boardReturn, outFile);
		
		outFile << endl << endl << endl << "The Game GoHome! Program has ended.\n" << endl;
		delete [] boardReturn;
		boardReturn = NULL;
		gameHomeScreen(outFile);
		userResponseToHomeScreen(gameStatus, fileNoCount, fileName5, outFile);
	}
	//Exits the games if the userinputed e/E to exit the game.
	if ( gameStatus == 1) {
        outFile << " The user has exited Game GoHome! Program." << endl; 
        return 0;
    }
}

// Below main function consists of all "USER DEFINED FUNCTIONS."

/*  Function programmerInfo()
 *  Input:
 *      None.
 *  Process:
 *      None.
 *  Output:
 *      Prints Students Name, Unix Username, Class & Section, Professor Name and Assignment#.
 */

void programmerInfo(ofstream &outFile)  {
    outFile << "Kenneth Dorji" << endl << "kdorji" << endl;
    outFile << "CISC 3110-MY9" << endl << "Prof: Dr. Dina Sokol" << endl;
    outFile << "Assignment#6 - Game GOHOME!" << endl;
    outFile << "April 26, 2017" << endl << endl;
    return;
}

/*  Function gameHomeScreen()
 *  Input:
 *      Referenced infile from main()
 *  Process:
 *      None.
 *  Output:
 *      Prints GOHOME! Game home screen.
 */

void gameHomeScreen(ofstream& outFile)  {
    for(int i = 0; i < 30; i++)
        outFile << " * ";
    outFile  << endl;

    for(int i = 0; i < 10; i++)  {
        outFile  << " *";
        if ( i == 1 )  {
            outFile.width(51); outFile << " Welcome to Game GoHome!";
            outFile.width(36); outFile << "*" << endl;
            continue;
        }
        else if ( i == 7)  {
            outFile.width(20);
            outFile << "PLAY(P)";
            outFile.width(15);
            outFile << "RULES(R)";
            outFile.width(15);
            outFile << "CREDITS(C)";
            outFile.width(15);
            outFile << "EXIT(E)";
            outFile.width(22);
            outFile << "*" << endl;
            continue;
        }
        outFile.width(87);
        outFile << "*" << endl;
    }
    for(int i = 0; i < 30; i++)
        outFile << " * ";
    outFile << endl << endl;
    return;
}

/*  Function credit()
 *  Input:
 *      None.
 *  Process:
 *      None.
 *  Output:
 *      Prints credits for GOHOME! Game.
 */

void credit (ofstream& outFile)  {
    outFile.width(55);
    outFile << "Credits" << endl;
    outFile << "Prof. Dina Sokol" << endl << "Prof. Chaim Ziegler" << endl << "Prof. Yan Chen" << endl;
    outFile << "Brooklyn College" << endl << "Borough of Manhattan Community College" << endl << endl;
    return;
}

/*  Function userResponseToHomeScreen()
 *  Input:
 *      All datatypes in the parameter are reference by main()
 *  Process:
 *      When user inputs:
 *      p\P - it means the user wants to play the game so it exits and goes to main() to start the game.
 *      r\r - it calls gamerules() to print the game rules.
 *      c\C - it calls the credit() to print the credit for the game.
 *      q\Q - it returns gameStatus to main funciton and the main fuction exits the program.
 *  Output:
 *      Prints the Game Rules and Credit for the game.
 */

int userResponseToHomeScreen(int& gameStatus, int& fileCount, const string filePath, ofstream& outFile )  {
	fileCount++; 
	ifstream inFile(filePath);
	if (inFile.fail()) {
		cerr << "File path is not valid. Creating file." << endl;
        if (fileCount == 1)  {
			ofstream outFile1("UserInterface1.txt");
			outFile1 << "c\n R\n p" << endl;
			outFile1.close();
        }
        else {
        	ofstream outFile2("UserInterface4.txt");
			outFile2 << "E" << endl;
			outFile2.close();
		}
	}
    cout << endl << " Enter \"p/P\" for Play, \"r/R\" for Game Rules, \"c/C\" for Credit and \"e/E\" for exit.  " << endl; 
    char userResponse; 
    inFile >> userResponse;
    while (userResponse != 'e' && userResponse != 'E')  {
        if (userResponse == 'p' || userResponse == 'P')
            return gameStatus;
        else if (userResponse == 'r' || userResponse == 'R')
            gameRules(outFile);
        else // (userResponse == 'c' || userResponse == 'C')
            credit(outFile);
		inFile >> userResponse;
    }
    if (userResponse == 'e' || userResponse == 'E') 
        gameStatus = 1;	
	inFile.close();
    return gameStatus;
}

/*  Function gameRules()
 *  Input:
 *      None.
 *  Process:
 *      None.
 *  Output:
 *      Prints the GOHOME! Game Rules.
 */

void gameRules(ofstream &outfile)  {
    outfile.width(62);
    outfile << "Rules of Game GoHome!" << endl << endl;
    outfile << "- The object of this board game is to reach \"Home\" first which is the last square on the board game." << endl;
    outfile << "- The order of the player's turn depends on their first name. The player with the shortest first name goes first and the player with the longest first name goes last." << endl << endl;
    outfile << "- There are 10 cards in the GOHOME!. Two cards with each number from 1-4, one card with \"lose a turn\" and one with \"switch places with another player.\"" << endl << endl;
    outfile << "- A turn consists of picking a card. if there is number on the card the player moves forward that number of spaces." << endl << endl;
    outfile << "* Condtion 1:-  If the card says \"lose a turn\" then the player does nothing, and the turn moves to the next player." << endl << endl;
    outfile << "* Condition 2:- If the player gets \"Switch places\" then it must switch position with another player." << endl << endl;
    outfile << "* Condition 3:- After the player moves, if the player lands on an obstacle, the player must moves back the number of spaces depending on the landed obstacle which range from 1-3 spaces backwards";
    outfile << "If the player lands on an obstacle after moving backwards, it does not have to move again." << endl << endl;
    return;
}

/*  Function createsBoard()
 *  Input:
 *      int boardArray[] - A array passed by reference by main function.
 *      int boardSize - A constant int passed by value by main function.
 *  Process:
 *      1st - The for loop fills up the array with 0 value, where 0 is a non-obstacle location on board.
 *      2nd - Ten obstacles from value 1 - 3 is filled randomly in the random array location.
 *  Output:
 *      None.
 */

int* createsBoard(const int GO_AGAIN, const string filePath, ofstream& outFile)
{
	ifstream inFile(filePath);
	if (inFile.fail()) {
		cerr << "File path is not valid. Creating UserInterface3 file." << endl;
        ofstream outFile1("UserInterface3.txt");
        outFile1 << "\n15 \n5 \n2" << endl;
        outFile1.close();
	}
	//inFile.open(filePath);
	
	int userInputObstacles = 0, usersLargestObstacle = 0;
    cout << endl << "How large should the board be?" << endl;
    inFile >> userInputBoardSize;
    int* boardGame = new int [userInputBoardSize + 1];

    cout << endl << "How many obstacles do you want to place on the board?" << endl;
    inFile >> userInputObstacles;
    cout << endl << "What should the largest obstacle number be?" << endl;
    inFile >> usersLargestObstacle;
    // This for loop will fill up the array with 0 values where 0 represents a regular spot without an obstacle
    for (int j=0; j < (userInputBoardSize + 1); j++)
        boardGame[j] = 0;

    // This for loop places user inputed amt of random obstacles value from 1 - 3 on random location on the board. 1 represents an obstacle where you move back 1, 2 represents step back 2, 3 represents 3 step back 3.
    for (int i = 1; i <= userInputObstacles; i++)  {
        int randBoardLocation = (rand() % (userInputBoardSize - 1))+1;
		
        while ( randBoardLocation != 0  && boardGame[randBoardLocation] != 0 )
            randBoardLocation = ((rand() % userInputBoardSize)-1);
        int randObstacleNo = ((rand() % usersLargestObstacle)+1);
        boardGame[randBoardLocation] = randObstacleNo;
    }
    int totalNoOfGoAgainInBoard = userInputBoardSize/10;
    for (int m = 0; m < totalNoOfGoAgainInBoard; m++)  {
        int randBoardLocation = ((rand() % userInputBoardSize)-1);

        while ( randBoardLocation != 0  && boardGame[randBoardLocation] != 0 )
            randBoardLocation = ((rand() % userInputBoardSize)-1);
        boardGame[randBoardLocation] = GO_AGAIN;
    }
    outFile << "The user requested " << userInputBoardSize << " locations for Board Size and " << userInputObstacles
            <<  " obstacles in the board." << endl << endl;
    outFile << "Following is the picture of the Game GoHome Board:" << endl;
    for (int k = 0; k < (userInputBoardSize + 1); k++)
    {
        if ( k % 35 == 0)
            outFile << endl;
        outFile << boardGame[k] << "  " ;
    }
    outFile << endl << endl;
    inFile.close(); 
    return boardGame;
}

/*  Function initPlayerNames()
 *  Input:
 *      string - playerName[] array is passed by firstPlayCalculation
 *      int - A const int variable is passed by value by firstPlayCalculation
 *      int - numPlayers variable is passed by reference by firstPlayCalculation
 *  Process:
 *      1st while loop test whether user inputed no or alphabetical characters.
 *      2nd while loop test whether user inputed below or above required no of players for playing the game.
 *      3rd the for loop ask the users to input their names.
 *  Output:
 *      All output in this function is outputed to the screen to interact with the user.
 */

void initPlayerNames(const string filePath1, const string filePath2) {
	ifstream inFile1(filePath1);
	if (inFile1.fail()) {
		cerr << "File path is not valid. Creating UserInterface1 file." << endl;
        ofstream outFile("UserInterface2.txt");
        outFile << "4\n" << endl;
        outFile.close();
	}
	//inFile1.open(filePath1); 
	int maxGamePlayer = 6;
	cout << "This game accommodates 2-6 players." << endl << endl;
    cout << "How many players will be playing?" << endl;
    inFile1 >> numPlayers;
    while (numPlayers > maxGamePlayer || numPlayers < 2) {
        cout << "ERROR" << endl;
        if (numPlayers < 2)
            cout << "You've entered " << numPlayers << " player but this game requires minimun of two players to play the game." << endl;

        else
            cout << "You've entered " << numPlayers << " players but this game can only accomodate maximum of 6 players." << endl;

        cout << "Please reenter the number of players, playing the game?" << endl;
        inFile1 >> numPlayers;
    }
    cout << endl;
    inFile1.close(); 
    
    ifstream inFile2(filePath2);
	if (inFile2.fail()) {
		cerr << "File path is not valid. Creating PlayerNames file." << endl;
        ofstream outFile("PlayerNames.txt");
        outFile << "Sophia\n Charlotte\n Mia\n Harper" << endl;
        outFile.close();
	}
	//inFile2.open(filePath2); 
    
    int count = 0; string playerNameCheck, playerName; 
    for (int i=0; i<numPlayers; i++)  {
        count++;
        cout << "Player" << count << " enter your first name: " << endl;
        inFile2 >> playerName;
        players[i].setName(playerName); 
    }
	inFile2.close(); 
	return;
}

/*  Function sortTurn()
 *  Input:
 *      string - playerName[] array is passed by gamePlay()
 *      cont int - A const int variable is passed by value by gamePlay()
 *      int& - numPlayers variable is passed by reference by gamePlay()
 *  Process:
 *      Does Bubble Sorting in an ascending order in the array playerNames[].
 *  Output:
 *      None.
 */

void sortTurn()  {
    string temp;
    for(int i = 1; i < numPlayers; i++)  {
        for(int j = 0; j < (numPlayers - 1); j++)  {
            if(players[j].getNameLength() > players[j + 1].getNameLength())  {
                temp = players[j].getName();
                players[j].setName(players[j + 1].getName());
                players[j + 1].setName(temp);
            }
        }
    }
}

/*  Function switchPlayersTurn()
 *  Input:
 *      string playerName[], int playersPosition[], int cardStack[], int boardGame[] - All these arrays values are passed by refernce by gamePlay()
 *      int& gameStatus and int& turn is passed by refernce by gamePlay()
 *      int numPlayers, const int LOSE_TURN, const int SWITCH_PLAYERS is passed by value by gamePlay()
 *      ofstream& outfile is passed by to gamePlay() to send to output to the outfile.
 *  Process:
 *      Step 1: Generates random number from 0 to 9. This no is used to determine the card value in the card array where the card value is hard coded.
 *      Step 2: If player gets card value 1 - 4. Then it calls backToStartPosition(), passHome() and movingBackward() functions.
 *      Step 3: If player gets LOSE_TURN card then it does nothing.
 *      Step 4: If player gets SWITCH_TURN card then it calls switchPlayersPosition() function.
 *      Step 5: Then it calls showState() and playerPositionCheck() function and evalutates for all the players.
 *  Output:
 *      Outfile - Prints Turn no and players name and the card it picked.
 */

void switchPlayersTurn (int* boardGame, ofstream& outfile)
{
	int turn = 0, winStatus = 0, tempLocation = 0; 
	while (winStatus != 1)  { 
    for (int index = 0; index < numPlayers; index++)  {
        turn++;
        outfile << endl << "Turn: " << turn << endl;
        outfile << "It's " << players[index].getName() << " turn to pick a card.";
        int card = 0; int const LOSE_TURN = 5, SWITCH_PLAYERS = 6;
        card = generatesRandomCard(LOSE_TURN, SWITCH_PLAYERS);
        if(card == 1 || card == 2 || card == 3 || card == 4) {
            outfile << " You're withdrawn card value is " << card << endl;
            // Below code adds card value ranging from 1-4 to the player1position in the board. To show the forward movement of the player1 in the game.
            players[index].moveForward(card);
            while (boardGame[players[index].getLocation()] == -2)  {
                outfile << players[index].getName() << " is in " << players[index].getLocation() << " position on the board." << endl;
                outfile << players[index].getName() << " has landed on \"Go Again!\". You get to pick a card again!" << endl;
                card = generatesRandomCard(LOSE_TURN, SWITCH_PLAYERS);
                outfile << "You're withdrawn card value is " << card << endl;
            	players[index].moveForward(card);
            }
            for (int m = 0; m < numPlayers; m++)   {
    			if (index != m) {
    				if (players[index].getLocation() == players[m].getLocation()) {
    					outfile << players[index].getName() << " has landed on " << players[m].getName() << "\'s location. So " << players[m].getName() << " has been moved back to the starting position." << endl; 
    					players[m].backToStartLocation(); 
    				}
    			}
    		}
            players[index].passHome(userInputBoardSize);
           	bool hit = checkForObstacle(boardGame, index, outfile);
           	if(hit == true)  
    			players[index].moveBackwards(boardGame[players[index].getLocation()]);
    		players[index].passStartPos();
        }
        else if ( card == LOSE_TURN ) 
            outfile << players[index].getName() << " You're withdrawn card value is \"Lose a turn\". So you've lost your turn" << endl;
        else  {
            outfile << players[index].getName() << " You're withdrawn card value is \"Switch places with another player\" " << endl;
            switchPlayersPosition(index);
        }
        // This function is called after the play to show the players position on the game.
        showState(outfile);
        // This function evalutes whether a player has reached home.
        players[index].checkPos(userInputBoardSize); 
    	if (players[index].getGameOver() == 1 )  
    		return;
    	}
    }
    return;
}
/*  Function generatesRandomCard()
 *  Input:
 *      const int LOSE_TURN and const int SWITCH_PLAYERS is passed by value by switchPlayerTurn()
 *  Process:
 *      Generates random card no from 0 - 9 and this no is used in cardStack[ ] to figure out the card value.
 *  Output:
 *      None.
 */

int generatesRandomCard(const int LOSE_TURN, const int SWITCH_PLAYERS )
{
    // The card size is an constant int with 10 card size. The card values are hard coded.
    // 1-4 value in card array represents player to move forward in the board.
    const int CARD_SIZE = 10;
    int cardStack[CARD_SIZE] = {1, 1, 2, 2, 3, 3, 4, 4, LOSE_TURN, SWITCH_PLAYERS};

    // STEP 1 -  Below code generates random no from 0 to 9 & assigns it to int temp since we have array[10] which is from array 0 to 9
    int temp = (rand() % 10);
    //Below code takes the temp no into the cardStack array and searches for the card value inside that array & finally it assigns that card value to the int card.
    int card = cardStack[temp];

    return card;
}

/*  Function switchPlayersPosition()
 *  Input:
 *      string playerName[] and int playersPosition[] values is passed by refernce by switchPlayersTurn()
 *      int currentPlayer and int numPlayers is passed by value by switchPlayersTurn()
 *  Process:
 *      Perfoms linear assorting aiding in swapping the players position in the board.
 *  Output:
 *      None. The switched players position is passed by reference.
 */

void switchPlayersPosition(int currentPlayerIndex)
{
    int furthestPlayer = 0;
    int maxPlayerPosition = players[furthestPlayer].getLocation();
    for (int j = 1; j < numPlayers; j++)  {
        if (players[j].getLocation() > maxPlayerPosition)  {
        	furthestPlayer = j;
            maxPlayerPosition = players[furthestPlayer].getLocation();
        }
    }
  	players[currentPlayerIndex].switchPlayersPos(players[furthestPlayer]);
  	return;
}

/*  Function backToStartPosition()
 *  Input:
 *      string playerName[] and int playersPosition[] values is passed by refernce by switchPlayersTurn()
 *      int currentPlayer and int numPlayers is passed by value by switchPlayersTurn()
 *  Process:
 *      If player passed home then it rebounds the players position on the board.
 *  Output:
 *      Prints to the outfile if player has passed home.
 */

void backToStartPosition(int currentPlayerIndex, ofstream& outFile)  {
    for (int m = 0; m < numPlayers; m++ )  {
        if(m != currentPlayerIndex)  {
            if (players[currentPlayerIndex].getLocation() == players[m].getLocation())  {
                players[m].setLocation(0);
                outFile << players[currentPlayerIndex].getName() << " has landed on " << players[m].getName() << "'s location.";
                outFile << players[m].getName() << " has been moved back to start position." << endl;
            }
        }
    }
    return;
}

/*  Function checkForObstacle()
 *  Input:
 *      int boardGame[], outfile is passed by reference by switchPlayersTurn()
 *      int currentPlayer is passed by value in switchPlayersTurn()
 *  Process:
 *      Board array is marked by 0(zero) for non-obstacles & 1-3 for obstacles. If player found to have landed on an obstacle, then it performs backward move according to the landed obstacle array no.(1-3).
 *  Output:
 *       Outputs the string to outfile for either cases: If player is found to be on an array location with value 0(non-obstacle) or if the player found on an obstacle.
 */

bool checkForObstacle(int boardGame[], int currentPlayerIndex, ofstream& outFile)
{	
    if (boardGame[players[currentPlayerIndex].getLocation()] != 0)  {
        outFile << "OH NO! You've landed on an obstacle. You must now move " << boardGame[players[currentPlayerIndex].getLocation()] << " position back." << endl;
		return true;
    }
	 else  {
        outFile << "You are !! SAFE !! You've landed on a non obstacle position." << endl;
        return false;
    }
}

/*  Function showState()
 *  Input:
 *      Outfile is passed by refernce by switchPlayersTurn()
 *  Process:
 *      Calculates the player's position in the board only when the player picks 1 to 4 range card value. This function is not called if player picks 5(lose turn) & 6(switch players position) value card. For 5 value card its lose a turn so we do nothing for that turn and for 6 value turn we call the funcSwitchPlayers to switch the players position.
 *  Output:
 *      This function calculates whether or not the player has reached home and accordingly outputs the player's position on the outfile.
 */

void showState(ofstream& outFile)  {
    bool some=false; int homePlayer = 0;
    for(int i = 0; i < numPlayers; i++)  {
        if ( players[i].getLocation() == userInputBoardSize)  {
            homePlayer = i;
            some=true;
        }
    }
    if(some)  {
        for(int j = 0; j < numPlayers; j++)
            outFile << players[j].getName() << " is in " << players[j].getLocation() << " position on the board." << endl;
        outFile << endl <<  players[homePlayer].getName()  << " has reached HOME! ! ! \nC O N G R A T U L A T I O N !! \nYou've won the game!" << endl;
    }
    else  {
        for(int k = 0; k < numPlayers; k++) 
            outFile << players[k].getName() << " is in " << players[k].getLocation() << " position on the board." << endl;
    }
	return;
}









