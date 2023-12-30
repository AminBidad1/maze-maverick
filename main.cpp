#include <termios.h>
#include <unistd.h>
#include <iostream>
#include "colors.hpp"

using namespace std;

void reset_terminal();
int start_menu();
int getch(void);

int main()
{
    start_menu();
}

int start_menu()
{
    reset_terminal();
    int selected_option = 2;
    const string options[12] = {
        "1. Create a New Map",
        "   1.1 Easy",
        "   1.2 Hard",
        "2. PlayGround",
        "   2.1 Choose from Existing Maps",
        "   2.2 Import a Custom Map",
        "3. Solve a Maze",
        "   3.1 Choose from Existing Maps",
        "   3.2 Import a Custom Map",
        "4. History",
        "5. Leaderboard",
        "6. Exit"};
    while (true)
    {
        for (int i = 0; i < 12; i++)
        {
            if (selected_option == i + 1)
            {
                cout << color::rize(options[i], "Red", "Blue") << endl;
            }
            else
            {
                cout << options[i] << endl;
            }
        }
        switch (getch())
        {
        case 65:
            selected_option--; // key up
            if (selected_option == 4)
                selected_option = 3;
            else if (selected_option == 7)
                selected_option = 6;
            break;
        case 66:
            selected_option++; // key down
            if (selected_option == 4)
                selected_option = 5;
            else if (selected_option == 7)
                selected_option = 8;
            break;
        case '\n':
            return selected_option; // enter key
        default:
            break;
        }
        if (selected_option > 12)
            selected_option = 12;
        else if (selected_option <= 1)
            selected_option = 2;
        reset_terminal();
    }
}

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

void reset_terminal()
{
#if defined _WIN32
    system("cls");
    // clrscr(); // including header file : conio.h
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    // std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences
#elif defined(__APPLE__)
    system("clear");
#endif
}