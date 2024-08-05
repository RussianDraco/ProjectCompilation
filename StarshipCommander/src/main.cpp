#include "../include/colors/colors.hpp"
#include "../include/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <random>
using namespace std;
using json = nlohmann::json;
using string = std::string;

const int GALAXY_COUNT = 5;
const int STAR_SYSTEM_COUNT = 10;
const int PLANET_COUNT = 10;

//g++ src/main.cpp -o build/StarshipCommander
/*
Lore idea:
-  You used to be a great space captain but after a terrible accident you were left as a digital consciousness that can only control your ship 
   and communicate with the outside world through robots.


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
class PlanetProperties {
    public:
        int size; //
        int temperature;
        int atmosphere;
        vector<pair<string, float>> resources;
        vector<string> anomalies;
        vector<string> buildings;
    
    PlanetProperties(int id) {
        seed_seq seed{ id };
        mt19937 generator(seed);
        uniform_int_distribution<int> sizeDist(1, 10);
        uniform_int_distribution<int> temperatureDist(-150, 150); //celsius
        uniform_int_distribution<int> atmosphereDist(0, 4); // Type of atmosphere, range: 0 - 4 (0: None, 1: Thin, 2: Normal, 3: Thick, 4: Toxic)
        //!!NEED TO ADD RESOURCES, ANOMALIES, BUILDINGS

        size = sizeDist(generator);
        temperature = temperatureDist(generator);
        atmosphere = atmosphereDist(generator);
        
    }
};
class PlanetManager { //to use less memory planets will be given an id and their properties can be regenerated
    public:
        PlanetProperties retrievePlanetProperties(int id) {
            return PlanetProperties(id);
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
        float health = 100;
        float maxHealth = 100;
        float shield = 100;
        float maxShield = 100;
        float credits = 0;
        Ship ship;
        vector<string> inventory;
        vector<string> technologies;
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
    for (int i = 0; i < GALAXY_COUNT; i++) {
        if ((*game.galaxies)[i].name == arg) {
            game.playerShip->currentGalaxy = &(*game.galaxies)[i];
            game.playerShip->currentStarSystem = &((game.playerShip->currentGalaxy->starSystems)[0]);
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
    for (int i = 0; i < STAR_SYSTEM_COUNT; i++) {
        if (((*game.playerShip->currentGalaxy).starSystems)[i].name == arg) {
            game.playerShip->currentStarSystem = &((*game.playerShip->currentGalaxy).starSystems)[i];
            game.playerShip->currentPlanet = nullptr;
            game.playerShip->currentSpaceStation = nullptr;
            cout << (*game.playerShip).shipname << " just flew into the " << colors::yellow << game.playerShip->currentStarSystem->name << colors::reset << " star system" << endl;
            return;
        }
    }
    cout << "Star system not found" << endl;
}
void flyToPlanet(Game& game, string arg) {
    cout << "Attempting to fly to planet: " << arg << "..." << endl;
    for (int i = 0; i < PLANET_COUNT; i++) {
        if (((*game.playerShip->currentStarSystem).planets)[i].name == arg) {
            game.playerShip->currentPlanet = &((*game.playerShip->currentStarSystem).planets)[i];
            game.playerShip->currentSpaceStation = nullptr;
            cout << (*game.playerShip).shipname << " just landed on " << colors::blue << game.playerShip->currentPlanet->name << colors::reset << endl;
            return;
        }
    }
    cout << "Planet not found" << endl;
}
void visitSpaceStation(Game& game, string arg) {
    if (game.playerShip->currentStarSystem->spaceStation.name.empty()) {
        cout << "No space station in this star system" << endl;
        return;
    }
    cout << "Flying to local space station " << game.playerShip->currentStarSystem->spaceStation.name << "..." << endl;
    game.playerShip->currentPlanet = nullptr;
    game.playerShip->currentSpaceStation = &game.playerShip->currentStarSystem->spaceStation;
}
void checkNavigator(Game& game, string arg) {
    cout << "---NAVIGATOR---" << endl;
    cout << "--Current Location--" << endl;
    cout << "Galaxy: " << colors::magenta << game.playerShip->currentGalaxy->name << colors::reset << endl;
    cout << "Star System: " << colors::yellow << game.playerShip->currentStarSystem->name << colors::reset << endl;
    if (game.playerShip->currentPlanet != nullptr) {
        cout << "Planet: " << colors::blue << game.playerShip->currentPlanet->name << colors::reset << endl;
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
        cout << colors::blue << planet.name << colors::reset << endl;
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
        if (rand() % 2 == 0) {
            SpaceStation spaceStation;
            spaceStation.name = starSystem.name + " SS";
            starSystem.spaceStation = spaceStation;
        }
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
    commandManager.addCommand("warp", "Warp to a new galaxy using warp cells (takes galaxy name)", [&](string arg) { warpToGalaxy(game, arg); });
    commandManager.addCommand("navigator", "Check the navigator for information", [&](string arg) { checkNavigator(game, arg); });
    commandManager.addCommand("starfly", "Fly to a new star system (takes star system name)", [&](string arg) { flyToStarSystem(game, arg); });
    commandManager.addCommand("fly", "Fly to a new planet (takes planet name)", [&](string arg) { flyToPlanet(game, arg); });
    commandManager.addCommand("dock", "Visit the space station in the current star system", [&](string arg) { visitSpaceStation(game, arg); });

    for (int g = 0; g < GALAXY_COUNT; g++) {galaxies.push_back(generateGalaxy(&namingManager));}
    for (auto galaxy : galaxies) {game.galaxyNames.push_back(galaxy.name);}

    playership.currentGalaxy = &galaxies[0]; playership.currentStarSystem = &playership.currentGalaxy->starSystems[0]; playership.currentPlanet = &playership.currentStarSystem->planets[0]; playership.currentSpaceStation = nullptr;
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