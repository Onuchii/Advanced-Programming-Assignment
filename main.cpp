/**
 * @file main.cpp
 * @brief Main game loop and user interface
 *
 * This file contains the main game loop, combat system, and user command
 * processing for the text-based adventure game. It handles player movement,
 * combat, inventory management, and the day/night cycle.
 *
 * @author [Ish Soundankar]
 */
#include "ItemsDB.h"
#include <iostream>
#include <vector>
#include <memory>
#include <characters.hpp>
#include <items.hpp>
#include <board.hpp>
#include <stdlib.h>
using namespace std;

/**
 * @brief Handle combat between two characters
 *
 * Pseudo-code:
 * 1. Display attack message
 * 2. Generate random attack roll (0.0 to 1.0)
 * 3. IF attack roll > attacker's attack chance:
 *    a. Display miss message
 *    b. RETURN (attack failed)
 * 4. Generate random defense roll (0.0 to 1.0)
 * 5. IF defense roll < defender's defense chance:
 *    a. Call defender's successfulDef() method (race-specific)
 *    b. RETURN (attack blocked)
 * 6. Calculate damage = attacker total attack - defender total defense
 * 7. IF damage > 0:
 *    a. Subtract damage from defender health
 *    b. IF health < 0: set health to 0
 *    c. Display damage message
 *    d. IF health > max health: set health to max health
 * 8. ELSE: display block message
 * 9. IF defender health <= 0: display defeat message
 *
 * @param attacker Character initiating the attack
 * @param defender Character being attacked
 */
void attack(Character* attacker, Character* defender) {
    cout << attacker->name << " attacks " << defender->name << endl;
    float attackRoll = (float)rand() / RAND_MAX;
    if (attackRoll > attacker->attack_chance) {
        cout << attacker->name << " missed!" << endl;
        return;
    }
    float defenceRoll = (float)rand() / RAND_MAX;
    if (defenceRoll < defender->defence_chance) {
        defender->successfulDef(attacker, defender);
        return;
    }

    if (attacker->getTotalAttack() > defender->getTotalDefence()) {
        int damage = attacker->getTotalAttack() - defender->getTotalDefence();
        defender->health -= damage;
        if (defender->health < 0) defender->health = 0;
        cout << defender->name << " takes " << damage << " hits of damage" << endl;
        cout << defender->name << " health: " << defender->getTotalHealth() << endl;
        if (defender->health > defender->getTotalHealth()) {
            defender->health = defender->getTotalHealth();
        }
    } else {
        cout << defender->name << " blocked the attack" << endl;
    }
    if (defender->getTotalHealth() <= 0) {
        cout << defender->name << " defeated" << endl;
    }
}

// Global player and enemy character pointers
shared_ptr<Character> player;
shared_ptr<Character> enemy;

/**
 * @brief Create and configure the player character
 *
 * Pseudo-code:
 * 1. Create temporary instances of each race to display stats
 * 2. Get player name from input
 * 3. WHILE valid choice is false:
 *    a. Display race selection menu with stats
 *    b. Get race choice from user
 *    c. SWITCH on choice:
 *       - Case 1-5: Create appropriate race character, set validChoice = true
 *       - Default: Display error, clear input buffer
 * 4. Clear screen and display selected character stats
 *
 * @param player Reference to the player character pointer to be set
 */
void user(shared_ptr<Character>& player) {
    int choice;
    Human tempHuman("Human");
    Elf tempElf("Elf");
    Dwarf tempDwarf("Dwarf");
    Hobbit tempHobbit("Hobbit");
    Orc tempOrc("Orc");

    string userName;
    cout << "Enter Name: " << endl;
    cin >> userName;

    bool validChoice = false;
    while (!validChoice) {
        cout << "Select race of player: " << endl;
        cout << "1." << endl;
        tempHuman.printStats();
        cout << "2." << endl;
        tempElf.printStats();
        cout << "3." << endl;
        tempDwarf.printStats();
        cout << "4." << endl;
        tempHobbit.printStats();
        cout << "5." << endl;
        tempOrc.printStats();

        cout << "Enter your choice (1-5): " << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            player = make_shared<Human>(userName);
            validChoice = true;
            break;
        case 2:
            player = make_shared<Elf>(userName);
            validChoice = true;
            break;
        case 3:
            player = make_shared<Dwarf>(userName);
            validChoice = true;
            break;
        case 4:
            player = make_shared<Hobbit>(userName);
            validChoice = true;
            break;
        case 5:
            player = make_shared<Orc>(userName);
            validChoice = true;
            break;
        default:
            cout << "Invalid Choice! Please enter a number between 1 and 5." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            break;
        }
    }
    system("cls");
    cout << "You selected: ";
    player->printStats();
}

/**
 * @brief Display current game state information
 *
 * @param playerRow Current row position of player on board
 * @param playerColumn Current column position of player on board
 * @param player Pointer to player character
 * @param gold Current amount of gold collected
 */
void currentStats(int playerRow, int playerColumn, shared_ptr<Character> player, int gold) {
    cout << "Current location: " << playerRow << " " << playerColumn << endl;
    player->printStats();
    cout << "Gold: " << gold << endl;
}

/**
 * @brief Main game loop
 *
 * Pseudo-code:
 * 1. Initialize game variables (command count, day/night, enemies, items, board)
 * 2. Create player character
 * 3. WHILE game not over:
 *    a. Display command prompt
 *    b. Get user command
 *    c. Clear screen
 *    d. Remove player from current square
 *    e. SWITCH on command:
 *       - Movement (w/a/s/d): Update position, check square content
 *       - Pickup (g): Attempt to pick up item
 *       - Attack (j): Combat with enemy on square
 *       - Drop (h): Drop equipped item
 *       - Look (k): Display square information
 *       - Inventory (l): Display player inventory
 *       - Exit (x): Set gameOver = true
 *    f. Update day/night cycle if needed
 *    g. Place player on new square
 *    h. Display current stats and board
 * 4. RETURN 0
 *
 * @return int Exit code (0 for success)
 */
int main() {
    int commandCount = 0;
    bool isNight = false;
    int length = 12;
    int breadth = 12;
    char changeParameter;    // Start as day

    vector<shared_ptr<Character>> enemies;
    enemies.push_back(make_shared<Human>("Bob"));
    enemies.push_back(make_shared<Elf>("Legolas"));
    enemies.push_back(make_shared<Dwarf>("Gimli"));
    enemies.push_back(make_shared<Hobbit>("Frodo"));
    enemies.push_back(make_shared<Orc>("Azog"));

    vector<shared_ptr<Item>> items;
    items.push_back(Sword);
    items.push_back(Dagger);
    items.push_back(LeatherArmor);
    items.push_back(PlateArmor);
    items.push_back(RingOfLife);
    items.push_back(RingOfStrength);

    cout << "Default length and breadth of grid is 12. Press 1 to change parameters.\nPress any key to continue\nPress (1) to change parameters"<<endl;
    cin >> changeParameter;
    if(changeParameter == '1'){
        cout << "Enter length: ";
        cin >> length;
        cout << "Enter breadth: ";
        cin >> breadth;
        cout << endl;
    }
    system("cls");
    Board board(length, breadth);
    board.populateBoard(enemies, items);
    user(player);
    system("cls");
    player->printStats();
    int playerRow = 0;
    int playerColumn = 0;
    int gold = 0;

    char choice;
    bool gameOver = false;

    while (!gameOver) {

        cout << "Enter command (w/a/s/d = move, g = pickup, j = attack, h = drop, k = look, l = inventory, x = exit): " << endl;
        if(isNight == true){
            cout<< "Current Time: Night"<<endl;
        }
        else{
            cout << "Current Time: Day"<<endl;
        }
        cin >> choice;
        system("cls");

        board.grid[playerRow][playerColumn]->player = nullptr;

        switch (choice) {
        case 'w':
            cout << "moving up" << endl;
            if (playerRow > 0) {
                playerRow--;
                auto& currentSquare = board.grid[playerRow][playerColumn];
                if (currentSquare->enemy) {
                    cout << "\n*** You've encountered an enemy! ***" << endl;
                    currentSquare->enemy->printStats();
                }
                if (currentSquare->item) {
                    cout << "\n*** You've found an item! ***" << endl;
                    currentSquare->item->print();
                }
            } else {
                cout << "Cannot move up! You're at the top edge of the board." << endl;
            }
            commandCount++;
            break;

        case 's':
            cout << "moving down" << endl;
            if (playerRow < 11) {
                playerRow++;
                auto& currentSquare = board.grid[playerRow][playerColumn];
                if (currentSquare->enemy) {
                    cout << "\n*** You've encountered an enemy! ***" << endl;
                    currentSquare->enemy->printStats();
                }
                if (currentSquare->item) {
                    cout << "\n*** You've found an item! ***" << endl;
                    currentSquare->item->print();
                }
            } else {
                cout << "Cannot move down! You're at the bottom edge of the board." << endl;
            }
            commandCount++;
            break;

        case 'a':
            cout << "moving left" << endl;
            if (playerColumn > 0) {
                playerColumn--;
                auto& currentSquare = board.grid[playerRow][playerColumn];
                if (currentSquare->enemy) {
                    cout << "\n*** You've encountered an enemy! ***" << endl;
                    currentSquare->enemy->printStats();
                }
                if (currentSquare->item) {
                    cout << "\n*** You've found an item! ***" << endl;
                    currentSquare->item->print();
                }
            } else {
                cout << "Cannot move left! You're at the left edge of the board." << endl;
            }
            commandCount++;
            break;

        case 'd':
            cout << "moving right" << endl;
            if (playerColumn < 11) {
                playerColumn++;
                auto& currentSquare = board.grid[playerRow][playerColumn];
                if (currentSquare->enemy) {
                    cout << "\n*** You've encountered an enemy! ***" << endl;
                    currentSquare->enemy->printStats();
                }
                if (currentSquare->item) {
                    cout << "\n*** You've found an item! ***" << endl;
                    currentSquare->item->print();
                }
            } else {
                cout << "Cannot move right! You're at the right edge of the board." << endl;
            }
            commandCount++;
            break;

        case 'h':
            cout << "Drop what? (1=Weapon, 2=Armour, 3=Shield, 4=Ring): ";
            char slot;
            cin >> slot;
            if (slot == '1') {
                player->dropWeapon();
            } else if (slot == '2') {
                player->dropArmour();
            } else if (slot == '3') {
                player->dropShield();
            } else if (slot == '4') {
                if (player->ring.empty()) {
                    cout << "No rings to drop." << endl;
                } else {
                    cout << "Which ring? ";
                    for (size_t i = 0; i < player->ring.size(); ++i) {
                        cout << i+1 << ") " << player->ring[i]->name << "  ";
                    }
                    cout << endl;
                    int rnum;
                    cin >> rnum;
                    if (cin.fail() || rnum < 1 || rnum > (int)player->ring.size()) {
                        cout << "Invalid ring selection! Please enter a number between 1 and " << player->ring.size() << "." << endl;
                        cin.clear();
                        cin.ignore(10000, '\n');
                    } else {
                        player->dropRing(rnum - 1);
                    }
                }
            } else {
                cout << "Invalid choice! Please enter 1, 2, 3, or 4." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            }
            commandCount++;
            break;

        case 'j': {
            cout << "attack" << endl;
            auto& enemyOnSquare = board.grid[playerRow][playerColumn]->enemy;
            player->printStats();
            if (enemyOnSquare) {
                enemyOnSquare->printStats();
                attack(player.get(), enemyOnSquare.get());

                if (enemyOnSquare->getTotalHealth() <= 0) {
                    cout << enemyOnSquare->race << " Defeated!  Received 20 gold!" << endl;
                    board.grid[playerRow][playerColumn]->enemy = nullptr;
                    gold += 20;
                    bool anyEnemiesLeft = false;
                    for (int r = 0; r < board.height; ++r) {
                        for (int c = 0; c < board.width; ++c) {
                            if (board.grid[r][c]->enemy) {
                                anyEnemiesLeft = true;
                                break;
                            }
                        }
                        if (anyEnemiesLeft) break;
                    }
                    if (!anyEnemiesLeft) {
                        cout << "Congratulations! You defeated all the enemies and won the game!" << endl;
                        gameOver = true;
                    }
                    break;
                }

                attack(enemyOnSquare.get(), player.get());
                if (player->getTotalHealth() <= 0) {
                    cout << "You Died! \n Game over!" << endl;
                    gameOver = true;
                }
            } else {
                cout << "No enemy to attack" << endl;
            }
            commandCount++;
            break;
        }

        case 'k':
            cout << "Look" << endl;
            cout << "Information about current square: " << endl;
            board.grid[playerRow][playerColumn]->printInfo();
            commandCount++;
            break;

        case 'l':
            player->printInventory();
            cout << "Total gold collected: " << gold << endl;
            commandCount++;
            break;

        case 'g': {
            cout << "pickup";
            auto& itemOnSquare = board.grid[playerRow][playerColumn]->item;
            if (itemOnSquare) {
                if (player->pickUp(itemOnSquare)) {
                    itemOnSquare = nullptr;
                }
            } else {
                cout << "No item here!" << endl;
            }
        }
            commandCount++;
            break;

        case 'x':
            cout << "Exit" << endl;
            gameOver = true;
            break;

        default:
            cout << "Invalid command! Please enter one of the following:" << endl;
            cout << "w/a/s/d = move, g = pickup, j = attack, h = drop, k = look, l = inventory, x = exit" << endl;
            break;
        }

        // Day/night cycle logic - switches every 5 commands
        if (commandCount % 10 < 5) {
            if (isNight) {
                isNight = false;
                cout << "It is now daytime." << endl;
                for (int r = 0; r < board.height; ++r) {
                    for (int c = 0; c < board.width; ++c) {
                        auto enemyPtr = board.grid[r][c]->enemy;
                        if (enemyPtr) {
                            Orc* orcPtr = dynamic_cast<Orc*>(enemyPtr.get());
                            if (orcPtr) {
                                orcPtr->setTimeOfDay(isNight);
                            }
                        }
                    }
                }
            }
        } else {
            if (!isNight) {
                isNight = true;
                cout << "It is now night." << endl;
                for (int r = 0; r < board.height; ++r) {
                    for (int c = 0; c < board.width; ++c) {
                        auto enemyPtr = board.grid[r][c]->enemy;
                        if (enemyPtr) {
                            Orc* orcPtr = dynamic_cast<Orc*>(enemyPtr.get());
                            if (orcPtr) {
                                orcPtr->setTimeOfDay(isNight);
                            }
                        }
                    }
                }
            }
        }

        board.grid[playerRow][playerColumn]->player = player;
        currentStats(playerRow, playerColumn, player, gold);
        board.printBoard();
    }

    return 0;
}
