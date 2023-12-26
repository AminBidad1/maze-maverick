#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void History()
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
        cout << "Date: " << line << endl;
        getline(infile, line);
        cout << "User: " << line << endl;
        getline(infile, line);
        cout << "Map name: " << line << endl;
        getline(infile, line);
        cout << "Time spent: " << line << endl;
        getline(infile, line);
        cout << "Result: " << line << endl
             << endl;
      }
    }
  }
  else
  {
    cout << "Unable to open the file for writing." << endl;
  }
}