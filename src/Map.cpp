#include "Map.h"

Map::Map() : Map(1024, 1024) {}

Map::Map(int numOfTerritories, int numOfContinents) {
    this->territories = std::vector<std::shared_ptr<Territory>>(numOfTerritories);
    this->continents = std::vector<std::shared_ptr<Continent>>(numOfContinents);
}

Map::Map(Map *map) {
    this->territories = std::vector<std::shared_ptr<Territory>>(map->territories.size());
    this->continents = std::vector<std::shared_ptr<Continent>>(map->continents.size());

    for (std::shared_ptr<Territory> t : map->territories) {
        if (t == nullptr) continue;
        this->territories[t->id] = std::make_shared<Territory>(Territory(*t.get()));
    }

    for (std::shared_ptr<Continent> c : map->continents) {
        if (c == nullptr) continue;
        this->continents[c->id] = std::make_shared<Continent>(Continent(*c.get()));
    }
}

Map::~Map() {
    for (std::shared_ptr<Territory> t : this->territories) {
        t.reset();
    }

    for (std::shared_ptr<Continent> c : this->continents) {
        c.reset();
    }

    this->territories.clear();
    this->continents.clear();
}

std::string Map::to_string () {
    std::string s;

    for (std::shared_ptr<Continent> c : continents) {
        if (c == nullptr) continue;
        s += c->to_string();
        s += "\n";

        for (unsigned int index : c->territoryIDs) {
            s += this->getTerritory(index)->to_string();
            s += "\n";
        }
    }

    return s;
}

std::shared_ptr<Territory> Map::add(Territory territory) {
    if (territory.id == 0) {
        throw std::logic_error("ID's should start at 1");
    }

    if (this->getTerritory(territory.id) != nullptr) {
        throw std::logic_error("duplicate ID: territory already exists");
    }

    if (this->findTerritory(territory.name) != nullptr) {
        throw std::logic_error("duplicate name: territory already exists");
    }

    this->territories[territory.id] = std::shared_ptr<Territory> (new Territory(territory));

    std::shared_ptr<Continent> c = getContinent(territory.continentID);
    if (c != nullptr) {
        if (!std::count(c->territoryIDs.begin(), c->territoryIDs.end(), territory.id)) {
            c->territoryIDs.push_back(territory.id);
        }
    }

    return this->territories[territory.id];
}

std::shared_ptr<Continent> Map::add(Continent continent) {
    if (continent.id == 0) {
        throw std::logic_error("ID's should start at 1");
    }

    if (this->getContinent(continent.id) != nullptr) {
        throw std::logic_error("duplicate ID: continent already exists");
    }

    if (this->findContinent(continent.name) != nullptr) {
        throw std::logic_error("duplicate name: continent already exists");
    }

    this->continents[continent.id] = std::shared_ptr<Continent> (new Continent(continent));

    for (std::shared_ptr<Territory> t : this->territories) {
        if (t == nullptr) continue;
        if (t->continentID == continent.id) {
            if (!std::count(this->continents[continent.id]->territoryIDs.begin(), this->continents[continent.id]->territoryIDs.end(), t->id)) {
                this->continents[continent.id]->territoryIDs.push_back(t->id);
            }
        }
    }

    return this->continents[continent.id];
}

// TODO: finish validate function
// - Do a greedy connection matrix for Territories
// - Do a connection matrix for Continents based on the Territories connection matrix

bool Map::validate() {
    // resize territories and continents vectors

    for (int t = this->territories.size() - 1; t >= 0; t--) {
        if (this->territories[t] != nullptr) {
            this->territories.resize(t+1);
            this->territories.shrink_to_fit();
            break;
        }
    }

    for (int c = this->continents.size() - 1; c >= 0; c--) {
        if (this->continents[c] != nullptr) {
            this->continents.resize(c+1);
            this->continents.shrink_to_fit();
            break;
        }
    }

    std::cout << this->territories.size() << std::endl;
    std::cout << this->continents.size() << std::endl;

    std::vector<bool> checks = std::vector<bool>(this->territories.size());

    for (int i = 1; i < this->territories.size(); i++) {
        std::shared_ptr<Territory> t = this->territories[i];

        if (t == nullptr) {
            std::cerr << "there is an empty territory object" << std::endl;
            return false;
        }

        if (t->borders.size() > 0) {
            checks[t->id] = true;
        }

        for (unsigned int index : t->borders) {
            if (getTerritory(index) == nullptr) {
                std::cerr << t->to_string() << " points to " << index << " which is a nullptr" << std::endl;
                return false;
            }

            checks[index] = true;
        }

        if (getContinent(t->continentID) == nullptr) {
            std::cerr << t->to_string() << " doesn't have a valid continent. Either the continent is missing or the territory has a bad continentID." << std::endl;
        }
    }

    std::cout << this->territories.size() << std::endl;
    std::cout << checks.size() << std::endl;

    for (bool b : checks) {
        if (!b) {
            std::cerr << "there is a territory with no path to any other territory and that isn't accessed by any territory" << std::endl;
            return false;
        }
    }

    for (int i = 1; i < this->continents.size(); i++) {
        std::shared_ptr<Continent> c = this->continents[i];
        if (c == nullptr) {
            std::cerr << "there is an empty continent object at index " << i << std::endl;
            return false;
        }

        if (c->territoryIDs.size() == 0) {
            std::cerr << c->name << " has no territories" << std::endl;
            return false;
        }
    }

    std::cout << "map is valid" << std::endl;
    return true;
}

bool Map::link(std::shared_ptr<Territory> a, std::shared_ptr<Territory> b) {
    bool a2b = false;

    for (int i : a->borders) {
        if (i == b->id) {
            a2b = true;
            continue;
        }
    }

    if (!a2b) {
        a->borders.push_back(b->id);
    }

    return a2b;
}

bool Map::link(std::shared_ptr<Continent> a, std::shared_ptr<Continent> b) {
    bool a2b = false;

    for (int i : a->borders) {
        if (i == b->id) {
            a2b = true;
            continue;
        }
    }

    if (!a2b) {
        a->borders.push_back(b->id);
    }

    return a2b;
}

std::shared_ptr<Territory> Map::findTerritory(std::string name) {
    for (std::shared_ptr<Territory> t : this->territories) {
        if (t == nullptr) continue;
        if (t->name.compare(name) == 0) {
            return std::shared_ptr<Territory>(t);
        }
    }

    return nullptr;
}

std::shared_ptr<Territory> Map::getTerritory(unsigned int id) {
    if (id >= this->territories.size()) {
        throw std::out_of_range ("id is out of bounds");
    }

    return std::shared_ptr<Territory>(this->territories[id]);
}

std::shared_ptr<Continent> Map::findContinent(std::string name) {

    for (std::shared_ptr<Continent> c : this->continents) {
        if (c == nullptr) continue;
        if (c->name.compare(name) == 0) {
            std::cout << c->to_string() << std::endl;
            return std::shared_ptr<Continent>(c);
        }
    }

    return nullptr;
}

std::shared_ptr<Continent> Map::getContinent(unsigned int id) {
    if (id >= this->continents.size()) {
        throw std::out_of_range ("id is out of bounds");
    }

    return std::shared_ptr<Continent>(this->continents[id]);
}

void Map::operator=(Map map) {
    this->territories.clear();
    this->continents.clear();

    this->territories = map.territories;
    this->continents = map.continents;
}

Territory::Territory(unsigned int id, std::string name, unsigned int continentID) : Land(id, name) {
    this->units = 0;
    this->continentID = continentID;
}

std::string Territory::to_string() {
    char a[1024];

    sprintf_s(a, "Territory %d %s: %i unit(s), %li border(s)", this->id, this->name.c_str(), this->units, this->borders.size());

    return std::string(a);
}

Continent::Continent(int id, std::string name, unsigned int bonus) : Land(id, name) {
    this->bonus = bonus;
    this->territoryIDs = std::vector<unsigned int>();
}

std::string Continent::to_string() {
    char a[1024];

    sprintf_s(a, "Continent %d %s: %d bonus points, %li territories(s), %li border(s)", this->id, this->name.c_str(), this->bonus, this->territoryIDs.size(), this->borders.size());

    return std::string(a);
}

Land::Land(unsigned int id, std::string name) {
    this->id = id;
    this->name = name;

    this->borders = std::vector<unsigned int>();
}


std::ostream& operator<<(std::ostream& strm, Map &map) {
    return strm << map.to_string();
}