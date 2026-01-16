#ifndef INC_15SHKI_PUZZLE15_H
#define INC_15SHKI_PUZZLE15_H

#include <string>
#include <vector>
#include <utility>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

class Puzzle15 {
private:
    std::vector<std::vector<int>> board;
    int emptyRow, emptyCol;
    int moveCount;
    std::string startTime;
    bool gameWon;

    // Приватные методы будут определены в cpp файле
    bool isSolvable(const std::vector<int>& flatBoard);

public:
    Puzzle15();
    void initializeBoard();
    void displayBoard();
    bool isValidMove(int row, int col) const;
    bool makeMove(int row, int col);
    bool checkWin();
    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);
    bool isGameWon() const;
    int getMoveCount() const;
    std::string getGameTime() const;
    std::string getCurrentGameTime() const;  // Добавлено
    std::pair<int, int> getHint() const;
    void debugBoard() const;
    void clearConsole() const;
};

// Прототипы функций меню
void displayMainMenu();
void displayGameMenu();
void displayRules();
void playGame();

#endif //INC_15SHKI_PUZZLE15_H