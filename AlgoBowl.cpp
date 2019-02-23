#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <queue>
#include <vector>

using namespace std;

/// Structure that represents an addition action.
/// int:sum - summand1 + summand2 (result of action)
/// int:sumand1 - the first number in the sum
/// int:sumand2 - the second number in the sum
/// int:cost - the cost of performing the action
struct Action
{
   int sum;
   int sumand1;
   int sumand2;
   int cost;

   Action(int msum, int msumand1, int msumand2)
   {
      sum = msum;
      sumand1 = msumand1;
      sumand2 = msumand2;
      cost = 0;
   }
};

/// Less than / greater than operators for sorting actions based on cost
bool operator<(const Action &x, const Action &y)
{
   if (x.cost == y.cost)
      return x.sum < y.sum;
   return x.cost < y.cost;
}
bool operator>(const Action &x, const Action &y)
{
   if (x.cost == y.cost)
      return x.sum > y.sum;
   return x.cost > y.cost;
}

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

/// Saves an output file by a specified path with actions
/// string:path - path to file output
/// vector<Action>:actions - properly ordered list of actions
void saveOutput(string path, vector<Action> *actions)
{
   // Create output file stream
   ofstream outFile(path);

   // Write number of actions on first line
   outFile << actions->size() << endl;
   // Write each action on following lines
   for (auto action = actions->begin(); action != actions->end(); action++)
   {
      outFile << action->sumand1 << ' ' << action->sumand2 << endl;
   }
   outFile.close();
}

/// Perform the logic for performing a specified action on the data
/// Action:action - the action to be performed
/// vector<Action>:performedActions - the list of previously performed actions
/// set<Action>:availableActions - the set of actions that can be performed
/// set<int>:basisNumbers - the set of numbers that have been already discovered
/// set<int>:targetNumbers - the set of numbers that are targets to discover
void performAction(Action action, vector<Action> *performedActions, multiset<Action> *availableActions, set<int> *basisNumbers, set<int> *targetNumbers)
{
   // Add action to performed actions
   performedActions->push_back(action);

   // Remove action from available actions
   auto i = availableActions->begin();
   while (i != availableActions->end())
   {
      if (i->sum == action.sum)
         break;
      i++;
   }
   if (i != availableActions->end())
   {
      availableActions->erase(i);
   }

   // Add sum to basis numbers and remove from numbers if possible
   basisNumbers->insert(action.sum);
   targetNumbers->erase(action.sum);
}

/// Compute new available actions given a newly added basis number
/// int:newBasis - newly added basis number
/// set<int>:basisNumbers - all basis numbers
/// set<Action>:availableActions - set of currently available actions
void computeNewActions(int newBasis, set<int> *basisNumbers, multiset<Action> *availableActions)
{
   // Only create new action sums from combinations with the new basis number.
   for (auto basisNumber = basisNumbers->begin(); basisNumber != basisNumbers->end(); basisNumber++)
   {
      // Create new action from combination of basis numbers.
      Action newAction(newBasis + *basisNumber, newBasis, *basisNumber);

      // Check if the result of the action already exists in the available actions.
      bool actionExists = false;
      for (auto checkAction = availableActions->begin(); checkAction != availableActions->end(); checkAction++)
      {
         if (newAction.sum == checkAction->sum)
         {
            actionExists = true;
            break;
         }
      }

      // Only add new action if the result of the action does not already exist.
      if (!actionExists)
      {
         availableActions->insert(newAction);
      }
   }
}

/// Compute the heuristic optimal number of actions to achieve a target
/// int:target - the target number to discover
/// set<int>:modifiableBasisNumbers - the set of numbers to use as a basis that can be modified
int computeOptimalActionCount(int target, set<int> *modifiableBasisNumbers)
{
   // Get the maximum value in our basis numbers to determine if we need to do doubling
   int max_value = *(--modifiableBasisNumbers->end());
   int actions = 0;

   // Double the maximum value while too low to reach the target
   while (target >= 2 * max_value)
   {
      // Double the maximum value and add it to the basis numbers
      max_value += max_value;
      modifiableBasisNumbers->insert(max_value);
      actions++;
   }

   // Shortcut return cost if target equals maximum value
   if (target == max_value)
   {
      return actions;
   }

   // Initialize residual pathfinding storage
   queue<vector<int>> residuals;
   vector<int> initial_residual;
   initial_residual.push_back(target - max_value);
   residuals.push(initial_residual);

   // Continue looping over residuals until a residual of zero is found
   while (true)
   {
      // Pop the next residual from the residuals
      vector<int> current_residual = residuals.front();
      residuals.pop();

      // Try combining current residual with all current bases
      for (auto base = modifiableBasisNumbers->rbegin(); base != modifiableBasisNumbers->rend(); base++)
      {
         // An exact match for the residual was found
         if (*current_residual.begin() == *base)
         {
            // Insert the target and numbers used to get to target to basis numbers
            modifiableBasisNumbers->insert(target);
            for (auto added_base = current_residual.rbegin(); added_base != current_residual.rend() - 1; added_base++)
            {
               modifiableBasisNumbers->insert(*added_base);
            }

            // Return combined double time and residual time
            return actions + current_residual.size();
         }
         // Construct new smaller residual
         else if (*current_residual.begin() > *base)
         {
            // Construct new residual and add it to the residual collection
            vector<int> new_residual(current_residual);
            *new_residual.begin() = *current_residual.begin() - *base;
            new_residual.push_back(target - *current_residual.begin() + *base);
            residuals.push(new_residual);
         }
      }

      // Try combining current residual with all previous residual bases
      for (auto base = current_residual.rbegin(); base != current_residual.rend() - 1; base++)
      {
         // An exact match for the residual was found
         if (*current_residual.begin() == *base)
         {
            // Insert the target and numbers used to get to target to basis numbers
            modifiableBasisNumbers->insert(target);
            for (auto added_base = current_residual.rbegin(); added_base != current_residual.rend() - 1; added_base++)
            {
               modifiableBasisNumbers->insert(*added_base);
            }

            // Return combined double time and residual time
            return actions + current_residual.size();
         }
         // Construct new smaller residual
         else if (*current_residual.begin() > *base)
         {
            // Construct new residual and add it to the residual collection
            vector<int> new_residual(current_residual);
            *new_residual.begin() = *current_residual.begin() - *base;
            new_residual.push_back(target - *current_residual.begin() + *base);
            residuals.push(new_residual);
         }
      }
   }
}

/// Compute the cost of performing a particular action
/// Action:action - action to compute the cost of
/// set<int>:basisNumbers - set of numbers to use to construct targets
/// set<int>:targetNumbers - set of target numbers to discover
int computeActionCost(Action action, set<int> *basisNumbers, set<int> *targetNumbers)
{
   // Perform shortcut cost if action results in one of the target numbers
   if (targetNumbers->find(action.sum) != targetNumbers->end())
   {
      return 0;
   }

   // Initialize accumulated cost across targets.
   int sumCost = 0;

   // Copy basis numbers so they can be modified separately in computations
   set<int> c_basisNumbers(*basisNumbers);

   // Sum up each of the costs for target numbers.
   int offset = 0;
   for (auto target = targetNumbers->begin(); target != targetNumbers->end(); target++)
   {
      sumCost += computeOptimalActionCount(*target, &c_basisNumbers);
   }

   return sumCost;
}

/// Compute the costs of each action available
/// set<Action>:availableActions - all available actions
/// set<int>:basisNumbers - set of sorted basis numbers
/// set<int>:targetNumbers - set of sorted target numbers
void computeActionCosts(multiset<Action> *availableActions, set<int> *basisNumbers, set<int> *targetNumbers)
{
   // Copy the available actions set into a temporary copy set
   multiset<Action> c_availableActions(*availableActions);

   // Clear the original set of available actions
   availableActions->clear();

   // Recalculate the costs of actions and add each action to the available actions set now that it has been cleared
   for (auto action = c_availableActions.begin(); action != c_availableActions.end(); action++)
   {
      // Calculate the cost and add action
      Action recalculatedAction(*action);
      recalculatedAction.cost = computeActionCost(*action, basisNumbers, targetNumbers);
      availableActions->insert(recalculatedAction);
   }
}

/// Performs the algorithm for finding the estimated optimal addition chain for a set of given inputs
/// vector<int>:targetNumbers - the input set of numbers to the problem
/// vector<Action> - the ordered list of actions performed to solve the problem
vector<Action> findOptimalAdditionChain(set<int> targetNumbers)
{
   // Setup storage for algorithm
   vector<Action> performedActions;
   multiset<Action> availableActions;
   set<int> basisNumbers;
   int newBasisNumber = 1;

   // Initialize basis numbers
   basisNumbers.insert(1);

   // Continue performing actions until algorithm finds all the numbers
   while (targetNumbers.size() > 0)
   {
      // Compute new available actions from most recently added basis number
      // Compute the costs of each of all available actions and sort the actions based on cost
      computeNewActions(newBasisNumber, &basisNumbers, &availableActions);
      computeActionCosts(&availableActions, &basisNumbers, &targetNumbers);

      // Perform the minimum cost action and update actions and numbers
      performAction(*availableActions.begin(), &performedActions, &availableActions, &basisNumbers, &targetNumbers);
      newBasisNumber = performedActions.rbegin()->sum;
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
   set<int> inputs = loadInput(inFileName);

   // Perform algorithm.
   vector<Action> actions = findOptimalAdditionChain(inputs);

   // Save outputs to file.
   saveOutput(outFileName, &actions);
}