#include <iostream> 
#include <string> 
#include <fstream>
#include <ctime>
#include <cstdlib> 
#include "Player.h"

using namespace std; 

Player::Player(string playerName, int loc, int winStatus )  {
	name = playerName;
	location = loc;
	gameOver = winStatus; 
}

void Player::moveForward(int card)  { 
	location = location + card;
}

void Player::moveBackwards(int obs)  {  
	location = location - obs; 
}

void Player::switchPlayersPos(Player& p2)  {
	swap(location, p2.location);
} 

void Player::passHome(int boardSize)  {
	if (location > boardSize) 
		location =  boardSize  - (location - boardSize);
}

void Player::passStartPos()  {
	if (location < 0)
		location = 0;
}

void Player::backToStartLocation()  {
		location = 0;
} 

void Player::checkPos(int boardSize)  {
	if (location == boardSize)
		gameOver = 1; 
}


