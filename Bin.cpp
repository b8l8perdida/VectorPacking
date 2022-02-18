//
//  Bin.cpp
//  VectorPacking
//
//  Created by Michael Kalab on 30.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//

#include "Bin.hpp"


/*vector<Item> Bin::getItems(){
    return _items;
}*/

Bin::Bin(Item item){
    //vector<Item> dummyitem; //not really necessary, just slows thing down
    //dummyitem.push_back(item);
    /*_items = dummyitem;*/
    _values = item.getValues();
}

Bin::Bin(){}

Bin::Bin(vector<double> values){
    unsigned int weightsize = (int)values.size();
    for (int i = 0; i < weightsize; i++){
        _values.push_back(values[i]);
    }
}

const double Bin::getValue(int i){
    return _values[i];
}


void Bin::setValue(int i, double add){ //actualize value i of the bin
    _values[i] += add;
}

void Bin::setValues(vector<double> values){ //actualize all values of the bin
    for (int i = 0; i < values.size(); i++){
        setValue(i, values[i]);
    }
}

