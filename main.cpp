#include <unistd.h>
#include <bits/stdc++.h>
#include <time.h>
#include "colors.hpp"
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;
namespace fs = filesystem;

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
const int ENTER_KEY = int('\n');
int getch(void);
#endif

struct GameTimer
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

struct Map
{
    string name = "";
    string path = "";
};

struct Board
{
    int **table;
    int rows, columns;
};

void reset_terminal();
int start_menu();
bool create_map(Board board);
bool create_map(Board board, int path_length,
                int max_value, int min_value, int max_block, int min_block);
int generate_random(int min_value, int max_value, int *ignored_numbers, int size);
bool isin(int list[], int number, int size);
bool isin_position(position positions[], position point, int size);
bool isin_position(vector<position> positions, position point);
position generate_random_position(position min_position, position max_position, position *points, int size);
void read_UserInfo(string &user_name);
void read_History();
void read_map(const string &MapPath, Board &board);
Map choose_existing_map();
void write_UserInfo(const string &user_name, const int &status, const int time_spent);
void write_History(const string &user_name, const string &map_name, const bool &result, const int &time_spent);
void write_map(const string &map_name, Board board);
bool isSafePosition(int i, int j, Board board);
bool isaPath(Board, int i, int j, bool **visited,
             vector<position> &path, int path_length, int sum);
vector<position> findPath(Board board, int path_length);
void show_table(Board board, vector<position> path, position user_index);
bool isSimplePath(Board board, int i, int j, bool **visited, vector<position> &path, int path_length);
bool play_game(Board board, position &current_position, vector<position> path);
bool can_move(Board board, int i, int j, vector<position> path);
vector<vector<string>> read_leaderboard();
bool sort_leaderboard(vector<string> x, vector<string> y);
void update_leaderboard(string username, bool status, int elapsed_time);
void show_leaderboard();
void test_create_map();
void test_hard_create_map();
void test_play_game(Map map);
void test_find_path(string map_path);

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
    Map map;
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
        map = choose_existing_map();
        test_play_game(map);
    }
    else if (selected_option == 6)
    {
        reset_terminal();
        cout << "Enter the path of map: ";
        cin >> map.path;
        test_play_game(map);
    }
    else if (selected_option == 8)
    {
        map = choose_existing_map();
        test_find_path(map.path);
    }
    else if (selected_option == 9)
    {
        reset_terminal();
        cout << "Enter the path of map: ";
        cin >> map.path;
        test_find_path(map.path);
    }
    else if (selected_option == 10)
    {
        read_History();
    }
    else if (selected_option == 11)
    {
        show_leaderboard();
    }
    return 0;
}

void test_find_path(string map_path)
{
    reset_terminal();
    int rows, columns;
    string username, map_name;
    Board board;
    read_map(map_path, board);
    rows = board.rows;
    columns = board.columns;
    vector<position> path = findPath(board, rows + columns - 2);
    position user_index;
    show_table(board, path, user_index);
    if (path.empty())
    {
        cout << "Path does not exist!" << endl;
    }
}

void test_play_game(Map map)
{
    reset_terminal();
    int rows, columns;
    string username, map_name;
    Board board;
    read_map(map.path, board);
    rows = board.rows;
    columns = board.columns;
    cout << "Enter your name: ";
    cin >> username;
    reset_terminal();
    vector<position> path;
    position user_index;
    user_index.row = 0;
    user_index.col = 0;
    path.push_back(user_index);
    GameTimer gameTimer;
    gameTimer.start();
    bool result = play_game(board, user_index, path);
    int elapsedTimeInSeconds = gameTimer.stop();
    cout << "Elapsed time: " << elapsedTimeInSeconds << "s\n";
    write_History(username, map.name, result, elapsedTimeInSeconds);
    write_UserInfo(username, result, elapsedTimeInSeconds);
    update_leaderboard(username, result, elapsedTimeInSeconds);
    delete[] board.table;
}

void test_create_map()
{
    reset_terminal();
    Board board;
    cout << "Enter count of rows: ";
    cin >> board.rows;
    cout << "Enter count of columns: ";
    cin >> board.columns;
    string map_name;
    cout << "Enter the map name: ";
    cin >> map_name;
    reset_terminal();
    board.table = new int *[board.rows];
    for (int i = 0; i < board.rows; i++)
    {
        board.table[i] = new int[board.columns];
    }
    if (!create_map(board))
    {
        cout << "Can't Create" << endl;
        return;
    }
    vector<position> path;
    position user_index;
    show_table(board, path, user_index);
    write_map(map_name, board);
    delete[] board.table;
}

void test_hard_create_map()
{
    reset_terminal();
    int path_length, max_value, min_value, max_block, min_block;
    Board board;
    cout << "Enter count of rows: ";
    cin >> board.rows;
    cout << "Enter count of columns: ";
    cin >> board.columns;
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
    string map_name;
    cout << "Enter the map name: ";
    cin >> map_name;
    reset_terminal();
    board.table = new int *[board.rows];
    for (int i = 0; i < board.rows; i++)
    {
        board.table[i] = new int[board.columns];
    }
    if (!create_map(board, path_length, max_value, min_value, max_block, min_block))
    {
        cout << "Invalid path! Cannot create the table." << endl;
        delete[] board.table;
        return;
    }
    write_map(map_name, board);
    vector<position> path;
    position user_index;
    show_table(board, path, user_index);
    delete[] board.table;
}

vector<vector<string>> read_leaderboard()
{
    string line;
    ifstream file("./Stats/leaderboard.txt");
    getline(file, line);
    // int count_user = std::stoi(line);
    int count_user;
    istringstream(line) >> count_user;
    vector<vector<string>> board;
    vector<string> temp;
    for (int i = 0; i < count_user; i++)
    {
        board.push_back(temp);
        for (int j = 0; j < 3; j++)
        {
            getline(file, line);
            board[i].push_back(line);
        }
    }
    return board;
}

bool sort_leaderboard(vector<string> x, vector<string> y)
{
    int i1, i2;
    if (x[1] != y[1])
    {
        return stoi(x[1]) > stoi(y[1]);
    }
    else
    {
        return stoi(x[2]) <= stoi(y[2]);
    }
}

void update_leaderboard(string username, bool status, int elapsed_time)
{
    vector<vector<string>> leaderboard = read_leaderboard();
    bool exist = false;
    for (int i = 0; i < leaderboard.size(); i++)
    {
        if (leaderboard[i][0] == username)
        {
            if (status)
            {
                leaderboard[i][1] = to_string(stoi(leaderboard[i][1]) + 1);
            }
            leaderboard[i][2] = to_string(stoi(leaderboard[i][2]) + elapsed_time);
            exist = true;
            break;
        }
    }
    if (!exist)
    {
        vector<string> user_info;
        user_info.push_back(username);
        if (status)
        {
            user_info.push_back("1");
        }
        else
        {
            user_info.push_back("0");
        }
        user_info.push_back(to_string(elapsed_time));
        leaderboard.push_back(user_info);
    }
    sort(leaderboard.begin(), leaderboard.end(), sort_leaderboard);
    ofstream leaderboard_file("./Stats/leaderboard.txt", ofstream::trunc);
    leaderboard_file << to_string(leaderboard.size()) << '\n';
    for (int i = 0; i < leaderboard.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            leaderboard_file << leaderboard[i][j] << '\n';
        }
    }
    leaderboard_file.close();
}

void show_leaderboard()
{
    reset_terminal();
    string line;
    ifstream file("./Stats/leaderboard.txt");
    getline(file, line);
    int count_user;
    istringstream(line) >> count_user;
    string board[count_user + 1][3];
    board[0][0] = "Username";
    board[0][1] = "Wins";
    board[0][2] = "Time Spent";
    for (int i = 1; i <= count_user; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            getline(file, line);
            board[i][j] = line;
        }
    }
    int max_length = 0;
    for (int i = 0; i <= count_user; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j].length() > max_length)
            {
                max_length = board[i][j].length();
            }
        }
    }
    const int cell_width = max_length + 4;
    int left_spaces, right_spaces, element_size, total_spaces;
    for (int j = 0; j < 3; j++)
    {
        cout << '+' << string(cell_width, '-');
    }
    cout << '+' << endl;
    for (int i = 0; i <= count_user; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            element_size = board[i][j].length();
            total_spaces = cell_width - element_size;
            right_spaces = total_spaces / 2;
            left_spaces = total_spaces - right_spaces;
            if (i == 0)
            {
                cout << '|';
                cout << string(left_spaces, ' ');
                cout << board[i][j];
                cout << string(right_spaces, ' ');
            }
            else if (i == 1)
            {
                cout << '|';
                cout << color::rize(string(left_spaces, ' '), "White", "Light Yellow");
                cout << color::rize(board[i][j], "White", "Light Yellow");
                cout << color::rize(string(right_spaces, ' '), "White", "Light Yellow");
            }
            else if (i == 2)
            {
                cout << '|';
                cout << color::rize(string(left_spaces, ' '), "White", "Dark Gray");
                cout << color::rize(board[i][j], "White", "Dark Gray");
                cout << color::rize(string(right_spaces, ' '), "White", "Dark Gray");
            }
            else if (i == 3)
            {
                cout << '|';
                cout << color::rize(string(left_spaces, ' '), "White", "Light Red");
                cout << color::rize(board[i][j], "White", "Light Red");
                cout << color::rize(string(right_spaces, ' '), "White", "Light Red");
            }
            else
            {
                return;
            }
        }
        cout << '|' << endl;
        for (int j = 0; j < 3; j++)
        {
            cout << '+' << string(cell_width, '-');
        }
        cout << '+' << endl;
    }
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
    reset_terminal();
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

Map choose_existing_map()
{
    Map map;
    vector<string> files;
    string FolderPath, MapPath;
    int NumberOfMaps = 0;
    FolderPath = "./Maps/";
    for (const auto &entry : fs::directory_iterator(FolderPath))
    {
        if (fs::is_regular_file(entry.path()))
        {
            files.push_back(entry.path().filename().string());
            NumberOfMaps++;
        }
    }
    string maps[NumberOfMaps];
    for (int i = 0; i < NumberOfMaps; i++)
    {
        maps[i] = to_string(i + 1) + ". " + files[i];
    }

    reset_terminal();
    if (files.empty())
    {
        cout << "No files found in the specified directory.\n";
        return map;
    }
    else
    {
        int userChoice = 1;
        while (true)
        {
            cout << "Existing maps:\n";
            for (size_t i = 0; i < files.size(); ++i)
            {
                if (userChoice == i + 1)
                {
                    cout << color::rize(maps[i], "Red", "Blue") << endl;
                }
                else
                {
                    cout << maps[i] << endl;
                }
            }
            string name;
            switch (getch())
            {
            case UP_KEY:
                userChoice--;
                break;
            case DOWN_KEY:
                userChoice++;
                break;
            case ENTER_KEY:
                MapPath = FolderPath + files[userChoice - 1];
                name = files[userChoice - 1];
                name.replace(name.length() - 4, 4, "");
                map.name = name;
                map.path = MapPath;
                return map;
            default:
                break;
            }
            if (userChoice > NumberOfMaps)
                userChoice = NumberOfMaps;
            else if (userChoice <= 1)
                userChoice = 1;
            reset_terminal();
        }
    }

    return map;
}

void read_map(const string &MapPath, Board &board)
{
    ifstream inputfile(MapPath);
    inputfile >> board.rows >> board.columns;
    board.table = new int *[board.rows];
    for (int i = 0; i < board.rows; i++)
    {
        board.table[i] = new int[board.columns];
    }
    for (int i = 0; i < board.rows; i++)
    {
        for (int j = 0; j < board.columns; j++)
        {
            inputfile >> board.table[i][j];
        }
    }
}

void write_UserInfo(const string &user_name, const int &status, const int time_spent)
{
    string filePath = "./Users/" + user_name + ".txt";
    ifstream outputFile(filePath);

    time_t now = time(0);
    tm *local_time = localtime(&now);
    char date_time[80];

    if (status == 1)
    {
        strftime(date_time, 80, "%Y-%m-%d %H:%M:%S", local_time);
    }

    if (!outputFile)
    {
        ofstream first_outputFile(filePath);
        first_outputFile << 1 << endl
                         << status << endl;
        if (status == 1)
            first_outputFile << date_time << endl;
        else
            first_outputFile << "NULL" << endl;
        first_outputFile << time_spent;
    }
    else
    {

        int totalGames, totalWins, totalTimeSpent;
        string lastWinTime, stro;
        outputFile >> totalGames >> totalWins;
        outputFile.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(outputFile, lastWinTime);
        outputFile >> totalTimeSpent;
        totalGames++;
        totalTimeSpent += time_spent;

        if (status == 1)
        {
            totalWins++;
        }

        strftime(date_time, 80, "%Y-%m-%d %H:%M:%S", local_time);
        // Move the file cursor to the beginning
        ofstream first_outputFile(filePath);
        first_outputFile.seekp(0);
        first_outputFile << totalGames << endl
                         << totalWins << endl;
        if (status == 1)
            first_outputFile << date_time << endl;
        else
            first_outputFile << lastWinTime << endl;
        first_outputFile << totalTimeSpent;
    }

    outputFile.close();
}

void write_History(const string &user_name, const string &map_name, const bool &result, const int &time_spent)
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
        outfile << "Time spent: " << time_spent << "s\n";
        if (result)
            outfile << "Result: "
                    << "Win"
                    << "\n";
        else
            outfile << "Result: "
                    << "Lost"
                    << "\n";

        outfile << existing_content;

        outfile.close();
    }
    else
    {
        cerr << "Unable to open the file for writing." << endl;
    }
}
void write_map(const string &map_name, Board board)
{
    const int rows = board.rows;
    const int columns = board.columns;
    string filePath = "./Maps/" + map_name + ".txt";
    ofstream outfile(filePath);
    if (outfile.is_open())
    {
        outfile << rows << "\n"
                << columns << "\n";
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                outfile << board.table[i][j] << " ";
            }
            outfile << "\n";
        }
        outfile.close();
    }
    else
        cerr << "Unable to open file for writing.\n";
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

bool can_move(Board board, int i, int j, vector<position> path)
{
    if (isSafePosition(i, j, board))
    {
        if (board.table[i][j] == 0)
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

bool play_game(Board board, position &current_position, vector<position> path)
{
    reset_terminal();
    const int rows = board.rows;
    const int columns = board.columns;
    show_table(board, path, current_position);
    int sum = board.table[0][0];
    while (true)
    {
        if (current_position.row == rows - 1 && current_position.col == columns - 1)
        {
            if (sum == board.table[rows - 1][columns - 1])
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
            can_move(board, current_position.row - 1, current_position.col, path) ||
            can_move(board, current_position.row + 1, current_position.col, path) ||
            can_move(board, current_position.row, current_position.col - 1, path) ||
            can_move(board, current_position.row, current_position.col + 1, path))
        {
            switch (getch())
            {
            case UP_KEY:
                if (can_move(board, current_position.row - 1, current_position.col, path))
                {
                    current_position.row -= 1;
                    path.push_back(current_position);
                    if (!(current_position.row == rows - 1 && current_position.col == columns - 1))
                        sum += board.table[current_position.row][current_position.col];
                    reset_terminal();
                    show_table(board, path, current_position);
                }
                break;
            case DOWN_KEY:
                if (can_move(board, current_position.row + 1, current_position.col, path))
                {
                    current_position.row += 1;
                    path.push_back(current_position);
                    if (!(current_position.row == rows - 1 && current_position.col == columns - 1))
                        sum += board.table[current_position.row][current_position.col];
                    reset_terminal();
                    show_table(board, path, current_position);
                }
                break;
            case LEFT_KEY:
                if (can_move(board, current_position.row, current_position.col - 1, path))
                {
                    current_position.col -= 1;
                    path.push_back(current_position);
                    if (!(current_position.row == rows - 1 && current_position.col == columns - 1))
                        sum += board.table[current_position.row][current_position.col];
                    reset_terminal();
                    show_table(board, path, current_position);
                }
                break;
            case RIGHT_KEY:
                if (can_move(board, current_position.row, current_position.col + 1, path))
                {
                    current_position.col += 1;
                    path.push_back(current_position);
                    if (!(current_position.row == rows - 1 && current_position.col == columns - 1))
                        sum += board.table[current_position.row][current_position.col];
                    reset_terminal();
                    show_table(board, path, current_position);
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

void show_table(Board board, vector<position> path,
                position user_index)
{
    int rows = board.rows;
    int columns = board.columns;
    int max_length = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (to_string(board.table[i][j]).length() > max_length)
            {
                max_length = to_string(board.table[i][j]).length();
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
            number_size = to_string(board.table[i][j]).length();
            total_spaces = cell_width - number_size;
            right_spaces = total_spaces / 2;
            left_spaces = total_spaces - right_spaces;
            point.row = i;
            point.col = j;
            cout << '|';
            if (i == 0 && j == 0)
            {
                cout << color::rize(string(left_spaces, ' '), "Blue", "Red");
                cout << color::rize(to_string(board.table[i][j]), "Blue", "Red");
                cout << color::rize(string(right_spaces, ' '), "Blue", "Red");
            }
            else if (i == rows - 1 && j == columns - 1)
            {
                cout << color::rize(string(left_spaces, ' '), "Red", "Blue");
                cout << color::rize(to_string(board.table[i][j]), "Red", "Blue");
                cout << color::rize(string(right_spaces, ' '), "Red", "Blue");
            }
            else if (user_index.row == i && user_index.col == j)
            {
                cout << color::rize(string(left_spaces, ' '), "White", "Green");
                cout << color::rize(to_string(board.table[i][j]), "White", "Green");
                cout << color::rize(string(right_spaces, ' '), "White", "Green");
            }
            else if (board.table[i][j] == 0)
            {
                cout << color::rize(string(left_spaces, ' '), "Red", "Light Yellow");
                cout << color::rize(to_string(board.table[i][j]), "Red", "Light Yellow");
                cout << color::rize(string(right_spaces, ' '), "Red", "Light Yellow");
            }
            else if (isin_position(path, point))
            {
                cout << color::rize(string(left_spaces, ' '), "Red", "Light Blue");
                cout << color::rize(to_string(board.table[i][j]), "Red", "Light Blue");
                cout << color::rize(string(right_spaces, ' '), "Red", "Light Blue");
            }
            else
            {
                cout << string(left_spaces, ' ');
                cout << to_string(board.table[i][j]);
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

bool isSafePosition(int i, int j, Board board)
{
    if (i >= 0 && i < board.rows && j >= 0 && j < board.columns)
        return true;
    return false;
}

bool isaPath(Board board, int i, int j, bool **visited,
             vector<position> &path, int path_length, int sum)
{
    const int rows = board.rows;
    const int columns = board.columns;
    if ((isSafePosition(i, j, board) && board.table[i][j] != 0) && (!visited[i][j] && path.size() <= path_length))
    {
        visited[i][j] = true;
        if (i == rows - 1 && j == columns - 1)
        {
            if (path.size() == path_length && sum == board.table[i][j])
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
        bool up = isaPath(board, i - 1, j, visited, path, path_length, sum + board.table[i][j]);
        if (up)
        {
            return true;
        }
        bool left = isaPath(board, i, j - 1, visited, path, path_length, sum + board.table[i][j]);
        if (left)
        {
            return true;
        }
        bool down = isaPath(board, i + 1, j, visited, path, path_length, sum + board.table[i][j]);
        if (down)
        {
            return true;
        }
        bool right = isaPath(board, i, j + 1, visited, path, path_length, sum + board.table[i][j]);
        if (right)
        {
            return true;
        }
        visited[i][j] = false;
        path.pop_back();
    }
    return false;
}

vector<position> findPath(Board board, int path_length)
{
    bool **visited = new bool *[board.rows];
    for (int i = 0; i < board.rows; i++)
    {
        visited[i] = new bool[board.columns];
        for (int j = 0; j < board.columns; j++)
        {
            visited[i][j] = false;
        }
    }
    vector<position> path;
    vector<position> empty;
    if (isaPath(board, 0, 0, visited, path, path_length, 0))
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

bool isSimplePath(Board board, int i, int j, bool **visited,
                  vector<position> &path, int path_length)
{
    const int rows = board.rows;
    const int columns = board.columns;
    if (isSafePosition(i, j, board) && path.size() <= path_length && !visited[i][j])
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
                up = isSimplePath(board, i - 1, j, visited, path, path_length);
                if (up)
                    return true;
            }
            else if (movements[x] == "left")
            {
                left = isSimplePath(board, i, j - 1, visited, path, path_length);
                if (left)
                    return true;
            }
            else if (movements[x] == "down")
            {
                down = isSimplePath(board, i + 1, j, visited, path, path_length);
                if (down)
                    return true;
            }
            else if (movements[x] == "right")
            {
                right = isSimplePath(board, i, j + 1, visited, path, path_length);
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
bool create_map(Board board, int path_length,
                int max_value, int min_value, int max_block, int min_block)
{
    const int rows = board.rows;
    const int columns = board.columns;
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
    if (!isSimplePath(board, 0, 0, visited, path_positions, path_length))
    {
        return false;
    }
    for (int i = 0; i < path_positions.size(); i++)
    {
        board.table[path_positions[i].row][path_positions[i].col] = path_random_numbers[i];
    }
    position last_index;
    last_index.row = rows - 1;
    last_index.col = columns - 1;
    path_positions.push_back(last_index);
    board.table[rows - 1][columns - 1] = path_sum_numbers;
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
        board.table[blocks[i].row][blocks[i].col] = 100000000;
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
                if (board.table[i][j] != 100000000)
                {
                    board.table[i][j] = generate_random(min_value, max_value, ignored_numbers, 1);
                }
                else
                {
                    board.table[i][j] = 0;
                }
            }
        }
    }
    return true;
}

/*Create an easy map*/
bool create_map(Board board)
{
    // initial values
    const int path_length = board.rows + board.columns - 2;
    const int max_value = 3;
    const int min_value = -3;
    int max_block = 5;
    int min_block = 2;
    const int count_cells = board.rows * board.columns;
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
    int down_positions[board.rows - 1];
    for (int i = 0; i < board.rows - 1; i++)
    {
        down_positions[i] = generate_random(0, path_length - 1, down_positions, i);
    }
    // generate random indexes of path
    position path_positions[path_length + 1];
    int row_index = 0;
    int col_index = 0;
    board.table[0][0] = path_random_numbers[0];
    path_positions[0].row = 0;
    path_positions[0].col = 0;
    for (int i = 1; i < path_length; i++)
    {
        if (isin(down_positions, i - 1, board.rows - 1))
        {
            row_index++;
            board.table[row_index][col_index] = path_random_numbers[i];
        }
        else
        {
            col_index++;
            board.table[row_index][col_index] = path_random_numbers[i];
        }
        path_positions[i].row = row_index;
        path_positions[i].col = col_index;
    }
    path_positions[path_length].row = board.rows - 1;
    path_positions[path_length].col = board.columns - 1;
    board.table[board.rows - 1][board.columns - 1] = path_sum_numbers;
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
    max_position.row = board.rows - 1;
    max_position.col = board.columns - 1;
    for (int i = path_length + 1; i < count_block + path_length + 1; i++)
    {
        blocks[i] = generate_random_position(min_position, max_position, blocks, i);
        board.table[blocks[i].row][blocks[i].col] = 1000000000;
    }
    // generate other numbers of table
    for (int i = 0; i < board.rows; i++)
    {
        for (int j = 0; j < board.columns; j++)
        {
            temp_point.row = i;
            temp_point.col = j;
            if (!isin_position(path_positions, temp_point, path_length + 1))
            {
                if (board.table[i][j] != 1000000000)
                {
                    board.table[i][j] = generate_random(min_value, max_value, ignored_numbers, 1);
                }
                else
                {
                    board.table[i][j] = 0;
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