//
// Created by Ethan on 5/1/2021.
//

#ifndef EQUATION_FINDER_EQUATION_H
#define EQUATION_FINDER_EQUATION_H
#include <iostream>
#include <vector>
#include "Units.cpp"

using namespace std;

class Equation {
public:
    Equation(double num, double unit);
    Equation(Equation *eq1, Equation *eq2, MathOperator opp);
    double get_value();
    int get_order();
    double get_unit();
    char get_operation_char();
    Equation* simplify();
    friend ostream & operator<<(ostream & out, Equation* e);


private:
    MathOperator _operation;
    double _value;
    Equation* _eq1;
    Equation* _eq2;
    int _order; //number of operators
    double _unit;
    static Equation* _oneConstant;
    vector<Equation*> _numerator;
    vector<Equation*> _denominator;
    void updateNumerator();
    void updateDenominator();


};



ostream & operator<<(ostream & out, Equation* e);

bool operator ==(Equation e1, Equation e2);
bool operator ==(Equation e, double d);
bool operator ==(double d, Equation e);

bool operator !=(Equation e1, Equation e2);
bool operator !=(Equation e, double d);
bool operator !=(double d, Equation e);

bool operator >(Equation e1, Equation e2);
bool operator >(Equation e, double d);
bool operator >(double d, Equation e);

bool operator <(Equation e1, Equation e2);
bool operator <(Equation e, double d);
bool operator <(double d, Equation e);



#endif //EQUATION_FINDER_EQUATION_H
