#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using namespace std;

// !!!!! TO VERIFY FILES SIMPLY RUN THIS PROGRAM WITH INPUT AND OUTPUT FILES IN THE SAME DIRECTORY !!!!! //

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
/// set<int> - the list of numbers loaded from the file
set<int> loadInput(string path)
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
    set<int> numbers;
    int number;

    // Ignore first line because it only contains the number of numbers
    inFile >> number;
    // Read all numbers into storage
    while (inFile.eof() == false)
    {
        inFile >> number;
        numbers.insert(number);
    }
    inFile.close();

    return numbers;
}

/// Loads an output file by a specified path
/// string:path - path to file input
/// vector<Action> - the list of actions loaded from the file
vector<Action> loadOutput(string path)
{
    // Create input file stream
    ifstream inFile(path);

    // Check for failure during file loading
    if (inFile.fail())
    {
        cerr << "Error opening output file.";
        exit(1);
    }

    // Initialize actions
    vector<Action> actions;
    int numActions;

    // Take the first line as the number of actions there should be
    inFile >> numActions;
    // Read the actions into storage
    int sumand1;
    int sumand2;
    while (inFile.eof() == false)
    {
        inFile >> sumand1;
        inFile >> sumand2;

        actions.push_back(Action(sumand1 + sumand2, sumand1, sumand2));
    }

    // Verify that the number of actions matches the actions listed
    if (numActions != actions.size())
    {
        cerr << "Incorrect number of actions. FAILED VERIFY!";
        cin.ignore();
        exit(1);
    }

    return actions;
}

/// Verify whether the output is a valid response to the input
/// set<int>:targetNumbers target numbers from input
/// vector<Action>:actions actions contained in output
void verify(set<int> targetNumbers, vector<Action> actions)
{
    // Set up the basis numbers for the algorithm
    set<int> basisNumbers;
    basisNumbers.insert(1);

    // Verify that all basis numbers are accounted for in the actions
    int index = 0;
    for (auto action = actions.begin(); action != actions.end(); action++)
    {
        // Increment debugging index
        index++;
        // Check that both numbers involved in the action are valid
        if (basisNumbers.find(action->sumand1) == basisNumbers.end())
        {
            cerr << "Action #" << index << " is invalid. Tried using " << action->sumand1 << " which does not exist. FAILED VERIFY!";
            cin.ignore();
            exit(1);
        }
        if (basisNumbers.find(action->sumand2) == basisNumbers.end())
        {
            cerr << "Action #" << index << " is invalid. Tried using " << action->sumand2 << " which does not exist. FAILED VERIFY!";
            cin.ignore();
            exit(1);
        }
        // Add the new sum to the basis numbers
        basisNumbers.insert(action->sum);
    }

    // Verify that all target numbers have been achieved
    for (auto target = targetNumbers.begin(); target != targetNumbers.end(); target++)
    {
        if (basisNumbers.find(*target) == basisNumbers.end())
        {
            cerr << "Target number " << *target << " was missed. FAILED VERIFY!";
            cin.ignore();
            exit(1);
        }
    }
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

    // Load inputs and outputs from file.
    set<int> inputs = loadInput(inFileName);
    vector<Action> actions = loadOutput(outFileName);

    // Validate
    verify(inputs, actions);

    // Report success at verify
    cout << "No problems occurred. SUCCESS VERIFY!";
    cin.ignore();
}