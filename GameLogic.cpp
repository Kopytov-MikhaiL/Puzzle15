#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Puzzle15.h"

using namespace std;

// Проверка, можно ли решить головоломку
bool Puzzle15::isSolvable(const vector<int>& flatBoard) {
    int inversions = 0;

    // Считаем инверсии (игнорируя пустую ячейку)
    for (int i = 0; i < 15; i++) {
        for (int j = i + 1; j < 16; j++) {
            if (flatBoard[i] != 0 && flatBoard[j] != 0 &&
                flatBoard[i] > flatBoard[j]) {
                inversions++;
            }
        }
    }

    // Находим строку пустой клетки снизу (1-индексированная)
    int emptyRowPos = 0;
    for (int i = 0; i < 16; i++) {
        if (flatBoard[i] == 0) {
            emptyRowPos = 4 - (i / 4);
            break;
        }
    }

    // Правило для четности решаемости
    return (emptyRowPos % 2 == 0) ? (inversions % 2 == 1) : (inversions % 2 == 0);
}

// Получение текущего времени игры
string Puzzle15::getCurrentGameTime() const {
    auto now = chrono::steady_clock::now();
    auto gameStartTime = chrono::steady_clock::time_point();
    
    // Пытаемся преобразовать startTime в time_point
    try {
        // Простой способ - используем разницу от начала игры
        // В реальном проекте нужно было бы хранить время начала
        static auto start = now; // для простоты используем статическую переменную
        auto elapsed = chrono::duration_cast<chrono::seconds>(now - start);
        int minutes = static_cast<int>(elapsed.count() / 60);
        int seconds = static_cast<int>(elapsed.count() % 60);

        stringstream ss;
        ss << setfill('0') << setw(2) << minutes << ":"
           << setfill('0') << setw(2) << seconds;
        return ss.str();
    } catch (...) {
        return "00:00";
    }
}

// Конструктор
Puzzle15::Puzzle15() : board(4, vector<int>(4, 0)), emptyRow(3), emptyCol(3),
                       moveCount(0), gameWon(false) {
    initializeBoard();
}

// Инициализация игрового поля
void Puzzle15::initializeBoard() {
    vector<int> numbers(16);

    // Генерируем решаемую конфигурацию
    do {
        // Заполняем числами 1-15 и 0
        for (int i = 0; i < 15; i++) {
            numbers[i] = i + 1;
        }
        numbers[15] = 0;

        // Перемешиваем с использованием random_device для лучшей случайности
        random_device rd;
        mt19937 g(rd());
        shuffle(numbers.begin(), numbers.end(), g);
    } while (!isSolvable(numbers));

    // Заполняем поле и находим пустую ячейку
    int index = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = numbers[index];
            if (numbers[index] == 0) {
                emptyRow = i;
                emptyCol = j;
            }
            index++;
        }
    }

    // Сбрасываем счетчики
    moveCount = 0;
    gameWon = false;

    // Сохраняем время начала
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    stringstream ss;

    tm timeinfo{};
    #ifdef _WIN32
    localtime_s(&timeinfo, &time_t);
    #else
    localtime_r(&time_t, &timeinfo);
    #endif

    ss << put_time(&timeinfo, "%H:%M:%S");
    startTime = ss.str();
}

// Очистка консоли
void Puzzle15::clearConsole() const {
    system(CLEAR);
}

// Отображение игрового поля
void Puzzle15::displayBoard() {
    clearConsole();
    cout << "\n═══════════════════════════════════════════════════\n";
    cout << "         ИГРА В ПЯТНАШКИ - ТЕКУЩАЯ ИГРА         \n";
    cout << "═══════════════════════════════════════════════════\n";
    cout << "  Ходов: " << moveCount << " | Время игры: " << getCurrentGameTime()
         << " | Начало: " << startTime << "\n\n";

    cout << "       1    2    3    4\n";
    cout << "     ┌────┬────┬────┬────┐\n";

    for (int i = 0; i < 4; i++) {
        cout << "  " << (i + 1) << "  │";
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                cout << "    │";
            } else {
                cout << setw(4) << board[i][j] << "│";
            }
        }

        if (i < 3) {
            cout << "\n     ├────┼────┼────┼────┤\n";
        } else {
            cout << "\n     └────┴────┴────┴────┘\n";
        }
    }
    cout << "\n";
}

// Проверка возможности хода
bool Puzzle15::isValidMove(int row, int col) const {
    // Проверяем границы
    if (row < 0 || row >= 4 || col < 0 || col >= 4) {
        return false;
    }

    // Проверяем, является ли ячейка соседней с пустой
    int rowDiff = abs(row - emptyRow);
    int colDiff = abs(col - emptyCol);

    return (rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1);
}

// Выполнение хода
bool Puzzle15::makeMove(int row, int col) {
    if (!isValidMove(row, col)) {
        return false;
    }

    // Меняем местами выбранную плитку и пустую ячейку
    swap(board[row][col], board[emptyRow][emptyCol]);
    emptyRow = row;
    emptyCol = col;
    moveCount++;

    // Проверяем победу
    checkWin();

    return true;
}

// Проверка победы
bool Puzzle15::checkWin() {
    int expected = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // Последняя ячейка должна быть пустой
            if (i == 3 && j == 3) {
                if (board[i][j] != 0) {
                    gameWon = false;
                    return false;
                }
            }
            // Остальные ячейки должны содержать числа 1-15 по порядку
            else if (board[i][j] != expected++) {
                gameWon = false;
                return false;
            }
        }
    }
    gameWon = true;
    return true;
}

// Сохранение игры
bool Puzzle15::saveGame(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Сохраняем состояние поля
    for (const auto& row : board) {
        for (int val : row) {
            file << val << " ";
        }
        file << "\n";
    }

    // Сохраняем метаданные
    file << emptyRow << " " << emptyCol << "\n";
    file << moveCount << "\n";
    file << startTime << "\n";

    file.close();
    return true;
}

// Загрузка игры
bool Puzzle15::loadGame(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    try {
        // Загружаем состояние поля
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (!(file >> board[i][j])) {
                    throw runtime_error("Ошибка чтения поля");
                }
            }
        }

        // Загружаем метаданные
        if (!(file >> emptyRow >> emptyCol >> moveCount)) {
            throw runtime_error("Ошибка чтения метаданных");
        }

        file.ignore(); // Пропускаем перевод строки
        getline(file, startTime);

        // Проверяем корректность загруженной игры
        gameWon = checkWin();

        file.close();
        return true;

    } catch (const exception& e) {
        file.close();
        cerr << "Ошибка загрузки: " << e.what() << endl;
        return false;
    }
}

// Получение текущего состояния
bool Puzzle15::isGameWon() const { return gameWon; }
int Puzzle15::getMoveCount() const { return moveCount; }
string Puzzle15::getGameTime() const { return getCurrentGameTime(); }

// Генерация подсказки
pair<int, int> Puzzle15::getHint() const {
    vector<pair<int, int>> possibleMoves;

    // Проверяем все возможные ходы
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto& dir : directions) {
        int newRow = emptyRow + dir[0];
        int newCol = emptyCol + dir[1];

        if (newRow >= 0 && newRow < 4 && newCol >= 0 && newCol < 4) {
            possibleMoves.emplace_back(newRow, newCol);
        }
    }

    if (!possibleMoves.empty()) {
        // Возвращаем первую возможную плитку
        return possibleMoves[0];
    }

    return {-1, -1};
}

// Отображение состояния ячейки (для отладки)
void Puzzle15::debugBoard() const {
    cout << "Debug info:\n";
    cout << "Empty cell: (" << emptyRow << ", " << emptyCol << ")\n";
    cout << "Board state:\n";
    for (const auto& row : board) {
        for (int val : row) {
            cout << setw(3) << val;
        }
        cout << endl;
    }
}