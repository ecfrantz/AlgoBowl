using namespace std;
#include <vector> 
#include <utility>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <istream>

int main(){
    vector<int> inputs;
    int num = 1000;
    srand (time(NULL));
    int n = rand() % 10; 
    inputs.push_back(n);
    for(int i=0; i<num; i++){
        int m = rand() % n + 1000000; 
        inputs.push_back(m);
        n = m;
    }
    ofstream outfile("inputs.txt");
    for(int i=0; i<num; i++){
        outfile << inputs[i] << endl;
    }
}