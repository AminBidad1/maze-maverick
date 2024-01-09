#include <unistd.h>
#include <bits/stdc++.h>
#include <time.h>
#include "colors.hpp"
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;

#ifdef __MINGW32__
#include <conio.h>
const int UP_KEY = 72;
const int DOWN_KEY = 80;
const int ENTER_KEY = 13;
const int RIGHT_KEY = 77;
const int LEFT_KEY = 75;
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
#include <termios.h>
const int UP_KEY = 65;
const int DOWN_KEY = 66;
const int RIGHT_KEY = 67;
const int LEFT_KEY = 68;
int getch(void);
#endif

class GameTimer
{
private:
    chrono::high_resolution_clock::time_point startTime;

public:
    void start()
    {
        startTime = chrono::high_resolution_clock::now();
    }

    int stop()
    {
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime);
        return static_cast<int>(duration.count());
    }
};

struct position
{
    int row, col;
};

void reset_terminal();
int start_menu();
bool create_map(int **table, int rows, int columns);
bool create_map(int **table, int rows, int columns, int path_length,
                int max_value, int min_value, int max_block, int min_block);
int generate_random(int min_value, int max_value, int *ignored_numbers, int size);
bool isin(int list[], int number, int size);
bool isin_position(position positions[], position point, int size);
bool isin_position(vector<position> positions, position point);
position generate_random_position(position min_position, position max_position, position *points, int size);
void read_UserInfo(string &user_name);
void read_History();
void write_UserInfo(const string &user_name, const int &status, const int time_spent);
void write_History(const string &user_name, const string &map_name, const string &result, const string &time_spent);
bool isSafePosition(int i, int j, int **table, int rows, int columns);
bool isaPath(int **table, int i, int j, bool **visited, int rows, int columns,
             vector<position> &path, int path_length, int sum);
vector<position> findPath(int **table, int rows, int columns, int path_length);
void show_table(int **table, int rows, int columns, vector<position> path,
                position user_index);
bool isSimplePath(int **table, int i, int j, bool **visited, int rows, int columns,
                  vector<position> &path, int path_length);
bool play_game(int **table, int rows, int columns,
               position &current_position, vector<position> path);
bool can_move(int **table, int rows, int columns, int i, int j,
              vector<position> path);
void test_create_map();
void test_hard_create_map();
void test_play_game();

template <typename T>
void swap(T *a, T *b)
{
    T temp = *a;
    *a = *b;
    *b = temp;
}

template <typename T>
void shuffle_array(T inputs[], int count)
{
    int random_number;
    for (int i = count - 1; i > 0; i--)
    {
        random_number = rand() % (i + 1);
        swap(inputs[i], inputs[random_number]);
    }
}

int main()
{
    srand(time(NULL));
    int selected_option = start_menu();
    if (selected_option == 2)
    {
        test_create_map();
    }
    else if (selected_option == 3)
    {
        test_hard_create_map();
    }
    else if (selected_option == 5)
    {
        test_play_game();
    }
    else if (selected_option == 10)
    {
        read_History();
    }
    return 0;
}

void test_play_game()
{
    reset_terminal();
    int rows, columns, path_length, max_value, min_value, max_block, min_block;
    cout << "Enter count of rows: ";
    cin >> rows;
    cout << "Enter count of columns: ";
    cin >> columns;
    cout << "Enter path length: ";
    cin >> path_length;
    cout << "Enter max value of cells: ";
    cin >> max_value;
    cout << "Enter min value of cells: ";
    cin >> min_value;
    cout << "Enter max block of cells: ";
    cin >> max_block;
    cout << "Enter min block of cells: ";
    cin >> min_block;
    reset_terminal();
    int **table;
    table = new int *[rows];
    for (int i = 0; i < rows; i++)
    {
        table[i] = new int[columns];
    }
    if (!create_map(table, rows, columns, path_length, max_value, min_value, max_block, min_block))
    {
        cout << "Invalid path or Cannot create the table." << endl;
        delete[] table;
        return;
    }
    vector<position> path;
    position user_index;
    user_index.row = 0;
    user_index.col = 0;
    path.push_back(user_index);
    GameTimer gameTimer;
    gameTimer.start();
    play_game(table, rows, columns, user_index, path);
    int elapsedTimeInSeconds = gameTimer.stop();
    delete[] table;
}

void test_create_map()
{
    reset_terminal();
    int rows, columns;
    cout << "Enter count of rows: ";
    cin >> rows;
    cout << "Enter count of columns: ";
    cin >> columns;
    reset_terminal();
    int **table;
    table = new int *[rows];
    for (int i = 0; i < rows; i++)
    {
        table[i] = new int[columns];
    }
    if (!create_map(table, rows, columns))
    {
        cout << "Can't Create" << endl;
        return;
    }
    vector<position> path = findPath(table, rows, columns, rows + columns - 2);
    position user_index;
    show_table(table, rows, columns, path, user_index);
    if (path.empty())
    {
        cout << "Path does not exist!" << endl;
    }
    delete[] table;
}

void test_hard_create_map()
{
    reset_terminal();
    int rows, columns, path_length, max_value, min_value, max_block, min_block;
    cout << "Enter count of rows: ";
    cin >> rows;
    cout << "Enter count of columns: ";
    cin >> columns;
    cout << "Enter path length: ";
    cin >> path_length;
    cout << "Enter max value of cells: ";
    cin >> max_value;
    cout << "Enter min value of cells: ";
    cin >> min_value;
    cout << "Enter max block of cells: ";
    cin >> max_block;
    cout << "Enter min block of cells: ";
    cin >> min_block;
    reset_terminal();
    int **table;
    table = new int *[rows];
    for (int i = 0; i < rows; i++)
    {
        table[i] = new int[columns];
    }
    if (!create_map(table, rows, columns, path_length, max_value, min_value, max_block, min_block))
    {
        cout << "Invalid path! Cannot create the table." << endl;
        delete[] table;
        return;
    }
    vector<position> path = findPath(table, rows, columns, path_length);
    position user_index;
    show_table(table, rows, columns, path, user_index);
    if (path.empty())
    {
        cout << "Path does not exist!" << endl;
    }
    delete[] table;
}

void read_UserInfo(string &user_name)
{
    ifstream infile("./Users/" + user_name + ".txt");
    string line;
    if (infile.is_open())
    {
        if (getline(infile, line))
        {
            cout << "Total number of games: " << line << endl;
            getline(infile, line);
            cout << "Total number of wins: " << line << endl;
            getline(infile, line);
            cout << "Date of last win: " << line << endl;
            getline(infile, line);
            cout << "Total time of games: " << line << endl;
        }
    }
    else
    {
        cerr << "Unable to open the file for reading." << endl;
    }
}

void read_History()
{
    ifstream infile("./Stats/History.txt");
    string line;
    if (infile.is_open())
    {
        for (int i = 1; i < 11; i++)
        {
            if (getline(infile, line))
            {
                cout << "Game " << i << ":\n";
                cout << line << endl;
                getline(infile, line);
                cout << line << endl;
                getline(infile, line);
                cout << line << endl;
                getline(infile, line);
                cout << line << endl;
                getline(infile, line);
                cout << line << endl
                     << endl;
            }
        }
    }
    else
    {
        cerr << "Unable to open the file for reading." << endl;
    }
}

void write_UserInfo(const string &user_name, const int &status, const int time_spent)
{
    string filePath = "./Users/" + user_name + ".txt";
    fstream outputFile(filePath, ios::in | ios::out);

    if (!outputFile)
    {
        cerr << "Error: Could not open the file " << filePath << endl;
        return;
    }

    int totalGames, totalWins;
    string lastWinTimeStr, line;
    time_t lastWinTime, totalTimeSpent;
    outputFile >> totalGames >> totalWins >> lastWinTimeStr >> totalTimeSpent;
    struct tm tm = {};
    istringstream ss(lastWinTimeStr);
    ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
    lastWinTime = mktime(&tm);
    totalGames++;

    if (status == 1)
    {
        totalWins++;
        lastWinTime = time(0);
    }

    totalTimeSpent += time_spent;
    // Move the file cursor to the beginning
    outputFile.seekp(0);
    outputFile << totalGames << " " << totalWins << " " << lastWinTime << " " << totalTimeSpent << endl;
    outputFile.close();
}

void write_History(const string &user_name, const string &map_name, const string &result, const string &time_spent)
{
    ifstream infile("./Stats/History.txt");

    if (!infile)
    {
        cerr << "Unable to open the file for reading." << endl;
        return;
    }

    string existing_content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    infile.close();

    ofstream outfile("./Stats/History.txt");

    if (outfile.is_open())
    {
        time_t now = time(0);
        tm *local_time = localtime(&now);
        char date_time[80];
        strftime(date_time, 80, "%Y-%m-%d %H:%M:%S", local_time);

        outfile << date_time << "\n";
        outfile << "User: " << user_name << "\n";
        outfile << "Map name: " << map_name << "\n";
        outfile << "Time spent: " << time_spent << "\n";
        outfile << "Result: " << result << "\n";

        outfile << existing_content;

        outfile.close();
    }
    else
    {
        cerr << "Unable to open the file for writing." << endl;
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
            selected_option--;
            if (selected_option == 4)
                selected_option = 3;
            else if (selected_option == 7)
                selected_option = 6;
            break;
        case DOWN_KEY:
            selected_option++;
            if (selected_option == 4)
                selected_option = 5;
            else if (selected_option == 7)
                selected_option = 8;
            break;
        case ENTER_KEY:
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

bool can_move(int **table, int rows, int columns, int i, int j,
              vector<position> path)
{
    if (isSafePosition(i, j, table, rows, columns))
    {
        if (table[i][j] == 0)
        {
            return false;
        }
        position point;
        point.row = i;
        point.col = j;
        if (isin_position(path, point))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool play_game(int **table, int rows, int columns,
               position &current_position, vector<position> path)
{
    reset_terminal();
    show_table(table, rows, columns, path, current_position);
    int sum = table[0][0];
    while (true)
    {
        if (current_position.row == rows - 1 && current_position.col == columns - 1)
        {
            if (sum == table[rows - 1][columns - 1])
            {
                cout << color::rize(" You Won! ", "White", "Green") << endl;
                return true;
            }
            else
            {
                cout << color::rize(" You Lost! ", "Light Yellow", "Red") << endl;
                return false;
            }
        }
        if (
            can_move(table, rows, columns, current_position.row - 1, current_position.col, path) ||
            can_move(table, rows, columns, current_position.row + 1, current_position.col, path) ||
            can_move(table, rows, columns, current_position.row, current_position.col - 1, path) ||
            can_move(table, rows, columns, current_position.row, current_position.col + 1, path))
        {
            switch (getch())
            {
            case UP_KEY:
                if (can_move(table, rows, columns, current_position.row - 1, current_position.col, path))
                {
                    current_position.row -= 1;
                    path.push_back(current_position);
                    if (!(current_position.row == rows - 1 && current_position.col == columns - 1))
                        sum += table[current_position.row][current_position.col];
                    reset_terminal();
                    show_table(table, rows, columns, path, current_position);
                }
                break;
            case DOWN_KEY:
                if (can_move(table, rows, columns, current_position.row + 1, current_position.col, path))
                {
                    current_position.row += 1;
                    path.push_back(current_position);
                    if (!(current_position.row == rows - 1 && current_position.col == columns - 1))
                        sum += table[current_position.row][current_position.col];
                    reset_terminal();
                    show_table(table, rows, columns, path, current_position);
                }
                break;
            case LEFT_KEY:
                if (can_move(table, rows, columns, current_position.row, current_position.col - 1, path))
                {
                    current_position.col -= 1;
                    path.push_back(current_position);
                    if (!(current_position.row == rows - 1 && current_position.col == columns - 1))
                        sum += table[current_position.row][current_position.col];
                    reset_terminal();
                    show_table(table, rows, columns, path, current_position);
                }
                break;
            case RIGHT_KEY:
                if (can_move(table, rows, columns, current_position.row, current_position.col + 1, path))
                {
                    current_position.col += 1;
                    path.push_back(current_position);
                    if (!(current_position.row == rows - 1 && current_position.col == columns - 1))
                        sum += table[current_position.row][current_position.col];
                    reset_terminal();
                    show_table(table, rows, columns, path, current_position);
                }
                break;
            default:
                break;
            }
        }
        else
        {
            cout << color::rize(" You Lost! ", "Light Yellow", "Red") << endl;
            return false;
        }
    }
}

void show_table(int **table, int rows, int columns, vector<position> path,
                position user_index)
{
    int max_length = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (to_string(table[i][j]).length() > max_length)
            {
                max_length = to_string(table[i][j]).length();
            }
        }
    }
    const int cell_width = max_length + 4;
    int left_spaces, right_spaces, number_size, total_spaces;
    position point;
    for (int j = 0; j < columns; j++)
    {
        cout << '+' << string(cell_width, '-');
    }
    cout << '+' << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            number_size = to_string(table[i][j]).length();
            total_spaces = cell_width - number_size;
            right_spaces = total_spaces / 2;
            left_spaces = total_spaces - right_spaces;
            point.row = i;
            point.col = j;
            cout << '|';
            if (i == 0 && j == 0)
            {
                cout << color::rize(string(left_spaces, ' '), "Blue", "Red");
                cout << color::rize(to_string(table[i][j]), "Blue", "Red");
                cout << color::rize(string(right_spaces, ' '), "Blue", "Red");
            }
            else if (i == rows - 1 && j == columns - 1)
            {
                cout << color::rize(string(left_spaces, ' '), "Red", "Blue");
                cout << color::rize(to_string(table[i][j]), "Red", "Blue");
                cout << color::rize(string(right_spaces, ' '), "Red", "Blue");
            }
            else if (user_index.row == i && user_index.col == j)
            {
                cout << color::rize(string(left_spaces, ' '), "White", "Green");
                cout << color::rize(to_string(table[i][j]), "White", "Green");
                cout << color::rize(string(right_spaces, ' '), "White", "Green");
            }
            else if (table[i][j] == 0)
            {
                cout << color::rize(string(left_spaces, ' '), "Red", "Light Yellow");
                cout << color::rize(to_string(table[i][j]), "Red", "Light Yellow");
                cout << color::rize(string(right_spaces, ' '), "Red", "Light Yellow");
            }
            else if (isin_position(path, point))
            {
                cout << color::rize(string(left_spaces, ' '), "Red", "Light Blue");
                cout << color::rize(to_string(table[i][j]), "Red", "Light Blue");
                cout << color::rize(string(right_spaces, ' '), "Red", "Light Blue");
            }
            else
            {
                cout << string(left_spaces, ' ');
                cout << to_string(table[i][j]);
                cout << string(right_spaces, ' ');
            }
        }
        cout << '|' << endl;
        for (int j = 0; j < columns; j++)
        {
            cout << '+' << string(cell_width, '-');
        }
        cout << '+' << endl;
    }
}

bool isSafePosition(int i, int j, int **table, int rows, int columns)
{
    if (i >= 0 && i < rows && j >= 0 && j < columns)
        return true;
    return false;
}

bool isaPath(int **table, int i, int j, bool **visited, int rows, int columns,
             vector<position> &path, int path_length, int sum)
{
    if ((isSafePosition(i, j, table, rows, columns) && table[i][j] != 0) && (!visited[i][j] && path.size() <= path_length))
    {
        visited[i][j] = true;
        if (i == rows - 1 && j == columns - 1)
        {
            if (path.size() == path_length && sum == table[i][j])
            {
                return true;
            }
            else
            {
                visited[i][j] = false;
                return false;
            }
        }
        position point;
        point.row = i;
        point.col = j;
        path.push_back(point);
        bool up = isaPath(table, i - 1, j, visited, rows, columns, path, path_length, sum + table[i][j]);
        if (up)
        {
            return true;
        }
        bool left = isaPath(table, i, j - 1, visited, rows, columns, path, path_length, sum + table[i][j]);
        if (left)
        {
            return true;
        }
        bool down = isaPath(table, i + 1, j, visited, rows, columns, path, path_length, sum + table[i][j]);
        if (down)
        {
            return true;
        }
        bool right = isaPath(table, i, j + 1, visited, rows, columns, path, path_length, sum + table[i][j]);
        if (right)
        {
            return true;
        }
        visited[i][j] = false;
        path.pop_back();
    }
    return false;
}

vector<position> findPath(int **table, int rows, int columns, int path_length)
{
    bool **visited = new bool *[rows];
    for (int i = 0; i < rows; i++)
    {
        visited[i] = new bool[columns];
        for (int j = 0; j < columns; j++)
        {
            visited[i][j] = false;
        }
    }
    vector<position> path;
    vector<position> empty;
    if (isaPath(table, 0, 0, visited, rows, columns, path, path_length, 0))
    {
        return path;
    }
    else
    {
        return empty;
    }
}

bool isin(int list[], int number, int size)
{
    if (size == 0)
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        if (list[i] == number)
        {
            return true;
        }
    }
    return false;
}

int generate_random(int min_value, int max_value, int *ignored_numbers, int size)
{
    int random_number;
    do
    {
        random_number = ((rand() % (max_value - min_value + 1)) + min_value);
        if (!isin(ignored_numbers, random_number, size))
        {
            return random_number;
        }
    } while (true);
}

bool isin_position(position positions[], position point, int size)
{
    if (size == 0)
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        if (positions[i].row == point.row && positions[i].col == point.col)
        {
            return true;
        }
    }
    return false;
}

bool isin_position(vector<position> positions, position point)
{
    if (positions.size() == 0)
    {
        return false;
    }
    for (int i = 0; i < positions.size(); i++)
    {
        if (positions[i].row == point.row && positions[i].col == point.col)
        {
            return true;
        }
    }
    return false;
}

position generate_random_position(position min_position, position max_position, position *points, int size)
{
    position point;
    do
    {
        point.row = ((rand() % (max_position.row - min_position.row + 1)) + min_position.row);
        point.col = ((rand() % (max_position.col - min_position.col + 1)) + min_position.col);
        if (!isin_position(points, point, size))
        {
            return point;
        }
    } while (true);
}

bool isSimplePath(int **table, int i, int j, bool **visited, int rows, int columns,
                  vector<position> &path, int path_length)
{
    if (isSafePosition(i, j, table, rows, columns) && path.size() <= path_length && !visited[i][j])
    {
        if (i == rows - 1 && j == columns - 1)
        {
            if (path.size() == path_length)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        visited[i][j] = true;
        position point;
        point.row = i;
        point.col = j;
        path.push_back(point);
        string movements[4] = {"up", "left", "down", "right"};
        shuffle_array(movements, 4);
        bool up, left, down, right;
        for (int x = 0; x < 4; x++)
        {
            if (movements[x] == "up")
            {
                up = isSimplePath(table, i - 1, j, visited, rows, columns, path, path_length);
                if (up)
                    return true;
            }
            else if (movements[x] == "left")
            {
                left = isSimplePath(table, i, j - 1, visited, rows, columns, path, path_length);
                if (left)
                    return true;
            }
            else if (movements[x] == "down")
            {
                down = isSimplePath(table, i + 1, j, visited, rows, columns, path, path_length);
                if (down)
                    return true;
            }
            else if (movements[x] == "right")
            {
                right = isSimplePath(table, i, j + 1, visited, rows, columns, path, path_length);
                if (right)
                    return true;
            }
        }
        visited[i][j] = false;
        path.pop_back();
    }
    return false;
}

/*Create a hard map*/
bool create_map(int **table, int rows, int columns, int path_length,
                int max_value, int min_value, int max_block, int min_block)
{
    int min_path_length = rows + columns - 2;
    if (path_length < min_path_length)
    {
        return false;
    }
    else
    {
        if (path_length - min_path_length % 2 == 1)
        {
            return false;
        }
    }
    const int count_cells = rows * columns;
    if (path_length + min_block >= count_cells)
    {
        return false;
    }
    else if (path_length + max_block >= count_cells)
    {
        max_block = count_cells - path_length - 1;
    }
    int path_random_numbers[path_length];
    int path_sum_numbers = 0;
    int ignored_numbers[1] = {0};
    int empty_array[0] = {};
    // generate random numbers of path
    while (path_sum_numbers == 0)
    {
        for (int i = 0; i < path_length; i++)
        {
            path_random_numbers[i] = generate_random(min_value, max_value, ignored_numbers, 1);
            path_sum_numbers += path_random_numbers[i];
        }
    }
    // generate random indexes of path
    vector<position> path_positions;
    bool **visited = new bool *[rows];
    for (int i = 0; i < rows; i++)
    {
        visited[i] = new bool[columns];
        for (int j = 0; j < columns; j++)
        {
            visited[i][j] = false;
        }
    }
    if (!isSimplePath(table, 0, 0, visited, rows, columns, path_positions, path_length))
    {
        return false;
    }
    for (int i = 0; i < path_positions.size(); i++)
    {
        table[path_positions[i].row][path_positions[i].col] = path_random_numbers[i];
    }
    position last_index;
    last_index.row = rows - 1;
    last_index.col = columns - 1;
    path_positions.push_back(last_index);
    table[rows - 1][columns - 1] = path_sum_numbers;
    // generate random block indexes
    int count_block = generate_random(min_block, max_block, empty_array, 0);
    position blocks[count_block + path_length + 1];
    position temp_point;
    for (int i = 0; i < path_length + 1; i++)
    {
        blocks[i] = path_positions[i];
    }
    position min_position;
    min_position.row = 0;
    min_position.col = 0;
    position max_position;
    max_position.row = rows - 1;
    max_position.col = columns - 1;
    for (int i = path_length + 1; i < count_block + path_length + 1; i++)
    {
        blocks[i] = generate_random_position(min_position, max_position, blocks, i);
        table[blocks[i].row][blocks[i].col] = 100000000;
    }
    // generate other numbers of table
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            temp_point.row = i;
            temp_point.col = j;
            if (!isin_position(path_positions, temp_point))
            {
                if (table[i][j] != 100000000)
                {
                    table[i][j] = generate_random(min_value, max_value, ignored_numbers, 1);
                }
                else
                {
                    table[i][j] = 0;
                }
            }
        }
    }
    return true;
}

/*Create an easy map*/
bool create_map(int **table, int rows, int columns)
{
    // initial values
    const int path_length = rows + columns - 2;
    const int max_value = 3;
    const int min_value = -3;
    int max_block = 5;
    int min_block = 2;
    const int count_cells = rows * columns;
    if (path_length + min_block >= count_cells)
    {
        return false;
    }
    else if (path_length + max_block >= count_cells)
    {
        max_block = count_cells - path_length - 1;
    }
    int path_random_numbers[path_length];
    int path_sum_numbers = 0;
    int ignored_numbers[1] = {0};
    // generate random numbers of path
    while (path_sum_numbers == 0)
    {
        for (int i = 0; i < path_length; i++)
        {
            path_random_numbers[i] = generate_random(min_value, max_value, ignored_numbers, 1);
            path_sum_numbers += path_random_numbers[i];
        }
    }
    // generate random movements of path
    int down_positions[rows - 1];
    for (int i = 0; i < rows - 1; i++)
    {
        down_positions[i] = generate_random(0, path_length - 1, down_positions, i);
    }
    // generate random indexes of path
    position path_positions[path_length + 1];
    int row_index = 0;
    int col_index = 0;
    table[0][0] = path_random_numbers[0];
    path_positions[0].row = 0;
    path_positions[0].col = 0;
    for (int i = 1; i < path_length; i++)
    {
        if (isin(down_positions, i - 1, rows - 1))
        {
            row_index++;
            table[row_index][col_index] = path_random_numbers[i];
        }
        else
        {
            col_index++;
            table[row_index][col_index] = path_random_numbers[i];
        }
        path_positions[i].row = row_index;
        path_positions[i].col = col_index;
    }
    path_positions[path_length].row = rows - 1;
    path_positions[path_length].col = columns - 1;
    table[rows - 1][columns - 1] = path_sum_numbers;
    // generate random block indexes
    int empty_array[0] = {};
    int count_block = generate_random(min_block, max_block, empty_array, 0);
    position blocks[count_block + path_length + 1];
    position temp_point;
    for (int i = 0; i < path_length + 1; i++)
    {
        blocks[i] = path_positions[i];
    }
    position min_position;
    min_position.row = 0;
    min_position.col = 0;
    position max_position;
    max_position.row = rows - 1;
    max_position.col = columns - 1;
    for (int i = path_length + 1; i < count_block + path_length + 1; i++)
    {
        blocks[i] = generate_random_position(min_position, max_position, blocks, i);
        table[blocks[i].row][blocks[i].col] = 1000000000;
    }
    // generate other numbers of table
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            temp_point.row = i;
            temp_point.col = j;
            if (!isin_position(path_positions, temp_point, path_length + 1))
            {
                if (table[i][j] != 1000000000)
                {
                    table[i][j] = generate_random(min_value, max_value, ignored_numbers, 1);
                }
                else
                {
                    table[i][j] = 0;
                }
            }
        }
    }
    return true;
}

#if defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
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
#endif

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