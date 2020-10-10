//
//  main.cpp
//  Player
//  Defines player class and its methods. A player has a list of territories, a hand of warzone cards and a list of orders. A player can call the issueOrder method to create a new order. Additionally the player can see all territories that are either to be attacked or to be defended with the toAttack ad toDefend methods.
//
//
//  Created by Lina Kretzschmar on 2020-09-27.
//

//

#include "Player.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <list>

using namespace std;
    
    //Default constructor
    Player::Player()
    {
        playerCount++;
        string tempName = "Player" + ::to_string(playerCount);
        this->name = tempName;
        this->playerID = playerCount;
        this->territoryList = list<shared_ptr<Territory>>();
        this->handOfCards = new Hand();
        this->orderList = new OrderList();
    }

    //Destructor which clears all parameters of pointer type
    Player::~Player()
    {
        territoryList.clear();//go through list and reset before clearing
        handOfCards->~Hand();
        orderList->~OrderList();
    };
    
    //Parameterized constructor
    Player::Player(string playerName)
    {
        playerCount++;
        this->name = playerName;
        this->playerID = playerCount;
        this->territoryList = list<shared_ptr<Territory>>();
        this->handOfCards = new Hand();
        this->orderList = new OrderList();
    };

    //Copy constructor enables deep copy of pointer attributes
    Player::Player(const Player& aPlayer)
    {
        this->name = aPlayer.name;
        this->playerID = playerCount;
        this->territoryList = aPlayer.territoryList;
        this->handOfCards = aPlayer.handOfCards;
        this->orderList = aPlayer.orderList;
    }

    //Assignment opertor
    Player& Player::operator=(const Player& aPlayer)
    {
        this->territoryList.clear();
        this->handOfCards->~Hand();
        this->orderList->~OrderList();

        this->name = aPlayer.name;
        this->playerID = playerCount;
        this->territoryList = aPlayer.territoryList;
        this->handOfCards = aPlayer.handOfCards;
        this->orderList = aPlayer.orderList;

        return *this;
    }

    //ToString method of Player
    string Player::to_string()
    {
        string str = "\n\nPlayer " + name + " has ID " + ::to_string(playerID) + " and owns:\n";
        str += "\nList of Territories:\n";
        str += printList(getTerritoryList());
        str += "\nHand of Warzone cards:\n";
        Hand * h = getHandOfCards();
        str+= h->to_string();
        str += "\nList of Orders:\n";
        OrderList * o = getOrderList();
        str += o->to_string();
        return str;
    }

    //Stream insertion operator
    ostream& operator<<(ostream& strm, Player &player)
    {
        return strm << player.to_string();
    }
    
    //********** Mutators and accessors *************//

    void Player::setName(string playerName)
    {
        this->name = playerName;
    };
    
    string Player::getName ()
    {
        return name;
    };

    int Player::getPlayerCount()
    {
        return playerCount;
    };

    unsigned int Player::getPlayerID()
    {
        return playerID;
    };

    //********** Order methods *************//

    OrderList * Player::getOrderList()
    {
        return orderList;
    }

    //Method issueOrder - creates a new order objects according to orderType and adds it to the players OrderList
    void Player::issueOrder(string orderType)
    {
        if(orderType == "Deploy")
        {
            shared_ptr<Order> order (new Deploy);
            this->orderList->addOrder(order);
        }
        else if(orderType == "Advance")
        {
            shared_ptr<Order> order (new Advance);
            this->orderList->addOrder(order);
        }
        else if(orderType == "Bomb")
        {
            shared_ptr<Order> order (new Bomb);
            this->orderList->addOrder(order);
        }
        else if(orderType == "Blockade")
        {
            shared_ptr<Order> order (new Blockade);
            this->orderList->addOrder(order);
        }
        else if(orderType == "Airlift")
        {
            shared_ptr<Order> order (new Airlift);
            this->orderList->addOrder(order);
        }
        else if(orderType == "Negotiate")
        {
            shared_ptr<Order> order (new Negotiate);
            this->orderList->addOrder(order);
        }
            else
        {
            cerr << "Invalid Order Type" << endl;
        }
    }

    //********* Card methods **********//

    Hand * Player::getHandOfCards()
    {
        return handOfCards;
    }

    //********* Territory methods **********//

    //Returning TerritoryList
    list<shared_ptr<Territory>> Player::getTerritoryList()
    {
        return territoryList;
    }

    //Adding a territory to the TerritoryList and assigning the ownerId to the playerID
    void Player::addTerritory(shared_ptr<Territory> newTerritoryPtr)
    {
        territoryList.push_back(newTerritoryPtr);
//        cout << "In function owner id 1: " << newTerritoryPtr->ownerID << "\n";
        newTerritoryPtr->ownerID = playerID;
//        cout << "In function owner id 2: " << newTerritoryPtr->ownerID << "\n";
    }

    //Method toAttack - returns list of pointers to territory objects having adjacent territory not owned by the player
    list<shared_ptr<Territory>> Player::toAttack(Map aMap)
    {
        list<shared_ptr<Territory>> copyList;
        list<shared_ptr<Territory>>::iterator i = territoryList.begin();
        
        
        for(i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
        {
            vector <unsigned int> territoryIDs = (*i)->borders;
            for(auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
            {
                if(aMap.getTerritory(*iD)->ownerID != playerID)
                {
                    copyList.push_back(aMap.getTerritory(*iD));
                }
            }
        }
         return copyList;
    }
        
    //Method toDefend - returns list of pointers to territory objects having adjacent territory owned by the player
    list<shared_ptr<Territory>> Player::toDefend(Map aMap)
    {
        list<shared_ptr<Territory>> copyList;
        list<shared_ptr<Territory>>::iterator i = territoryList.begin();


        for(i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
        {
            vector <unsigned int> territoryIDs = (*i)->borders;
            for(auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
            {
//                cout << "Owner ID: " << aMap.getTerritory(*iD)->ownerID << "\n";
//                cout << "TerritoryID: " << *(iD) << "\n";
//                cout << "Player Id: " << playerID << "\n";
                if(aMap.getTerritory(*iD)->ownerID == playerID)
                {
                    copyList.push_back(aMap.getTerritory(*iD));
                }
            }
        }
        return copyList;
    }

    //method that takes any list or pointers as input and returns it as string
    string Player::printList(list<shared_ptr<Territory>> aList)
    {
        string tList = "";
        list<shared_ptr<Territory>>::iterator it = aList.begin();
        for(it = aList.begin(); it != aList.end(); advance(it, 1))
        {
            shared_ptr<Territory> t = *it;
            tList += t->to_string() + "\n";
        }
        return tList;
    }
    
    


