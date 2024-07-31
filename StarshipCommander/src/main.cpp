#include "../include/colors/colors.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class PlayerShip {
    public:
        string name;
        string location;
        float health;
        float maxHealth;
        float shield;
        float maxShield;
        float gold;
        vector<string> inventory;
    
    PlayerShip(string name) {
        this->name = name;
        location = "Earth";
        health = 100;
        maxHealth = 100;
        shield = 100;
        maxShield = 100;
        gold = 0;
    }
};

int rand_range(int min, int max) {return rand() % (max - min + 1) + min;}
int main() {
    string name;
    cout << "Enter your ship's name: ";
    cin >> name;

    PlayerShip playership(name);
    cout << "Hey Captain, the " << name << " is currently located in " << playership.location << ", what's our next move?" << endl;
    
    string a;
    cin >> a;
    return 0;
}