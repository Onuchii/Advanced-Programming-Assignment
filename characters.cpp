/**
 * @file characters.cpp
 * @brief Implementation of race-specific successful defense behaviors
 *
 * This file contains the implementations of the successfulDef() method for
 * each race, providing unique defensive behaviors when a character
 * successfully defends against an attack.
 *
 * @author [Steffy Pereppadan Ignatious]
 */
#include "characters.hpp"
#include <iostream>
#include <cstdlib> // for rand()

/**
 * @brief Base class successful defense handler (empty implementation)
 *
 * @param attacker Character that initiated the attack (unused)
 * @param defender Character that successfully defended (unused)
 */
void Character::successfulDef(Character*, Character*) {
}

/**
 * @brief Human race successful defense handler
 *
 * Pseudo-code:
 * 1. Display successful defense message with defender's name
 *
 * @param attacker Character that initiated the attack (unused)
 * @param defender Human character that successfully defended
 */
void Human::successfulDef(Character*, Character* defender) {
    cout << defender->name << " defended successfully!" << endl;
}

/**
 * @brief Elf race successful defense handler
 *
 * Pseudo-code:
 * 1. Display successful defense message
 * 2. Increase defender's health by 1
 * 3. Display updated health
 *
 * @param attacker Character that initiated the attack (unused)
 * @param defender Elf character that successfully defended
 */
void Elf::successfulDef(Character*, Character* defender) {
    cout << defender->name << " defended successfully!" << endl;
    defender->health += 1;
    cout << defender->name << " health increased to " << defender->health << endl;
}

/**
 * @brief Dwarf race successful defense handler
 *
 * Pseudo-code:
 * 1. Display successful defense message with defender's name
 *
 * @param attacker Character that initiated the attack (unused)
 * @param defender Dwarf character that successfully defended
 */
void Dwarf::successfulDef(Character*, Character* defender) {
    cout << defender->name << " defended successfully!" << endl;
}

/**
 * @brief Hobbit race successful defense handler
 *
 * Pseudo-code:
 * 1. Display successful defense message
 * 2. Generate random damage (0-5)
 * 3. Subtract damage from defender's health
 * 4. IF health < 0: set health to 0
 * 5. Display updated health
 *
 * @param attacker Character that initiated the attack (unused)
 * @param defender Hobbit character that successfully defended
 */
void Hobbit::successfulDef(Character*, Character* defender) {
    cout << defender->name << " defended successfully!" << endl;
    defender->health -= rand() % 6;
    if (defender->health < 0) defender->health = 0;
    cout << defender->name << " health reduced to " << defender->health << endl;
}

/**
 * @brief Orc race successful defense handler
 *
 * Pseudo-code:
 * 1. IF it's day (not night):
 *    a. Calculate base damage (attacker attack - defender defence)
 *    b. IF damage < 0: set damage to 0
 *    c. Calculate quarter damage (damage / 4)
 *    d. Subtract quarter damage from defender health
 *    e. IF health < 0: set health to 0
 * 2. ELSE (it's night):
 *    a. Increase defender's health by 1
 *    b. Display health increase message
 *
 * @param attacker Character that initiated the attack
 * @param defender Orc character that successfully defended
 */
void Orc::successfulDef(Character* attacker, Character* defender) {
    if (!isNight) {
        int temp = attacker->attack - defender->defence;
        if (temp < 0) temp = 0;
        int damageTaken = temp / 4;  // quarter damage
        defender->health -= damageTaken;
        if (defender->health < 0) defender->health = 0;
    } else {
        defender->health += 1;
        cout << defender->name << " health increased to " << defender->health << endl;
    }
}
