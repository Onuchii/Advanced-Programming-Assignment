/**
 * @file characters.hpp
 * @brief Character class hierarchy and race implementations
 *
 * This file contains the abstract base Character class and all race-specific
 * implementations (Human, Elf, Dwarf, Hobbit, Orc). Characters can equip items
 * and have race-specific combat behaviors. The Character class uses polymorphism
 * through the pure virtual printStats() method.
 *
 * @author [Steffy Pereppadan Ignatious]
 */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "items.hpp"
using namespace std;

/**
 * @class Character
 * @brief Abstract base class for all character types in the game
 *
 * This class provides the base functionality for all characters including
 * stats, inventory management, and combat calculations. It uses polymorphism
 * through the pure virtual printStats() method. Characters can carry an
 * unbounded amount of items (limited by strength) using dynamic data structures.
 */
class Character {
public:
    string name;                    ///< Character's name
    string race;                    ///< Character's race (Human, Elf, Dwarf, Hobbit, Orc)
    int attack;                     ///< Base attack value
    float attack_chance;            ///< Probability of successful attack (0.0 to 1.0)
    int defence;                    ///< Base defense value
    float defence_chance;           ///< Probability of successful defense (0.0 to 1.0)
    int health;                     ///< Current health points
    int strength;                   ///< Strength (affects carrying capacity)
    shared_ptr<Weapon> weapon;      ///< Equipped weapon (nullptr if none)
    shared_ptr<Armour> armor;       ///< Equipped armor (nullptr if none)
    shared_ptr<Shield> shield;      ///< Equipped shield (nullptr if none)
    vector<shared_ptr<Ring>> ring;  ///< Vector of equipped rings (can have multiple)
    vector<shared_ptr<Item>> inventory; ///< Inventory of all items carried

    /**
     * @brief Constructor to initialize character attributes
     *
     * @param n Character's name
     * @param r Character's race
     * @param a Base attack value
     * @param ac Attack chance (0.0 to 1.0)
     * @param d Base defense value
     * @param dc Defense chance (0.0 to 1.0)
     * @param h Base health points
     * @param s Base strength value
     */
    Character(string n, string r, int a, float ac, int d, float dc, int h, int s) {
        name = n;
        race = r;
        attack = a;
        attack_chance = ac;
        defence = d;
        defence_chance = dc;
        health = h;
        strength = s;
    }

    /**
     * @brief Pure virtual function to print character statistics
     *
     * Must be implemented by derived classes to display race-specific stats.
     */
    virtual void printStats() = 0;

    shared_ptr<Item> item;  ///< Temporary item pointer (used during pickup)

    /**
     * @brief Pick up an item from the ground and equip it if possible
     *
     * Pseudo-code:
     * 1. Calculate current total weight of equipped items
     * 2. IF (current weight + item weight > strength):
     *    a. Display "Item too heavy" message
     *    b. RETURN false
     * 3. IF item is a Weapon:
     *    a. Equip weapon (replace current weapon)
     *    b. Display weapon info
     *    c. RETURN true
     * 4. IF item is Armour:
     *    a. Equip armor (replace current armor)
     *    b. Add to inventory
     *    c. Display armor info
     *    d. RETURN true
     * 5. IF item is Shield:
     *    a. Equip shield (replace current shield)
     *    b. Add to inventory
     *    c. Display shield info
     *    d. RETURN true
     * 6. IF item is Ring:
     *    a. Add ring to ring vector (can have multiple)
     *    b. Add to inventory
     *    c. Display ring info
     *    d. RETURN true
     * 7. Display "Item not recognized" message
     * 8. RETURN false
     *
     * @param item Pointer to item to pick up
     * @return true if item was successfully picked up, false otherwise
     */
    bool pickUp(shared_ptr<Item> item) {
        if (getCurrentWeight() + item->weight > strength) {
            cout << "Item too heavy" << endl;
            return false;
        }
        if (auto w = dynamic_pointer_cast<Weapon>(item)) {
            weapon = w;
            w->print();
            return true;
        }
        if (auto a = dynamic_pointer_cast<Armour>(item)) {
            armor = a;
            a->print();
            inventory.push_back(item);
            return true;
        }
        if (auto s = dynamic_pointer_cast<Shield>(item)) {
            shield = s;
            s->print();
            inventory.push_back(item);
            return true;
        }
        if (auto r = dynamic_pointer_cast<Ring>(item)) {
            ring.push_back(r);
            r->print();
            inventory.push_back(item);
            return true;
        }
        cout << "Item not recognized." << endl;
        return false;
    }

    /**
     * @brief Print character's inventory (equipped items)
     *
     * Pseudo-code:
     * 1. Display "Equipped Items:" header
     * 2. IF weapon equipped: display weapon info, ELSE display "Weapon: None"
     * 3. IF armor equipped: display armor info, ELSE display "Armour: None"
     * 4. IF shield equipped: display shield info, ELSE display "Shield: None"
     * 5. IF rings vector is empty: display "Rings: None"
     *    ELSE:
     *    a. Display "Rings:" header
     *    b. FOR each ring in vector: display numbered ring info
     */
    void printInventory() {
        cout << "Equipped Items:" << endl;
        if (weapon) {
            cout << "Weapon: ";
            weapon->print();
        } else {
            cout << "Weapon: None" << endl;
        }
        if (armor) {
            cout << "Armour: ";
            armor->print();
        } else {
            cout << "Armour: None" << endl;
        }
        if (shield) {
            cout << "Shield: ";
            shield->print();
        } else {
            cout << "Shield: None" << endl;
        }
        if (ring.empty()) {
            cout << "Rings: None" << endl;
        } else {
            cout << "Rings:" << endl;
            for (size_t i = 0; i < ring.size(); ++i) {
                cout << "  " << i+1 << ". ";
                ring[i]->print();
            }
        }
    }

    /**
     * @brief Handle successful defense (race-specific behavior)
     *
     * @param attacker Character that initiated the attack
     * @param defender Character that successfully defended
     */
    void successfulDef(Character* attacker, Character* defender);

    /**
     * @brief Calculate total weight of all equipped items
     *
     * Pseudo-code:
     * 1. Initialize total to 0
     * 2. IF weapon equipped: add weapon weight to total
     * 3. IF armor equipped: add armor weight to total
     * 4. IF shield equipped: add shield weight to total
     * 5. FOR each ring in ring vector: add ring weight to total
     * 6. RETURN total
     *
     * @return int Total weight of all equipped items
     */
    int getCurrentWeight() const {
        int total = 0;
        if (weapon) total += weapon->weight;
        if (armor) total += armor->weight;
        if (shield) total += shield->weight;
        for (const auto& r : ring) total += r->weight;
        return total;
    }

    /**
     * @brief Calculate total attack value including equipment bonuses
     *
     * Pseudo-code:
     * 1. Start with base attack value
     * 2. IF weapon equipped: add weapon attack bonus
     * 3. IF armor equipped: subtract armor attack penalty
     * 4. IF shield equipped: subtract shield attack penalty
     * 5. FOR each ring: add ring strength bonus to attack
     * 6. RETURN total attack value
     *
     * @return int Total attack value including all equipment modifiers
     */
    int getTotalAttack() const {
        int total = attack;
        if (weapon) total += weapon->attack_inc;
        if (armor) total -= armor->attack_dec;
        if (shield) total -= shield->attack_dec;
        for (const auto& r : ring) total += r->strength_inc;
        return total;
    }

    /**
     * @brief Calculate total defense value including equipment bonuses
     *
     * Pseudo-code:
     * 1. Start with base defense value
     * 2. IF armor equipped: add armor defense bonus
     * 3. IF shield equipped: add shield defense bonus
     * 4. RETURN total defense value
     *
     * @return int Total defense value including all equipment modifiers
     */
    int getTotalDefence() const {
        int total = defence;
        if (armor) total += armor->defence_inc;
        if (shield) total += shield->defence_inc;
        return total;
    }

    /**
     * @brief Calculate total strength including equipment bonuses
     *
     * Pseudo-code:
     * 1. Start with base strength value
     * 2. FOR each ring: add ring strength bonus
     * 3. RETURN total strength value
     *
     * @return int Total strength value including ring bonuses
     */
    int getTotalStrength() const {
        int total = strength;
        for (const auto& r : ring) total += r->strength_inc;
        return total;
    }

    /**
     * @brief Calculate total maximum health including equipment bonuses
     *
     * Pseudo-code:
     * 1. Start with base health value
     * 2. FOR each ring: add ring health bonus (can be negative)
     * 3. RETURN total maximum health value
     *
     * @return int Total maximum health value including ring bonuses
     */
    int getTotalHealth() const {
        int total = health;
        for (const auto& r : ring) total += r->health;
        return total;
    }

    /**
     * @brief Drop currently equipped weapon
     */
    void dropWeapon() {
        if (weapon) {
            cout << "Dropping weapon: " << weapon->name << endl;
            weapon = nullptr;
        } else {
            cout << "No weapon to drop.\n";
        }
    }

    /**
     * @brief Drop currently equipped armor
     */
    void dropArmour() {
        if (armor) {
            cout << "Dropping armor: " << armor->name << endl;
            armor = nullptr;
        } else {
            cout << "No armor to drop.\n";
        }
    }

    /**
     * @brief Drop currently equipped shield
     */
    void dropShield() {
        if (shield) {
            cout << "Dropping shield: " << shield->name << endl;
            shield = nullptr;
        } else {
            cout << "No shield to drop.\n";
        }
    }

    /**
     * @brief Drop a ring by index
     *
     * @param index Zero-based index of ring to drop
     */
    void dropRing(int index) {
        if (index >= 0 && index < (int)ring.size()) {
            cout << "Dropping ring: " << ring[index]->name << endl;
            ring.erase(ring.begin() + index);
        } else {
            cout << "Invalid ring choice.\n";
        }
    }
};

/**
 * @class Human
 * @brief Human race class with balanced stats
 *
 * Attack chance: 2/3, Defense chance: 1/2
 */
class Human : public Character {
public:
    /**
     * @brief Constructor for Human character
     *
     * @param n Character's name
     */
    Human(string n) : Character(n, "Human", 30, 2.0/3.0, 20, 1.0/2.0, 60, 100) {}

    /**
     * @brief Print Human character statistics
     */
    virtual void printStats() override {
        cout << name << ", race: " << race << ", attack: " << getTotalAttack()
        << ", defence: " << getTotalDefence() << ", health: " << getTotalHealth()
        << ", strength: " << getTotalStrength() << endl;
    }

    void successfulDef(Character* attacker, Character* defender);
};

/**
 * @class Elf
 * @brief Elf race class with high attack, low defense
 *
 * Attack chance: 1/1, Defense chance: 1/4
 */
class Elf : public Character {
public:
    /**
     * @brief Constructor for Elf character
     *
     * @param n Character's name
     */
    Elf(string n) : Character(n, "Elf", 40, 1.0/1.0, 10, 1.0/4.0, 40, 70) {}

    /**
     * @brief Print Elf character statistics
     */
    virtual void printStats() override {
        cout << name << ", race: " << race << ", attack: " << getTotalAttack()
        << ", defence: " << getTotalDefence() << ", health: " << getTotalHealth()
        << ", strength: " << getTotalStrength() << endl;
    }

    void successfulDef(Character* attacker, Character* defender);
};

/**
 * @class Dwarf
 * @brief Dwarf race class with balanced stats and high strength
 *
 * Attack chance: 2/3, Defense chance: 2/3
 */
class Dwarf : public Character {
public:
    /**
     * @brief Constructor for Dwarf character
     *
     * @param n Character's name
     */
    Dwarf(string n) : Character(n, "Dwarf", 30, 2.0/3.0, 20, 2.0/3.0, 50, 130) {}

    /**
     * @brief Print Dwarf character statistics
     */
    virtual void printStats() override {
        cout << name << ", race: " << race << ", attack: " << getTotalAttack()
        << ", defence: " << getTotalDefence() << ", health: " << getTotalHealth()
        << ", strength: " << getTotalStrength() << endl;
    }

    void successfulDef(Character* attacker, Character* defender);
};

/**
 * @class Hobbit
 * @brief Hobbit race class with low attack, high defense chance, high health
 *
 * Attack chance: 1/3, Defense chance: 2/3
 */
class Hobbit : public Character {
public:
    /**
     * @brief Constructor for Hobbit character
     *
     * @param n Character's name
     */
    Hobbit(string n) : Character(n, "Hobbit", 25, 1.0/3.0, 20, 2.0/3.0, 70, 85) {}

    /**
     * @brief Print Hobbit character statistics
     */
    virtual void printStats() override {
        cout << name << ", race: " << race << ", attack: " << getTotalAttack()
        << ", defence: " << getTotalDefence() << ", health: " << getTotalHealth()
        << ", strength: " << getTotalStrength() << endl;
    }

    void successfulDef(Character* attacker, Character* defender);
};

/**
 * @class Orc
 * @brief Orc race class with stats that change based on time of day
 *
 * Day: Attack chance 1/4, Defense chance 1/4
 * Night: Attack chance 1/1, Defense chance 1/2
 */
class Orc : public Character {
public:
    bool isNight;  ///< Track whether it's currently night time

    /**
     * @brief Constructor for Orc character
     *
     * Initializes isNight to false (daytime)
     *
     * @param n Character's name
     */
    Orc(string n) : Character(n, "Orc", 25, 0.25, 10, 0.25, 50, 130), isNight(false) {}

    /**
     * @brief Update Orc stats based on time of day
     *
     * Pseudo-code:
     * 1. Set isNight flag
     * 2. IF it's night:
     *    a. Set attack to 45
     *    b. Set attack_chance to 1.0
     *    c. Set defence to 25
     *    d. Set defence_chance to 0.5
     * 3. ELSE (it's day):
     *    a. Set attack to 25
     *    b. Set attack_chance to 0.25
     *    c. Set defence to 10
     *    d. Set defence_chance to 0.25
     *
     * @param night true if it's night, false if it's day
     */
    void setTimeOfDay(bool night) {
        isNight = night;
        if (isNight) {
            attack = 45;
            attack_chance = 1.0;
            defence = 25;
            defence_chance = 0.5;
        } else {
            attack = 25;
            attack_chance = 0.25;
            defence = 10;
            defence_chance = 0.25;
        }
    }

    /**
     * @brief Print Orc statistics with time of day indicator
     */
    void printStats() override {
        cout << "Orc " << name << " stats: "
             << "Attack: " << getTotalAttack() << "Defence: " << getTotalDefence()
             << "Health: " << getTotalHealth() << ", Strength: " << getTotalStrength()
             << (isNight ? " [Night]" : " [Day]") << endl;
    }

    void successfulDef(Character* attacker, Character* defender);
};
