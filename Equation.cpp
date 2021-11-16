//
// Created by Ethan on 5/1/2021.
//

#include "Equation.h"
#include <cmath>

Equation* Equation::_oneConstant = new Equation(1, 1);

Equation::Equation(double num, double unit) {
    _order = 0;
    _value = num;
    _eq1 = nullptr;
    _eq2 = nullptr;
    _operation = '\0';
    _unit = unit;
}


Equation::Equation(Equation *eq1, Equation *eq2, char opp) {
    _order = eq1->get_order() + eq2->get_order() + 1;
    _eq1 = eq1;
    _eq2 = eq2;
    _operation = opp;

    double num1 = eq1->get_value();
    double num2;
    double unit1 = eq1->get_unit();
    double unit2;


    if(eq2 != nullptr){
        num2 = eq2->get_value();
        unit2 = eq2->get_unit();
    }


    switch (opp) {
        case '+':
            _value = num1+num2;
            if (unit1 != unit2){
                throw invalid_argument("different units");
            }
            _unit = unit1;
            break;
        case '-':
            _value = num1-num1; //maybe abs?
            if (unit1 != unit2){
                throw invalid_argument("different units");
            }
            _unit = unit1;
            break;
        case '*':
            _value = num1*num2;
            _unit = unit1 * unit2;
            break;
        case '/':
            _value = num1/num2;
            _unit = unit1/unit2;
            break;
        case 's':
            _value = sqrt(num1); // num2 should be NULL
            _unit = unit1; //Units here?? how?
            break;
        default:
            throw logic_error("Operation does not exist");

    }
}

double Equation::get_value() {
    return _value;
}

int Equation::get_order() {
    return _order;
}

double Equation::get_unit() {
    return _unit;
}

Equation* Equation::simplify() {
    if (_order == 0) return this;




    _eq1 = _eq1->simplify();
    _eq2 = _eq2->simplify();

    updateNumerator();
    updateDenominator();
    ///TODO: fix this
    for(Equation* n : _numerator){

        for(Equation* d : _denominator){
            cout << d << "\t";
            if (n == d){
                *n = *_oneConstant;
                *d = *_oneConstant;
            }
        }
    }
    cout << endl;
    //cout << _eq1 << " : " << _eq2 << endl;

    if (_eq1 == nullptr) return _eq2;
    if (_eq2 == nullptr) return _eq1;


    if (_operation == '/') {
        if (_eq1 == _eq2) return _oneConstant;
        if (_eq2 == _oneConstant) return _eq1;
    }

    if(_operation == '*'){
        if (_eq1 == _oneConstant) return _eq2;
        if (_eq2 == _oneConstant) return _eq1;
        if (_eq1->_operation == '/'){ // Fixes the scenario: 1/2 * (2+5) -> (2+5)/2
            if (_eq1->_eq1 == _oneConstant){
                _operation = '/';
                Equation* temp = _eq2;
                _eq2 = _eq1->_eq2;
                _eq1 = temp;

            }
            if (_eq2->_eq1 == _oneConstant){
                _operation = '/';
                _eq2 = _eq2->_eq2;
            }
        }
    }

    return this;
}


ostream & operator<<(ostream & out, Equation* e){

    if (e->_order == 0) {
        out << e->_value;
    }else{
        out << "(" << e->_eq1 << e->_operation << e->_eq2 << ")";
    }
    return out;
}

vector<Equation *> Equation::getNumerator() {
    vector<Equation*> numerator = {};
    if (_operation == '/'){
        for (Equation* x : _eq1->getNumerator()){
            numerator.push_back(x);
        }
        for (Equation* x : _eq2->getDenominator()) {
            numerator.push_back(x);
        }
    }
    else if (_operation == '*') {
        for (Equation *x : _eq1->getNumerator()) {
            numerator.push_back(x);
        }
        for (Equation *x : _eq2->getNumerator()) {
            numerator.push_back(x);
        }
    }
    else {
        numerator.push_back(this);
    }

    return numerator;
}

vector<Equation *> Equation::getDenominator() {
    vector<Equation*> denominator = {};
    if (_operation == '/'){

        for (Equation* x : _eq1->getDenominator()){
            denominator.push_back(x);
        }
        for(Equation* x : _eq2->getNumerator()){
            denominator.push_back(x);
        }
    }
    else if (_operation == '*'){
        for (Equation* x : _eq1->getDenominator()) {
            denominator.push_back(x);
        }
        for (Equation* x : _eq2->getDenominator()){
            denominator.push_back(x);
        }
    }

    return denominator;
}

void Equation::updateNumerator() {
    if (_operation == '/'){
        for (Equation* x : _eq1->_numerator){
            _numerator.push_back(x);
        }
        for (Equation* x : _eq2->_denominator) {
            _numerator.push_back(x);
        }
    }
    else if (_operation == '*') {
        for (Equation *x : _eq1->_numerator) {
            _numerator.push_back(x);
        }
        for (Equation *x : _eq2->_numerator) {
            _numerator.push_back(x);
        }
    }
    else {
        _numerator.push_back(this);
    }
}

void Equation::updateDenominator() {
    vector<Equation*> denominator = {};
    if (_operation == '/'){
        for (Equation* x : _eq1->_denominator){
            _denominator.push_back(x);
        }
        for(Equation* x : _eq2->_numerator){
            _denominator.push_back(x);
        }
    }
    else if (_operation == '*'){
        for (Equation* x : _eq1->_denominator){
            _denominator.push_back(x);
        }
        for (Equation* x : _eq2->_denominator){
            _denominator.push_back(x);
        }
    }
}


bool operator ==(Equation e1, Equation e2){
    return e1.get_value() == e2.get_value();
}
bool operator ==(Equation e, double d){
    return e.get_value() == d;
}
bool operator ==(double d, Equation e){
    return e.get_value() == d;
}

bool operator !=(Equation e1, Equation e2){
    return !(e1==e2);
}
bool operator !=(Equation e, double d){
    return !(e==d);
}
bool operator !=(double d, Equation e){
    return !(e==d);
}

bool operator >(Equation e1, Equation e2){
    return e1.get_value() > e2.get_value();
}
bool operator >(Equation e, double d){
    return e.get_value() > d;
}
bool operator >(double d, Equation e) {
    return d > e.get_value();
}


bool operator <(Equation e1, Equation e2){
    return e1.get_value() < e2.get_value();
}
bool operator <(Equation e, double d){
    return e.get_value() < d;
}
bool operator <(double d, Equation e){
    return d < e.get_value();
}