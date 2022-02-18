//
//  Item.cpp
//  VectorPacking
//
//  Created by Michael Kalab on 29.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//

#include "Item.hpp"

using namespace std;


Item::Item(){}

Item::Item(double scalarvalue, vector<double> values, int ind){
    _scalarvalue = scalarvalue;
    _values = values;
    _index = ind;
}

const double Item::getScalarValue(){
        return _scalarvalue;
}

const double Item::getValue(int i){
    return _values[i];
}

vector <double> Item::getValues(){
    return _values;
}

void Item::setScalarValue(double scalar){
    _scalarvalue = scalar;
}

void Item::addScalarValue(double scalar){
    _scalarvalue += scalar;
}

void Item::setIndex(int ind){
    _index = ind;
}

const int Item::getIndex(){
    return _index;
}

