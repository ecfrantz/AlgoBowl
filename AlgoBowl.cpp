using namespace std;
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <math.h>
#include <iomanip>

/// Structure that represents an addition action.
/// int:sum - summand1 + summand2 (result of action)
/// int:sumand1 - the first number in the sum
/// int:sumand2 - the second number in the sum
struct Action
{
   int sum;
   int sumand1;
   int sumand2;

   Action(int msum, int msumand1, int msumand2)
   {
      sum = msum;
      sumand1 = msumand1;
      sumand2 = msumand2;
   }
};

/// Loads an input file by a specified path
/// string:path - path to file input
/// vector<int> - the list of numbers loaded from the file
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
   inFile.close();

   return numbers;
}

/// Saves an output file by a specified path with actions
/// string:path - path to file output
/// vector<Action>:actions - properly ordered list of actions
void saveOutput(string path, vector<Action> actions)
{
   // Create output file stream
   ofstream outFile(path);

   // Write number of actions on first line
   outFile << actions.size() << endl;
   // Write each action on following lines
   for (auto action = actions.begin(); action != actions.end(); action++)
   {
      outFile << action->sumand1 << ' ' << action->sumand2 << endl;
   }
   outFile.close();
}

/// Performs the algorithm for finding the estimated optimal addition chain for a set of given inputs
/// vector<int>:numbers - the input set of numbers to the problem
/// vector<Action> - the ordered list of actions performed to solve the problem
vector<Action> findOptimalAdditionChain(vector<int> numbers)
{
   // Setup storage for algorithm
   vector<Action> performedActions;
   vector<Action> availableActions;
   vector<int> actionCosts;
   vector<int> basisNumbers;

   // Initialize basis numbers
   basisNumbers.push_back(1);

   // Continue performing actions until algorithm finds all the numbers
   while (numbers.size() > 0)
   {
      // Compute new available actions from most recently added basis number
      // Compute the costs of each of these new available actions and sort the actions based on cost

      // Perform the minimum cost action and update actions and numbers
   }

   // Return resulting action list
   return performedActions;
}

int main()
{
   // Get input file name from user.
   string inFileName;
   cout << "Please enter an input file name: ";
   cin >> inFileName;

   // Get output file name from user.
   string outFileName;
   cout << "Please enter an output file name: ";
   cin >> outFileName;

   // Load inputs from file.
   vector<int> inputs = loadInput(inFileName);

   // Perform algorithm.
   vector<Action> actions = findOptimalAdditionChain(inputs);

   // Save outputs to file.
   saveOutput(outFileName, actions);

   // vector<vector<int> additions> groups;
   // vector<int> created;

   // while (created != inputs)
   // {
   //    for (int i = 0; i < inputs; i++)
   //    {
   //       for (int j = i; j < inputs; j++)
   //       {
   //          int sum = inputs[i] + inputs[j];
   //          groups[i].push_back(sum);
   //          groups[i].push_back(inputs[i]);
   //          groups[i].push_back(inputs[i]);
   //       }
   //    }
   // }
}