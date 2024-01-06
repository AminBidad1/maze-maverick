#include <unistd.h>
#include <iostream>
#include <time.h>
#include "colors.hpp"

using namespace std;


#ifdef __MINGW32__
    #include <conio.h>
    const int UP_KEY = 72;
    const int DOWN_KEY = 80;
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    #include <termios.h>
    const int UP_KEY = 65;
    const int DOWN_KEY = 66;
    int getch(void);
#endif

struct position {
    int row, col;
};
void reset_terminal();
int start_menu();
void create_map(int** table, int rows, int columns);
int generate_random(int min_value, int max_value, int* ignored_numbers, int size);
bool isin(int list[], int number, int size);
bool isin_position(position positions[], position point, int size);
position generate_random_position(position min_position, position max_position, position* points, int size);
void test_create_map();


int main(){
    srand(time(NULL));
    int selected_option = start_menu();
    if (selected_option == 2){
        test_create_map();
    }
    return 0;
}

void test_create_map(){
    reset_terminal();
    int rows, columns;
    cout << "Enter count of rows: ";
    cin >> rows;
    cout << "Enter count of columns: ";
    cin >> columns;
    reset_terminal();
    int** table;
    table = new int*[rows];
    for (int i=0; i<rows; i++){
        table[i] = new int[columns];
    }
    create_map(table, rows, columns);
    for (int i=0; i<rows; i++){
        for (int j=0; j<columns; j++){
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
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

        case UP_KEY:
            selected_option--; // key up
            if (selected_option == 4)
                selected_option = 3;
            else if (selected_option == 7)
                selected_option = 6;
            break;

        case DOWN_KEY:
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

bool isin(int list[], int number, int size){
    if (size == 0){
        return false;
    }
    for (int i=0; i<size; i++){
        if (list[i] == number){
            return true;
        }
    }
    return false;
}

int generate_random(int min_value, int max_value, int* ignored_numbers, int size){
    int random_number;
    do {
        random_number = ((rand() % (max_value - min_value +1)) + min_value);
        if (!isin(ignored_numbers, random_number, size)) {
            return random_number;
        }
    } while (true);
}


bool isin_position(position positions[], position point, int size){
    if (size == 0){
        return false;
    }
    for (int i=0; i<size; i++){
        if (positions[i].row == point.row && positions[i].col == point.col){
            return true;
        }
    }
    return false;
}

position generate_random_position(position min_position, position max_position, position* points, int size){
    position point;
    do {
        point.row = ((rand() % (max_position.row - min_position.row +1)) + min_position.row);
        point.col = ((rand() % (max_position.col - min_position.col +1)) + min_position.col);
        if (!isin_position(points, point, size)) {
            return point;
        }
    } while(true);
}


/*Create an easy map*/
void create_map(int** table, int rows, int columns){
    // initial values
    const int path_length = rows + columns - 2;
    const int max_value = 3;
    const int min_value = -3;
    const int max_block = 5;
    const int min_block = 2;
    int path_random_numbers[path_length];
    int path_sum_numbers = 0;
    int ignored_numbers[1] = {0};
    // generate random numbers of path
    for (int i=0; i<path_length; i++){
        path_random_numbers[i] = generate_random(min_value, max_value, ignored_numbers, 1);
        path_sum_numbers += path_random_numbers[i];
    }
    // generate random movements of path
    int down_positions[rows-1];
    for (int i=0; i<rows-1; i++){
        down_positions[i] = generate_random(0, path_length-1, down_positions, i);
    }
    // generate random indexes of path
    position path_positions[path_length+1];
    int row_index = 0;
    int col_index = 0;
    table[0][0] = path_random_numbers[0];
    path_positions[0].row = 0;
    path_positions[0].col = 0;
    for (int i=1; i<path_length; i++){
        if (isin(down_positions, i-1, rows-1)){
            row_index++;
            table[row_index][col_index] = path_random_numbers[i];
        }
        else {
            col_index++;
            table[row_index][col_index] = path_random_numbers[i];
        }
        path_positions[i].row = row_index;
        path_positions[i].col = col_index;
    }
    path_positions[path_length].row = rows - 1;
    path_positions[path_length].col = columns -1;
    table[rows-1][columns-1] = path_sum_numbers;
    // generate random block indexes
    int empty_array[0] = {};
    int count_block = generate_random(min_block, max_block, empty_array, 0);
    position blocks[count_block + path_length + 1];
    position temp_point;
    for (int i=0; i<path_length+1; i++){
        blocks[i] = path_positions[i];
    }
    position min_position;
    min_position.row = 0;
    min_position.col = 0;
    position max_position;
    max_position.row = rows - 1;
    max_position.col = columns - 1;
    for (int i=path_length + 1; i < count_block + path_length + 1; i++){
        blocks[i] = generate_random_position(min_position, max_position, blocks, i);
        table[blocks[i].row][blocks[i].col] = 1000000000;
    }
    // generate other numbers of table
    for (int i=0; i<rows; i++){
        for (int j=0; j<columns; j++){
            temp_point.row = i;
            temp_point.col = j;
            if (!isin_position(path_positions, temp_point, path_length+1)){
                if (table[i][j] != 1000000000){
                    table[i][j] = generate_random(min_value, max_value, ignored_numbers, 1);
                }
                else {
                    table[i][j] = 0;
                }
            }
        }
    }
}

#if defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    /* reads from keypress, doesn't echo */
    int getch(void)
    {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr( STDIN_FILENO, &oldattr );
        newattr = oldattr;
        newattr.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
        return ch;
    }
#endif

void reset_terminal(){
    #if defined _WIN32
        system("cls");
        //clrscr(); // including header file : conio.h
    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system("clear");
        //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
    #elif defined (__APPLE__)
        system("clear");
    #endif
}