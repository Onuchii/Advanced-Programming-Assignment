/**
 * @file board.hpp
 * @brief Board and Square classes for game grid management
 *
 * This file contains the Square class (representing individual board locations)
 * and the Board class (managing the game grid). All squares are dynamically
 * allocated and managed with smart pointers as required by the project
 * specifications.
 *
 * @author [Ish Soundankar]
 */
#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "characters.hpp"
#include "items.hpp"

using namespace std;

// Forward declarations
class Character;
class Item;

/**
 * @class Square
 * @brief Class representing a single square on the game board
 *
 * Each square can contain an enemy, an item, and/or the player.
 * All pointers are managed using smart pointers for automatic memory management.
 */
class Square {
public:
    shared_ptr<Character> enemy;  ///< Pointer to enemy character (nullptr if none)
    shared_ptr<Item> item;       ///< Pointer to item (nullptr if none)
    shared_ptr<Character> player; ///< Pointer to player character (nullptr if none)

    /**
     * @brief Print information about what's on this square
     *
     * Pseudo-code:
     * 1. IF enemy present: display "Enemy here:" and enemy stats
     * 2. IF item present: display "Item here:" and item info
     * 3. IF player present: display "Player here:" and player stats
     * 4. IF square is empty: display "Square is empty!"
     */
    void printInfo() {
        if (enemy) {
            cout << "Enemy here: " << endl;
            enemy->printStats();
        }
        if (item) {
            cout << "Item here: " << endl;
            item->print();
        }
        if (player) {
            cout << "Player here: " << endl;
            player->printStats();
        }
        if (!enemy && !item && !player) {
            cout << "Square is empty!" << endl;
        }
    }
};

/**
 * @class Board
 * @brief Class representing the game board
 *
 * The board is a 2D grid of Square objects, all dynamically allocated
 * and managed with smart pointers. The grid uses a vector of vectors
 * to store shared pointers to Square objects.
 */
class Board {
public:
    int width;                                    ///< Width of the board (number of columns)
    int height;                                   ///< Height of the board (number of rows)
    vector<vector<shared_ptr<Square>>> grid;      ///< 2D grid of squares

    /**
     * @brief Constructor to create a board of specified dimensions
     *
     * Pseudo-code:
     * 1. Set width and height
     * 2. Resize outer vector to match height
     * 3. FOR each row:
     *    a. Resize inner vector to match width
     *    b. FOR each column: create new Square using make_shared
     *
     * @param w Width of the board
     * @param h Height of the board
     */
    Board(int w, int h) {
        width = w;
        height = h;
        grid.resize(height);
        for (int i = 0; i < height; ++i) {
            grid[i].resize(width);
            for (int j = 0; j < width; j++) {
                grid[i][j] = make_shared<Square>();
            }
        }
    }

    /**
     * @brief Print the board in ASCII format
     *
     * Pseudo-code:
     * 1. FOR each row:
     *    a. FOR each column:
     *       - IF player present: print "#"
     *       - ELSE IF enemy present: print "*"
     *       - ELSE IF item present: print "+"
     *       - ELSE: print space
     *       - Print "|" separator
     *    b. Print newline
     *
     * Symbols: # = player, * = enemy, + = item, space = empty
     */
    void printBoard() {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; j++) {
                cout << "|" << (grid[i][j]->player ? "#" :
                                    (grid[i][j]->enemy ? "*" :
                                         (grid[i][j]->item ? "+" : " "))) << "|";
            }
            cout << endl;
        }
    }

    /**
     * @brief Populate board with enemies and items
     *
     * @param enemies Vector of enemy characters to place
     * @param items Vector of items to place
     */
    void populateBoard(const vector<shared_ptr<Character>> enemies,
                       const vector<shared_ptr<Item>> items);
};
