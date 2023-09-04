//Frank Zhuang
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>

using namespace std;

//goal state
const int goal = 40;

map<string, pair<double, string> > subTrees;

string dataToKey(int lives, int total, int scratch){//in this notation: "<lives> <total> <scratch>"
    return to_string(lives) + " " + to_string(total) + " " + to_string(scratch);
}

string dataToText(int lives, int total, int scratch, string decision, double prob){//
    return to_string(lives) + " " + to_string(total) + " " + to_string(scratch) + " " + decision + " " + to_string(prob);   
}

int findScratch(string line){
    stringstream ss(line);
    string num;
    vector<int> info;
    while (ss >> num){
        info.push_back(stoi(num));
    }
    return info.at(2);
}

bool findSol(string line){
    stringstream ss(line);
    string num;
    vector<int> info;
    while (ss >> num){
        info.push_back(stoi(num));
    }
    if(info.at(0)>1 && info.at(1)==20 && info.at(2)<20){
        return true;
    }
    return false;
}

double propogate(int lives, int total, int scratch){
    if(lives==0){//lose
        return 0;
    }
    if(total + scratch >= goal){//win
        return 1;
    }

    string key = dataToKey(lives, total, scratch);
    if(subTrees.find(key) == subTrees.end()){//if subtree isnt available...
        double op1 = 0.0;
        double op2 = 0.0;
        for(int i = 1; i<=5; i++){
            op1 += propogate(lives-1, total+scratch+i, 0);//hold given dice roll of i
            op2 += propogate(lives, total, scratch+i);//not hold given dice roll of i
        }
        double sixRoll = propogate(lives-1, total, 0);//rolling a six
        op1 += sixRoll;
        op2 += sixRoll;
        op1/=6.0;//prob of the roll
        op2/=6.0;
        if(op1>op2){//hold better
            subTrees.insert(make_pair(key, make_pair(op1,dataToText(lives, total, scratch, "Yes", op1))));
            return op1;
        }
        else{//not hold better
            subTrees.insert(make_pair(key, make_pair(op2,dataToText(lives, total, scratch, "No", op2))));
            return op2;
        }
    }
    else{//if subtree already exists return the odds
        return subTrees.find(key)->second.first;
    }
}

void save(){//saves to output.txt
    ofstream ofile;
    ofile.open("output.txt");
    
    map<string, pair<double, string> > ::iterator it = subTrees.begin();
    for(; it!=subTrees.end(); ++it){
        if(!(findScratch(it->first)==0)){
            ofile << it->second.second << endl;
        }
        // if(findSol(i.first)){
        //     ofile << i.second.second << endl;
        // }
    }
    ofile.close();

}

int main(){
    int lives = 5;
    propogate(lives, 0, 0);
    save();
}
