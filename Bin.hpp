//
//  Bin.hpp
//  VectorPacking
//
//  Created by Michael Kalab on 30.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//

#ifndef Bin_hpp
#define Bin_hpp

#include <stdio.h>
#include <iostream>
#include "Item.hpp"

class Bin{
public:
    //vector<Item> _items;
    vector<double> _values;
    void setValue(int i, double add);
    const double getValue(int i);
    //vector<Item> getItems();
    Bin();
    Bin(Item item);
    Bin(vector<double> values);
    void setValues(vector<double> values);
};

#endif /* Bin_hpp */


