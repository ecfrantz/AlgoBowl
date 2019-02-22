 using namespace std;
 #include <string>
 #include <iostream>
 #include <fstream>
 #include <istream>
 #include <vector> 
 #include <utility>
 #include <math.h>
 #include <iomanip>
 
 
 int main(){
        vector<int> inputs;
        
        string fileName;
        cout << "Please enter a file name: ";
        cin >> fileName;
        ifstream inFile(fileName);
        if(inFile.fail()){ // Check for failures opening
           cerr << "Error opening input file";
           exit(1);
        }
        
        int num;
        while(inFile.eof() == false){
           inFile >> num;
           inputs.push_back(num);
        }
        
        
        vector<vector<int> additions> groups;
        vector<int> created;
        
        while(created != inputs){
         for(int i=0; i < inputs; i++){
          for(int j=i; j < inputs; j++){
           int sum = inputs[i] + inputs[j];
           groups[i].push_back(sum);
           groups[i].push_back(inputs[i]);
           groups[i].push_back(inputs[i]);
          }
         }
        }
 }