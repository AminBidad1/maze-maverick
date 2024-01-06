#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void user_info(string user_name)
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
    cout << "Unable to open the file for writing." << endl;
  }
}
