#include <iostream>
#include <limits>
#include "Puzzle15.h"

using namespace std;

// Функция для отображения главного меню
void displayMainMenu() {
    system(CLEAR);
    cout << "\n═══════════════════════════════════════════════════\n";
    cout << "            ИГРА В ПЯТНАШКИ - МЕНЮ             \n";
    cout << "═══════════════════════════════════════════════════\n";
    cout << "1. Начать новую игру\n";
    cout << "2. Загрузить сохраненную игру\n";
    cout << "3. Правила игры\n";
    cout << "4. Выход\n";
    cout << "═══════════════════════════════════════════════════\n";
    cout << "Выберите действие (1-4): ";
}

// Функция для отображения игрового меню
void displayGameMenu() {
    cout << "\n═══════════════════════════════════════════════════\n";
    cout << "              ИГРОВОЕ МЕНЮ                        \n";
    cout << "═══════════════════════════════════════════════════\n";
    cout << "Доступные команды:\n";
    cout << "  row col    - Сделать ход (например: 2 3)\n";
    cout << "  menu       - Вернуться в главное меню\n";
    cout << "  save       - Сохранить игру\n";
    cout << "  hint       - Получить подсказку\n";
    cout << "  debug      - Показать отладочную информацию\n";
    cout << "═══════════════════════════════════════════════════\n";
    cout << "Введите команду или координаты: ";
}

// Функция для отображения правил
void displayRules() {
    system(CLEAR);
    cout << "\n═══════════════════════════════════════════════════\n";
    cout << "                  ПРАВИЛА ИГРЫ                    \n";
    cout << "═══════════════════════════════════════════════════\n";
    cout << "Цель игры:\n";
    cout << "  Расположить все плитки по порядку от 1 до 15,\n";
    cout << "  оставив пустую ячейку в правом нижнем углу.\n\n";
    cout << "Как играть:\n";
    cout << "  1. Плитки можно перемещать только если они\n";
    cout << "     соседствуют с пустой ячейкой.\n";
    cout << "  2. Для перемещения плитки введите ее координаты:\n";
    cout << "     сначала номер строки (1-4), затем номер столбца (1-4)\n";
    cout << "     Например: \"2 3\" - плитка во 2 строке, 3 столбце\n\n";
    cout << "Пример выигрышной позиции:\n";
    cout << "     1   2   3   4\n";
    cout << "     5   6   7   8\n";
    cout << "     9  10  11  12\n";
    cout << "    13  14  15   _\n\n";
    cout << "Нажмите Enter чтобы вернуться в меню...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Функция для обработки игрового процесса
void playGame() {
    Puzzle15 game;
    bool inGame = true;

    while (inGame) {
        game.displayBoard();

        if (game.isGameWon()) {
            cout << "\n═══════════════════════════════════════════════════\n";
            cout << "      ПОЗДРАВЛЯЕМ! ВЫ ВЫИГРАЛИ!                     \n";
            cout << "═══════════════════════════════════════════════════\n";
            cout << "  Итог: " << game.getMoveCount() << " ходов за "
                 << game.getGameTime() << "\n";
            cout << "\n1. Новая игра\n";
            cout << "2. Главное меню\n";
            cout << "Выберите действие: ";

            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 1) {
                game.initializeBoard();
                continue;
            } else {
                return;
            }
        }

        displayGameMenu();

        string input;
        cin >> input;

        if (input == "menu") {
            inGame = false;
        }
        else if (input == "save") {
            cout << "Введите имя файла для сохранения: ";
            string filename;
            cin >> filename;

            if (game.saveGame(filename)) {
                cout << "Игра сохранена в файл: " << filename << "\n";
            } else {
                cout << "Ошибка сохранения игры!\n";
            }
            cout << "Нажмите Enter чтобы продолжить...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        else if (input == "hint") {
            auto hint = game.getHint();
            if (hint.first != -1) {
                cout << "Подсказка: попробуйте переместить плитку в позиции "
                     << hint.first + 1 << " " << hint.second + 1 << "\n";
            } else {
                cout << "Подсказка недоступна\n";
            }
            cout << "Нажмите Enter чтобы продолжить...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        else if (input == "debug") {
            game.debugBoard();
            cout << "Нажмите Enter чтобы продолжить...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        else {
            // Пытаемся обработать как координаты
            try {
                int row = stoi(input) - 1;
                int col;
                if (cin >> col) {
                    col--; // Преобразуем в 0-индекс

                    if (!game.makeMove(row, col)) {
                        cout << "Ошибка! Невозможно переместить эту плитку.\n";
                        cout << "Убедитесь что плитка соседствует с пустой ячейкой.\n";
                    }
                } else {
                    cout << "Неверный формат ввода! Введите два числа через пробел.\n";
                }
            } catch (const invalid_argument&) {
                cout << "Неизвестная команда! Используйте команды из меню.\n";
            } catch (const out_of_range&) {
                cout << "Число вне допустимого диапазона! Используйте числа 1-4.\n";
            }

            cout << "Нажмите Enter чтобы продолжить...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

        // Очищаем буфер ввода
        cin.clear();
    }
}