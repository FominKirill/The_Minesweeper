#include <iostream>
#include <Windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

string intToString(int num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

static string** ARRAY(int size1, int size2)
{
    string** arr = new string * [size1];
    for (int i = 0; i < size1; i++)
    {
        arr[i] = new string[size2];
    }
    return arr;
}

void generateMines(int** mine_field, int x, int y, int mines_count)
{
    int mines_placed = 0;
    while (mines_placed < mines_count)
    {
        int rand_x = rand() % x, rand_y = rand() % y;

        if (mine_field[rand_x][rand_y] != -1)
        {
            mine_field[rand_x][rand_y] = -1; mines_placed++;
        }
    }
}

int countAdjacentMines(int** mine_field, int x, int y, int field_size_x, int field_size_y)
{
    int count = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int new_x = x + i, new_y = y + j;
            if (new_x >= 0 && new_x < field_size_x && new_y >= 0 && new_y < field_size_y)
            {
                if (mine_field[new_x][new_y] == -1) { count++; }
            }
        }
    }
    return count;
}

void initializeGame(int** mine_field, string** display_field, int x, int y, int mines_count)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            mine_field[i][j] = 0;
        }
    }
    generateMines(mine_field, x, y, mines_count);
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (mine_field[i][j] != -1)
            {
                mine_field[i][j] = countAdjacentMines(mine_field, i, j, x, y);
            }
        }
    }
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            display_field[i][j] = ">";
        }
    }
}

void displayGameField(string** display_field, int x, int y)
{
    cout << " ";
    for (int j = 0; j < y; j++) { cout << setw(3) << j + 1; }
    cout << endl;

    for (int i = 0; i < x; i++)
    {

        cout << setw(2) << i + 1;
        cout << " ";
        for (int j = 0; j < y; j++)
        {
            cout << display_field[i][j] << "  ";
        }
        cout << endl;
    }
}

bool openCell(int** mine_field, string** display_field, int x, int y, int field_size_x, int field_size_y)
{
    if (x < 0 || x >= field_size_x || y < 0 || y >= field_size_y)
    {
        return true;
    }
    if (display_field[x][y] != ">")
    {
        return true;
    }
    if (mine_field[x][y] == -1)
    {
        display_field[x][y] = "X"; return false;
    }
    else if (mine_field[x][y] > 0)
    {
        display_field[x][y] = intToString(mine_field[x][y]); return true;
    }
    else
    {
        display_field[x][y] = " ";
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i != 0 || j != 0)
                {
                    openCell(mine_field, display_field, x + i, y + j, field_size_x, field_size_y);
                }
            }
        }
        return true;
    }
}

bool checkWin(string** display_field, int** mine_field, int x, int y, int mines_count)
{
    int closed_cells = 0;

    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (display_field[i][j] == ">")
            {
                closed_cells++;
            }
        }
    }
    return closed_cells == mines_count;
}

int main()
{
    SetConsoleOutputCP(1251); SetConsoleCP(1251); srand(time(0));
    int next_game = 0;

    do {
        int choice = 0, x = 0, y = 0, mines_count = 0;

        cout << "Выберите сложность:" << endl
            << "1) 9x9 (10 мин)" << endl
            << "2) 16x16 (40 мин)" << endl
            << "3) 32x32 (100 мин)" << endl
            << "->";
        cin >> choice;

        switch (choice)
        {
        case 1:
            x = 9; y = 9; mines_count = 10;
            break;
        case 2:
            x = 16; y = 16; mines_count = 40;
            break;
        case 3:
            x = 32; y = 32; mines_count = 100;
            break;
        default:
            cout << "Неверный выбор! Запускается игра 9x9." << endl;
            x = 9; y = 9; mines_count = 10;
            break;
        }

        string** display_field = ARRAY(x, y);
        int** mine_field = new int* [x];
        for (int i = 0; i < x; i++)
        {
            mine_field[i] = new int[y];
        }
        initializeGame(mine_field, display_field, x, y, mines_count);
        bool game_running = true;
        while (game_running)
        {
            displayGameField(display_field, x, y);
            int input_x, input_y;
            cout << "Введите координаты (строка столбец): ";
            cin >> input_x >> input_y;
            input_x--;
            input_y--;
            if (input_x < 0 || input_x >= x || input_y < 0 || input_y >= y)
            {
                cout << "Неверные координаты! Попробуйте снова." << endl;
                continue;
            }

            game_running = openCell(mine_field, display_field, input_x, input_y, x, y);

            if (!game_running)
            {
                cout << "Вы наткнулись на мину! Игра окончена." << endl;

                for (int i = 0; i < x; i++)
                {
                    for (int j = 0; j < y; j++)
                    {
                        if (mine_field[i][j] == -1)
                        {
                            display_field[i][j] = "X";
                        }
                    }
                }
                displayGameField(display_field, x, y);
            }
            else if (checkWin(display_field, mine_field, x, y, mines_count))
            {
                cout << "Поздравляем! Вы выиграли!" << endl;
                displayGameField(display_field, x, y);
                game_running = false;
            }
        }
        for (int i = 0; i < x; i++)
        {
            delete[] mine_field[i];
            delete[] display_field[i];
        }
        delete[] mine_field; delete[] display_field;
        cout << "Играть ещё?" << endl << "(1 - да)" << endl << "(2 - нет)" << endl << "Ввод: ";
        cin >> next_game;
    } while (next_game == 1);

    return 0;
}