#ifndef GAMEGOHOME_H
#define GAMEGOHOME_H 

#include <iostream> 
#include <string> 
#include <fstream>
#include <ctime>
#include <cstdlib> 

//using namespace std; 

using std::string; 
using std::swap; 

class Player
{
	string name;
	int location;
	int gameOver; 
	
	public:
	Player(string name = "", int location = 0, int gameOver = 0);
	void moveForward(int);
	void moveBackwards(int);
	void switchPlayersPos(Player&);
	void passHome(int);
	void passStartPos();
	void backToStartLocation(); 
	void checkPos(int); 
	void setName(string s){ name = s; }
	void setLocation (int l) { location = location + l; }  
	
	string getName(){ return name; }
	int getNameLength(){return name.length();}
	int getLocation() { return location; } 
	int getGameOver() { return gameOver; } 
};

#endif 
