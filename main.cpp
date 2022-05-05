#include <iostream>
#include <map>
#include <vector>
#include "Equation.h"
#include "Units.h"
#include <cmath>

using namespace std;

bool close_enough(double answer, double guess, double precision = 0.005){
    return abs(answer-guess) < precision;
}



int main() {
    double answer = 11.174;
    double answerUnits = m/s/s;
    vector<double> givenValues = {18.5, 46.1, 2.47};
    vector<double> givenUnits =  {m/s, m/s, s};

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
                    newEquations.push_back(newAddPtr);
                    //cout << newAddPtr << newAddPtr->get_value() << endl;
                    if (it1 != it2) { //prevents creating equations equal to zero
                        Equation *newSubPtr1 = new Equation(it1->second.at(0), it2->second.at(0), '-');
                        Equation *newSubPtr2 = new Equation(it2->second.at(0), it1->second.at(0), '-');
                        newEquations.push_back(newSubPtr1);
                        newEquations.push_back(newSubPtr2);
                        //cout << newSubPtr1 << newSubPtr1->get_value() << endl;
                        //cout << newSubPtr2 << newSubPtr2->get_value() << endl;
                    }


                }
                Equation* newMulPtr = new Equation(it1->second.at(0), it2->second.at(0), '*');
                newEquations.push_back(newMulPtr);
                //cout << newMulPtr << newMulPtr->get_value() << endl;
                if (it1 != it2) { //prevents creating equations equal to one
                    Equation *newDivPtr1 = new Equation(it1->second.at(0), it2->second.at(0), '/');
                    Equation *newDivPtr2 = new Equation(it2->second.at(0), it1->second.at(0), '/');
                    newEquations.push_back(newDivPtr1);
                    newEquations.push_back(newDivPtr2);
                    //cout << newDivPtr1 << newDivPtr1->get_value() << endl;
                    //cout << newDivPtr2 << newDivPtr2->get_value() << endl;
                }




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