#pragma once
#include "player/Player.h"
#include "MapLoader/MapLoader.h"
#include <iostream>


using namespace std;

struct GameEngine
{
	GameEngine();
	~GameEngine();
	void gameStart();
	int getNumOfPlayers();
	Deck getDeckCards();
	vector<Player> getPlayersList();
	bool getObserverStatus();
	void setObserverStatus(bool status);
	//bool mainGameLoop();

	GameEngine& operator= (const GameEngine&);
	friend ostream& operator<<(ostream& strm, GameEngine& gameEngine);
	//shared_ptr<GameEngine> createEngine();

private:
//	bool startupPhase();	//Part 2
//	bool reinforcementsPhase();
//	bool issueOrdersPhase();
//	bool executeOrdersPhase();

	
	int numOfPlayers;
	vector<Player> players;
	bool activateObservers;
	Map gameMap;
	bool isMapInDirectory(string fileName);
	bool equals(const string& a, const string& b);
	void setNumOfPlayers();
	bool Observers();
	string selectMap();
	Deck deckCards;
};
