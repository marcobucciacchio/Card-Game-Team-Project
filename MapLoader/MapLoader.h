#pragma once

#include "../Map/Map.h"
#include <fstream>

using namespace std;

class MapLoader 
{
public:

	MapLoader();
	MapLoader(const MapLoader&);
	~MapLoader();
	MapLoader& operator=(const MapLoader&);
	friend ostream& operator<<(std::ostream& strm, MapLoader& mapLoader);

	shared_ptr<Map> createMap(string fileName, string pathToDirectory = "");
};

class ConquestFileReader
{

public:
	ConquestFileReader();
	ConquestFileReader(const ConquestFileReader&);
	~ConquestFileReader();
	ConquestFileReader& operator=(const ConquestFileReader&);
	friend ostream& operator<<(ostream& strm, ConquestFileReader& conquestFileReader);

	shared_ptr<Map> createMap(string fileName, string pathToDirectory = "");
};
