#include <iostream>
#include <map>
#include <vector>
#include "Equation.h"
#include <cmath>

using namespace std;

bool close_enough(double answer, double guess, double precision = 0.06){
    return abs(answer-guess) < precision;
}



int main() {
    double answer = 8.0;
    double answerUnits = 2.0;
    vector<double> givenValues = {5.0, 2.0, 6.0};
    vector<double> givenUnits =  {2.0, 7.0, 14.0};
    int order = 0;
    //initialize map
    map<double, vector<Equation*>> allEquations;

    for(int x = 0; x<givenValues.size(); x++){
        Equation* newValPtr = new Equation(givenValues.at(x), givenUnits.at(x));
        allEquations[newValPtr->get_value()].push_back(newValPtr);
    }

    vector<Equation*> solutions;
    vector<Equation*> newEquations;
    bool found = false;
    Equation* result;

    while(!found){
        for(auto it1 = allEquations.begin(); it1 != allEquations.end(); it1++){
            for(auto it2 = it1; it2 != allEquations.end(); it2++){
                if (it1->second.at(0)->get_unit() == it2->second.at(0)->get_unit()){
                    Equation* newAddPtr = new Equation(it1->second.at(0), it2->second.at(0), '+');
                    Equation* newSubPtr = new Equation(it1->second.at(0), it2->second.at(0), '-');
                    newEquations.push_back(newAddPtr);
                    newEquations.push_back(newSubPtr);
                }
                Equation* newMulPtr = new Equation(it1->second.at(0), it2->second.at(0), '*');
                Equation* newDivPtr = new Equation(it1->second.at(0), it2->second.at(0), '/');
                newEquations.push_back(newMulPtr);
                newEquations.push_back(newDivPtr);
            }
        }

        for(Equation* x:newEquations) {
            if(close_enough(answer, x->get_value()) && x->get_unit() == answerUnits){
                found = true;
                result = x;
                solutions.push_back(x);
            }
            allEquations[x->get_value()].push_back(x);
        }
        newEquations.clear();
        cout << allEquations.size() << endl;

    }

    //cout << result << " = " << result->get_value() << "  Units: " << result->get_unit() << endl;
    for(Equation* x:solutions){
        x = x->simplify();
        cout << x << " = " << x->get_value() << endl;
    }

    return 0;
}