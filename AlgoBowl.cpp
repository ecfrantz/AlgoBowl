using namespace std;
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <utility>
#include <math.h>
#include <iomanip>

/// Loads an input file by a specified path
/// string:path - path to file input
vector<int> loadInput(string path)
{
   // Create input file stream
   ifstream inFile(path);

   // Check for failure during file loading
   if (inFile.fail())
   {
      cerr << "Error opening input file.";
      exit(1);
   }

   // Initialize integers
   vector<int> numbers;
   int number;

   // Ignore first line because it only contains the number of numbers
   inFile >> number;
   // Read all numbers into storage
   while (inFile.eof() == false)
   {
      inFile >> number;
      numbers.push_back(number);
   }

   return numbers;
}

int main()
{
   // Get input file name from user.
   string inFileName;
   cout << "Please enter a file name: ";
   cin >> inFileName;

   // Load inputs from file.
   vector<int> inputs = loadInput(inFileName);

   vector<vector<int> additions> groups;
   vector<int> created;

   while (created != inputs)
   {
      for (int i = 0; i < inputs; i++)
      {
         for (int j = i; j < inputs; j++)
         {
            int sum = inputs[i] + inputs[j];
            groups[i].push_back(sum);
            groups[i].push_back(inputs[i]);
            groups[i].push_back(inputs[i]);
         }
      }
   }
}