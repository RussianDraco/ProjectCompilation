#include "../include/colors/colors.hpp"
#include "../include/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include <vector>
using namespace std;
using json = nlohmann::json;
using string = std::string;

const int GALAXY_COUNT = 5;
const int STAR_SYSTEM_COUNT = 10;
const int PLANET_COUNT = 10;

//g++ src/main.cpp -o build/StarshipCommander
/*
Ideas:
- Travel between planets, star systems, galaxies
- Buy/sell resources
- Upgrade ship
- Hire crew
- Fight enemies
    - parry/dodge through reaction speed test
- Random events
- Quests
- Anomalies
- Space stations
- Travelers
- Planets with different properties
*/

class Game;

class NamingManager {
    public:
        vector<string> genericWords;
        string greekNumbers[10] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", "Eta", "Theta", "Iota", "Kappa"};

        int rand_range(int min, int max) {return rand() % (max - min + 1) + min;}
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
            return greekNumbers[rand_range(0, 9)] + " " + genericWords[rand_range(0, genericWords.size() - 1)];
        }
        string generateStarSystemName(int galaxyindex, int systemindex) {
            return "SYS" + to_string(galaxyindex+1) + "-" + to_string(systemindex+1) + " " + genericWords[rand_range(0, genericWords.size() - 1)];
        }
        string generatePlanetName(string systemname, int planetindex) { //might change this to consider planetary properties
            string systemNamePart = systemname.substr(systemname.find(' ') + 1);
            return systemNamePart + " " + int_to_roman(planetindex+1);
        }
        
        void init() {
            ifstream f("../jsons/spaceNames.json");
            json data = json::parse(f);
            for (auto& element : data["names"]) {
                genericWords.push_back(element);
            }
        }
};

class Action {
    public:
        string description;
        function<void(string)> execute;
};
class CommandManager {
    public:
        map<string, Action> commands;
        
        void addCommand(string name, string description, function<void(string)> execute) {
            Action action;
            action.description = description;
            action.execute = execute;
            commands[name] = action;
        }
        
        void executeCommand(string name, string arg) {
            if (commands.find(name) != commands.end()) {
                Action action = commands[name];
                action.execute(arg);
            } else {
                std::cout << "Unknown command." << endl;
            }
        }

        void printOptions() {
            cout << "--------------------Actions--------------------" << endl;
            for (auto& command : commands) {
                cout << "[" << command.first << "] " << command.second.description << endl;
            }
        }
};

class Ship {
    public:
        std::string model;
        int price;
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
        //want to add anomalies, resources, travelers, etc.
    Planet(string name) {
        this->name = name;
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
class PlayerShip {
    public:
        string shipname;
        SpaceStation* currentSpaceStation;
        Planet* currentPlanet;
        StarSystem* currentStarSystem;
        Galaxy* currentGalaxy;
        float warpFuel = 100;
        float health = 100;
        float maxHealth = 100;
        float shield = 100;
        float maxShield = 100;
        float credits = 0;
        Ship ship;
        vector<string> inventory;
};
class Game { //holds pointers torwards to all important classes to allow command running to access everything
    public:
        PlayerShip* playerShip;
        CommandManager* commandManager;
        NamingManager* namingManager;
        vector<Galaxy>* galaxies;

        vector<string> galaxyNames;
};

//ACTIONS START

void warpToGalaxy(Game& game, string arg) {
    cout << "Attempting warp to galaxy: " << arg << "..." << endl;
    for (auto galaxy : *game.galaxies) {
        if (galaxy.name == arg) {
            game.playerShip->currentGalaxy = &galaxy;
            game.playerShip->currentStarSystem = &galaxy.starSystems[0];
            game.playerShip->currentPlanet = nullptr;
            game.playerShip->currentSpaceStation = nullptr;
            cout << "Warp successful!" << endl;
            cout << "Arrived in the " << colors::yellow << game.playerShip->currentStarSystem->name << colors::reset << " star system in the " << colors::magenta << game.playerShip->currentGalaxy->name << colors::reset << " galaxy" << endl;
            return;
        }
    }
    cout << "Galaxy not found" << endl;
}
void flyToStarSystem(Game& game, string arg) {
    cout << "Attempting to fly to star system: " << arg << "..." << endl;
    for (auto starSystem : game.playerShip->currentGalaxy->starSystems) {
        if (starSystem.name == arg) {
            game.playerShip->currentStarSystem = &starSystem;
            game.playerShip->currentPlanet = &starSystem.planets[0];
            game.playerShip->currentSpaceStation = nullptr;
            cout << (*game.playerShip).shipname << " just flew into the " << colors::yellow << game.playerShip->currentStarSystem->name << colors::reset << " star system" << endl;
            return;
        }
    }
    cout << "Star system not found" << endl;
}
void checkNavigator(Game& game, string arg) {
    cout << "---NAVIGATOR---" << endl;
    cout << "--Current Location--" << endl;
    cout << "Galaxy: " << colors::magenta << game.playerShip->currentGalaxy->name << colors::reset << endl;
    cout << "Star System: " << colors::yellow << game.playerShip->currentStarSystem->name << colors::reset << endl;
    if (game.playerShip->currentPlanet != nullptr) {
        cout << "Planet: " << colors::grey << game.playerShip->currentPlanet->name << colors::reset << endl;
    }
    if (game.playerShip->currentSpaceStation != nullptr) {
        cout << "Space Station: " << colors::bright_white << game.playerShip->currentSpaceStation->name << colors::reset << endl;
    }

    cout << "--Nearby Galaxies--" << endl;
    int galaxyIndex = 0;
    for (int i = 0; i < GALAXY_COUNT; i++) {
        if (game.galaxyNames[i] == game.playerShip->currentGalaxy->name) {
            galaxyIndex = i;
            break;
        }
    }
    cout << "Left: " << colors::magenta << game.galaxyNames[(galaxyIndex - 1 + GALAXY_COUNT) % GALAXY_COUNT] << colors::reset << endl;
    cout << "Right: " << colors::magenta << game.galaxyNames[(galaxyIndex + 1) % GALAXY_COUNT] << colors::reset << endl;

    cout << "--Nearby Star Systems--" << endl;
    int starSystemIndex = 0;
    for (int i = 0; i < STAR_SYSTEM_COUNT; i++) {
        if (game.playerShip->currentGalaxy->starSystems[i].name == game.playerShip->currentStarSystem->name) {
            starSystemIndex = i;
            break;
        }
    }
    cout << "2 Left: " << colors::yellow << game.playerShip->currentGalaxy->starSystems[(starSystemIndex - 2 + STAR_SYSTEM_COUNT) % STAR_SYSTEM_COUNT].name << colors::reset << endl;
    cout << "1 Left: " << colors::yellow << game.playerShip->currentGalaxy->starSystems[(starSystemIndex - 1 + STAR_SYSTEM_COUNT) % STAR_SYSTEM_COUNT].name << colors::reset << endl;
    cout << "1 Right: " << colors::yellow << game.playerShip->currentGalaxy->starSystems[(starSystemIndex + 1) % STAR_SYSTEM_COUNT].name << colors::reset << endl;
    cout << "2 Right: " << colors::yellow << game.playerShip->currentGalaxy->starSystems[(starSystemIndex + 2) % STAR_SYSTEM_COUNT].name << colors::reset << endl;

    cout << "--Planets in Star System--" << endl;
    for (auto planet : game.playerShip->currentStarSystem->planets) {
        cout << planet.name << colors::grey << colors::reset << endl;
    }

    if (!game.playerShip->currentStarSystem->spaceStation.name.empty()) {
        cout << "--Star System's Space Station--" << endl;
        cout << game.playerShip->currentStarSystem->spaceStation.name << endl;
    }
}


//END

Galaxy generateGalaxy(NamingManager* namingManager) {
    Galaxy galaxy;
    galaxy.name = namingManager->generateGalaxyName();
    for (int i = 0; i < STAR_SYSTEM_COUNT; i++) {
        StarSystem starSystem;
        starSystem.name = namingManager->generateStarSystemName(0, i);
        for (int j = 0; j < PLANET_COUNT; j++) {
            starSystem.planets.push_back(Planet(namingManager->generatePlanetName(starSystem.name, j)));
        }
        galaxy.starSystems.push_back(starSystem);
    }
    return galaxy;
}
void passInput(string input, CommandManager& commandManager) {
    size_t spacePos = input.find(' ');
    string commandName = input.substr(0, spacePos);
    string arg = spacePos != string::npos ? input.substr(spacePos + 1) : "";
    commandManager.executeCommand(commandName, arg);
}
int rand_range(int min, int max) {return rand() % (max - min + 1) + min;}
int main() {
    string choosename;
    cout << "Enter your ship's name: ";
    getline(cin, choosename);

    hash<string> hash_fn;
    size_t hash = hash_fn(choosename);
    srand(static_cast<unsigned int>(hash));

    NamingManager namingManager;
    CommandManager commandManager;
    vector<Galaxy> galaxies;
    PlayerShip playership;
    Game game;
    game.playerShip = &playership;
    game.commandManager = &commandManager;
    game.namingManager = &namingManager;
    game.galaxies = &galaxies;

    namingManager.init();

    //commands are added here
    commandManager.addCommand("warp", "Warp to a new galaxy", [&](string arg) { warpToGalaxy(game, arg); });
    commandManager.addCommand("navigator", "Check the navigator for information", [&](string arg) { checkNavigator(game, arg); });
    commandManager.addCommand("starfly", "Fly to a new star system", [&](string arg) { flyToStarSystem(game, arg); });


    for (int g = 0; g < GALAXY_COUNT; g++) {galaxies.push_back(generateGalaxy(&namingManager));}
    for (auto galaxy : galaxies) {game.galaxyNames.push_back(galaxy.name);}

    playership.currentGalaxy = &galaxies[0]; playership.currentStarSystem = &playership.currentGalaxy->starSystems[0]; playership.currentPlanet = &playership.currentStarSystem->planets[0];
    playership.shipname = choosename;
    string shipsrc = choosename + ": ";

    cout << colors::bright_white << shipsrc << colors::reset << "Beep Beep!" << endl;
    cout << colors::bright_white << shipsrc << colors::reset << "Hello Captain, this is your ship's AI" << endl;
    cout << colors::bright_white << shipsrc << colors::reset << "We are currently on " << playership.currentPlanet->name << " in the " << playership.currentStarSystem->name << " star system (Galaxy: " << playership.currentGalaxy->name << ")" << endl;
    
    while (true) { //main game loop
        commandManager.printOptions();
        string input;
        cout << "Enter command: ";
        getline(cin, input);
        passInput(input, commandManager);
    }

    string a;cin >> a;
    return 0;
}