/**
 * @file board.cpp
 * @brief Implementation of Board class methods
 *
 * This file contains the implementation of the populateBoard() method
 * which randomly places enemies and items on the game board.
 *
 * @author [Ish Soundankar]
 */
#include "board.hpp"
#include <cstdlib> // rand, srand
#include <ctime>

/**
 * @brief Randomly place enemies and items on the game board
 *
 * Pseudo-code:
 * 1. Seed random number generator with current time
 * 2. FOR each enemy in enemies vector:
 *    a. Generate random x, y coordinates
 *    b. WHILE square at (x,y) already has an enemy:
 *       - Generate new random x, y coordinates
 *    c. Place enemy at (x,y)
 * 3. FOR each item in items vector:
 *    a. Generate random x, y coordinates
 *    b. WHILE square at (x,y) has enemy OR item:
 *       - Generate new random x, y coordinates
 *    c. Place item at (x,y)
 *
 * @param enemies Vector of enemy characters to place on board
 * @param items Vector of items to place on board
 */
void Board::populateBoard(const vector<shared_ptr<Character>> enemies,
                          const vector<shared_ptr<Item>> items) {
    srand(time(nullptr));

    // Place each enemy on a random empty square
    for (size_t i = 0; i < enemies.size(); i++) {
        shared_ptr<Character> enemyPointer = enemies[i];
        int x = rand() % height;
        int y = rand() % width;

        while (grid[x][y]->enemy != nullptr) {
            x = rand() % height;
            y = rand() % width;
        }

        grid[x][y]->enemy = enemyPointer;
    }

    // Place each item on a random empty square (no enemy or item already there)
    for (size_t i = 0; i < items.size(); i++) {
        shared_ptr<Item> itemPointer = items[i];
        int x = rand() % height;
        int y = rand() % width;

        while (grid[x][y]->enemy != nullptr || grid[x][y]->item != nullptr) {
            x = rand() % height;
            y = rand() % width;
        }

        grid[x][y]->item = itemPointer;
    }
}
