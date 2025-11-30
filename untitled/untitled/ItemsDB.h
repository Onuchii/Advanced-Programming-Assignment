/**
 * @file ItemsDB.h
 * @brief Predefined items database
 *
 * This file contains static shared pointers to predefined items that can be
 * used in the game. All items are created using make_shared for proper
 * memory management.
 *
 * @author [Onuchi Kalu, 25052624]
 */
#pragma once
#include "items.hpp"
#include <memory>

// Predefined weapons
static std::shared_ptr<Weapon> Sword = std::make_shared<Weapon>("Sword", 10, 10);
static std::shared_ptr<Weapon> Dagger = std::make_shared<Weapon>("Dagger", 5, 5);

// Predefined armor
static std::shared_ptr<Armour> PlateArmor = std::make_shared<Armour>("Plate Armor", 40, 10, 5);
static std::shared_ptr<Armour> LeatherArmor = std::make_shared<Armour>("Leather Armor", 20, 5, 0);

// Predefined shields
static std::shared_ptr<Shield> LargeShield = std::make_shared<Shield>("Large Shield", 30, 10, 5);
static std::shared_ptr<Shield> SmallShield = std::make_shared<Shield>("Small Shield", 10, 5, 0);

// Predefined rings
static std::shared_ptr<Ring> RingOfLife = std::make_shared<Ring>("Ring of Life", 1, 10, 0);
static std::shared_ptr<Ring> RingOfStrength = std::make_shared<Ring>("Ring of Strength", 1, -10, 50);
