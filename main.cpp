#include <iostream>
#include <limits>
#include <windows.h>
#include "Puzzle15.h"

using namespace std;

int main() {
    // Настройка кодировки для Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    bool running = true;

    while (running) {
        displayMainMenu();

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: // Новая игра
                playGame();
                break;

            case 2: // Загрузить игру
                {
                    system(CLEAR);
                    cout << "\n═══════════════════════════════════════════════════\n";
                    cout << "                ЗАГРУЗКА ИГРЫ                    \n";
                    cout << "═══════════════════════════════════════════════════\n";
                    cout << "Введите имя файла для загрузки: ";

                    string filename;
                    cin >> filename;

                    Puzzle15 loadedGame;
                    if (loadedGame.loadGame(filename)) {
                        cout << "Игра успешно загружена!\n";
                        cout << "Нажмите Enter чтобы начать игру...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin.get();

                        // Запускаем загруженную игру
                        bool inLoadedGame = true;
                        while (inLoadedGame) {
                            loadedGame.displayBoard();

                            if (loadedGame.isGameWon()) {
                                cout << "\n═══════════════════════════════════════════════════\n";
                                cout << "      ПОЗДРАВЛЯЕМ! ВЫ ВЫИГРАЛИ!                     \n";
                                cout << "═══════════════════════════════════════════════════\n";
                                cout << "\nНажмите Enter чтобы вернуться в меню...";
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cin.get();
                                inLoadedGame = false;
                                break;
                            }

                            displayGameMenu();
                            string command;
                            cin >> command;

                            if (command == "menu") {
                                inLoadedGame = false;
                            } else if (command == "save") {
                                cout << "Введите имя файла для сохранения: ";
                                string saveFile;
                                cin >> saveFile;
                                if (loadedGame.saveGame(saveFile)) {
                                    cout << "Игра сохранена!\n";
                                } else {
                                    cout << "Ошибка сохранения!\n";
                                }
                                cout << "Нажмите Enter чтобы продолжить...";
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cin.get();
                            } else if (command == "hint") {
                                auto hint = loadedGame.getHint();
                                if (hint.first != -1) {
                                    cout << "Подсказка: " << hint.first + 1 << " " << hint.second + 1 << "\n";
                                }
                                cout << "Нажмите Enter чтобы продолжить...";
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cin.get();
                            } else {
                                try {
                                    int row = stoi(command) - 1;
                                    int col;
                                    if (cin >> col) {
                                        col--;
                                        if (!loadedGame.makeMove(row, col)) {
                                            cout << "Неверный ход!\n";
                                        }
                                    }
                                } catch (...) {
                                    cout << "Неверная команда!\n";
                                }
                                cout << "Нажмите Enter чтобы продолжить...";
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cin.get();
                            }
                        }
                    } else {
                        cout << "Ошибка загрузки игры! Файл не найден или поврежден.\n";
                        cout << "Нажмите Enter чтобы продолжить...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin.get();
                    }
                }
                break;

            case 3: // Правила
                displayRules();
                break;

            case 4: // Выход
                running = false;
                cout << "\nСпасибо за игру! До свидания!\n";
                break;

            default:
                cout << "Неверный выбор! Попробуйте снова.\n";
                cout << "Нажмите Enter чтобы продолжить...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
        }
    }

    return 0;
}