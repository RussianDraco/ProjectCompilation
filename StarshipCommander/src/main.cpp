#include "../include/colors/colors.hpp"
#include "../include/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using json = nlohmann::json;

class NamingManager {
    public:
        vector<string> galaxyNames;
        vector<string> starSystemNames;
        string greekNumbers[10] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", "Eta", "Theta", "Iota", "Kappa"};

        string int_to_roman(int a) {
            string ans;
            string M[] = {"","M","MM","MMM"};
            string C[] = {"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"};
            string X[] = {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"};
            string I[] = {"","I","II","III","IV","V","VI","VII","VIII","IX"};
            ans = M[a/1000]+C[(a%1000)/100]+X[(a%100)/10]+I[(a%10)];
            return ans;
        }
        string generateGalaxyName() {
            return greekNumbers[rand_range(0, 9)] + " " + galaxyNames[rand_range(0, galaxyNames.size() - 1)];
        }
        string generateStarSystemName(int galaxyindex, int systemindex) {
            return "SYS" + to_string(galaxyindex+1) + "-" + to_string(systemindex+1) + " " + starSystemNames[rand_range(0, starSystemNames.size() - 1)];
        }
        string generatePlanetName(int planetindex) { //might change this to consider planetary properties
            return 
        }
        

};

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
        StarSystem* currentStarSystem;
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
        StarSystem* starSystem;
        //want to add anomalies, resources, travelers, etc.
    Planet(string name, StarSystem* starSystem) {
        this->name = name;
        this->starSystem = starSystem;
    }
};;

class StarSystem {
    public:
        string name;
        SpaceStation spaceStation;
        vector<Planet> planets;
};

class Galaxy {
    public:
        string name;
        vector<StarSystem> starSystems;
};

Galaxy genesisGalaxy(NamingManager* namingManager) {
    Galaxy galaxy;
    for (int i = 0; i < 10; i++) {
        StarSystem starSystem;
        starSystem.name = namingManager->generateStarSystemName(0, i);
        for (int j = 0; j < 10; j++) {
            starSystem.planets.push_back(Planet(namingManager->generatePlanetName(), &starSystem));
        }
        galaxy.starSystems.push_back(starSystem);
    }
    return galaxy;
}
int rand_range(int min, int max) {return rand() % (max - min + 1) + min;}
int main() {
    vector<Galaxy> galaxies;
    galaxies.push_back(genesisGalaxy());

    string choosename;
    cout << "Enter your ship's name: ";
    getline(cin, choosename);

    PlayerShip playership;

    playership.currentGalaxy = &galaxies[0];
    playership.currentStarSystem = &playership.currentGalaxy->starSystems[0];
    playership.currentPlanet = &playership.currentStarSystem->planets[0];

    playership.shipname = choosename;
    cout << "Hey Captain!" << endl;
    cout << "The " << choosename << " are currently on " << playership.currentPlanet->name << " in the " << playership.currentStarSystem->name << " star system (Galaxy: " << playership.currentGalaxy->name << ")" << endl;
    
    string a;
    cin >> a;
    return 0;
}