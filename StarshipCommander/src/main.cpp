//#include "../include/colors/colors.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Ship {
    public:
        string model;
        int price;
};

class PlayerShip {
    public:
        string shipname;
        string location = "planet";
        Planet* currentPlanet;
        SolarSystem* currentSolarSystem;
        Galaxy* currentGalaxy;
        float health = 100;
        float maxHealth = 100;
        float shield = 100;
        float maxShield = 100;
        float credits = 0;
        Ship ship;
        vector<string> inventory;
};


class Traveler {
    string name;
    string race;
    Ship ship;
};

//has access to galactic market
class SpaceStation {
    public:
        string name;
        vector<Traveler> travelers;
};

class Planet {
    public:
        string name;
        SolarSystem* solarSystem;
};;

class SolarSystem {
    public:
        string name;
        SpaceStation spaceStation;
        vector<Planet> planets;
};

class Galaxy {
    public:
        vector<SolarSystem> solarSystems;
};

int rand_range(int min, int max) {return rand() % (max - min + 1) + min;}
int main() {
    string choosename;
    cout << "Enter your ship's name: ";
    getline(cin, choosename);

    PlayerShip playership;
    playership.shipname = choosename;
    cout << "Hey Captain, the " << choosename << " is currently located in " << playership.location << ", what's our next move?" << endl;
    
    string a;
    cin >> a;
    return 0;
}