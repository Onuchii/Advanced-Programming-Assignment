/**
 * @file items.hpp
 * @brief Item class hierarchy for game items
 *
 * This file contains the base Item class and derived classes (Weapon, Armour,
 * Shield, Ring) that can be found and equipped by characters. The Item class
 * uses polymorphism through virtual functions.
 *
 * @author [Onuchi Kalu, 25052624]
 */
#pragma once
#include <string>
#include <iostream>
using namespace std;

/**
 * @class Item
 * @brief Base class for all items in the game
 *
 * This abstract base class provides the common interface for all items.
 * Derived classes override the print() method to display item-specific information.
 */
class Item {
public:
    string name;   ///< Name of the item
    int weight;    ///< Weight of the item (affects carrying capacity)

    /**
     * @brief Constructor to initialize item
     *
     * @param n Item name
     * @param w Item weight
     */
    Item(string n, int w) {
        name = n;
        weight = w;
    }

    /**
     * @brief Virtual function to print item information
     *
     * Can be overridden by derived classes to display item-specific information.
     */
    virtual void print() {
        cout << name << "(Weight: " << weight << endl;
    }

    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~Item() {}
};

/**
 * @class Weapon
 * @brief Weapon class that increases attack power
 *
 * Inherits from Item and adds attack bonus functionality.
 */
class Weapon : public Item {
public:
    int attack_inc;  ///< Attack bonus provided by this weapon

    /**
     * @brief Constructor to initialize weapon
     *
     * @param n Weapon name
     * @param w Weapon weight
     * @param atk Attack bonus
     */
    Weapon(string n, int w, int atk) : Item(n, w) {
        attack_inc = atk;
    }

    /**
     * @brief Override print function to display weapon-specific information
     */
    virtual void print() override {
        cout << name << "(Weapon, Attack + " << attack_inc
             << ",Weight: " << weight << endl;
    }
};

/**
 * @class Armour
 * @brief Armor class that increases defense but may reduce attack
 *
 * Inherits from Item and provides defense bonus with optional attack penalty.
 */
class Armour : public Item {
public:
    int defence_inc;  ///< Defense bonus provided by this armor
    int attack_dec;   ///< Attack penalty from wearing this armor

    /**
     * @brief Constructor to initialize armor
     *
     * @param n Armor name
     * @param w Armor weight
     * @param def Defense bonus
     * @param atk Attack penalty
     */
    Armour(string n, int w, int def, int atk) : Item(n, w) {
        defence_inc = def;
        attack_dec = atk;
    }

    /**
     * @brief Override print function to display armor-specific information
     */
    virtual void print() override {
        cout << name << "(Armour, Defence + " << defence_inc
             << ", Attack - " << attack_dec << ",Weight: " << weight << endl;
    }
};

/**
 * @class Shield
 * @brief Shield class that inherits from Armour
 *
 * Provides defense bonus similar to armor but with different display name.
 */
class Shield : public Armour {
public:
    /**
     * @brief Constructor to initialize shield
     *
     * @param n Shield name
     * @param w Shield weight
     * @param def Defense bonus
     * @param atk Attack penalty
     */
    Shield(string n, int w, int def, int atk) : Armour(n, w, def, atk) {}

    /**
     * @brief Override print function to display shield-specific information
     */
    virtual void print() override {
        cout << name << "(Shield, Defence + " << defence_inc
             << ", Attack - " << attack_dec << ",Weight: " << weight << endl;
    }
};

/**
 * @class Ring
 * @brief Ring class that provides various bonuses (health, strength)
 *
 * Characters can equip multiple rings, allowing unbounded item carrying
 * capacity if sufficient rings of strength are found.
 */
class Ring : public Item {
public:
    int health;        ///< Health bonus/penalty (can be positive or negative)
    int strength_inc;  ///< Strength bonus (can be positive or zero)

    /**
     * @brief Constructor to initialize ring
     *
     * @param n Ring name
     * @param w Ring weight
     * @param hel Health bonus/penalty
     * @param str Strength bonus
     */
    Ring(string n, int w, int hel, int str)
        : Item(n, w), health(hel), strength_inc(str) {}

    /**
     * @brief Override print function to display ring-specific information
     */
    void print() override {
        cout << name << " (Ring, Health "
             << (health >= 0 ? "+" : "") << health
             << ", Strength + " << strength_inc
             << ", Weight: " << weight << ")\n";
    }
};
