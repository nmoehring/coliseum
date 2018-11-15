//Coliseum.cpp
//Chapter 8 of C++ Through Game Programming
//Week 12 of Beginning C++ Programming at Butler Community College
//Program by Nathaniel Moehring
//Villain names taken from Dexter's Laboratory

#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

//Globals
//dmg_type_end useful for selecting a random damage type
enum damage_type {FIRE, WATER, EARTH, WIND, dmg_type_end};
vector<string> villainNames = {"Mental Mouse", "Math Magician", "Silver Spooner",
"Heck Hound", "Disgruntled Postman", "Comrade Red", "Bertha the Barbarian"};

//Class Declarations
class Villain {
public:
	Villain();
	string getName();
	damage_type getDmgType();
	string getElement();
	int giveDamage();
	bool takeDamage(int);
	void upgrade();
	bool dodge();
private:
	string m_name;
	int m_health;
	int m_attack;
	int m_evasion;
	damage_type m_dmgType;
};

//Function Declarations
string selectRandomVillain();
damage_type selectRandomDmgType();
Villain* fight(vector<Villain*>);
bool strike(Villain*,Villain*,int);
int rollDice(int sides=100);
void sleep(int);

int main() {
	srand(0);

	cout << "Welcome to the Villain Coliseum, where the most vile villains";
	cout << " from around the world will be given random elemental powers and ";
	cout << "forced to fight each other to the death !!!" << endl;

	//I originally intended to use vectors of references, which I now know is
	// impossible.
	Villain fighter1, fighter2, fighter3, fighter4, fighter5;
	vector<Villain*> round1_fight1 = {&fighter1, &fighter2};
	vector<Villain*> round1_fight2 = {&fighter3, &fighter4};
	vector<Villain*> round2;
	vector<Villain*> finalRound;

	//Make the fifth fighter the current champion
	fighter5.upgrade();
	finalRound.push_back(&fighter5);

	sleep(2000);
	cout << "The first fight of round 1 is about to begin." << endl;
	round2.push_back(fight(round1_fight1));
	cout << endl;

	sleep(2000);
	cout << "The second fight of round 1 is about to begin." << endl;
	round2.push_back(fight(round1_fight2));
	cout << endl;

	sleep(2000);
	cout << "The second round is starting now." << endl;
	finalRound.push_back(fight(round2));
	cout << endl;

	sleep(2000);
	cout << "The final round is starting now." << endl;
	Villain* winner = fight(finalRound);
	cout << endl;

	sleep(2000);
	if (winner == &fighter5) {
		cout << winner->getName() << " remains the champion!" << endl;
	} else {
		cout << winner->getName() << " is the new champion!" << endl;
	}

	sleep(2000);
	return 0;
}

//Class member function definitions
Villain::Villain() {
	m_health = (rand() % 50) + 20;
	m_attack = (rand() % 20) + 5;
	m_dmgType = selectRandomDmgType();
	m_name = selectRandomVillain();

	//Give different attributes based on damage type
	switch (m_dmgType) {
		case WIND:
			m_evasion = 40;
			break;
		case EARTH:
			m_evasion = 15;
			m_health += 15;
			break;
		default:
			m_evasion = 25;
	}
}

string Villain::getName() {
	return m_name;
}

int Villain::giveDamage() {
	return m_attack;
}

bool Villain::takeDamage(int dmg) {
	cout << m_name << " took " << dmg << " damage!" << endl;
	m_health -= dmg;
	if (m_health <= 0) {return true;} else {return false;};
}

bool Villain::dodge() {
	int roll = rollDice();
	if (roll <= m_evasion) {
		return true;
	} else {
		return false;
	}
}

void Villain::upgrade() {
	m_health += 20;
	m_attack += 5;
	m_evasion += 5;
}

//Returns the actual damage_type
damage_type Villain::getDmgType() {
	return m_dmgType;
}

//Returns the string representation of the damage_type
string Villain::getElement() {
	switch (m_dmgType) {
		case FIRE: return "fire";
		case WATER: return "water";
		case EARTH: return "earth";
		case WIND: return "wind";
	}
}

//Function Definitions
string selectRandomVillain() {
	//This is inefficient but there are only 5 vector elements
	random_shuffle(villainNames.begin(), villainNames.begin());
	string name = villainNames.back();
	villainNames.pop_back();
	return name;
}

damage_type selectRandomDmgType() {
	return static_cast<damage_type>(rand() % dmg_type_end);
}

int rollDice(int sides) {
	return (rand() % sides) + 1;
}

Villain* fight(vector<Villain*> fighters) {
	//Shuffle fighters, whoever is fighters[0] goes first
	random_shuffle(fighters.begin(), fighters.end());

	//"Matt [water] and Ben [fire] step into the arena!"
	cout << fighters[0]->getName() << " [" << fighters[0]->getElement() << "]";
	cout <<  " and " << fighters[1]->getName() << " [" << fighters[1]->getElement();
	cout << "] step into the arena!" << endl;

	sleep(1000);

	int dmg_modifier = 0;
	//dmg_modifier set up so that it adds to fighters[0] and subtracts
	// from fighters[1]

	//The enumeration of the elements is set so that the next element (+1) beats
	// the previous element
	damage_type element0 = fighters[0]->getDmgType();
	damage_type element1 = fighters[1]->getDmgType();
	if (element1 - element0 == 1 || element1 - element0 == -4) {
		dmg_modifier = -20;
	} else if (element0 - element1 == 1 || element0 - element1 == -4) {
		dmg_modifier = 20;
	}

	while(true) {
		//strike() returns true if the opponent died
		sleep(500);
		if (strike(fighters[0],fighters[1],dmg_modifier)) {
			return fighters[0];
		}
		sleep(500);
		if (strike(fighters[1],fighters[0],0-dmg_modifier)) {
			return fighters[1];
		};

	}
}

bool strike(Villain* attacker, Villain* victim, int dmg_modifier) {
	cout << attacker->getName() << " attacks..." << endl;
	sleep(500);
	if (victim->dodge()) {
		cout << "...and misses!" << endl;
		return false;
	} else {
		int netDmg = attacker->giveDamage() + dmg_modifier;
		cout << "...and gives " << netDmg << " damage!" << endl;
		return victim->takeDamage(netDmg);
	}
}

void sleep(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}
