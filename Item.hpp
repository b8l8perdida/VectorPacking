//
//  Item.hpp
//  VectorPacking
//
//  Created by Michael Kalab on 29.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//

#ifndef Item_hpp
#define Item_hpp

#include <stdio.h>
#include <vector>
#include <cstdlib>

using namespace std;

class Item{
    
private:
    double _scalarvalue;
    int _index;
public:
    vector<double> _values;
    Item();
    Item(double scalarvalue, vector<double> values, int ind);
    const double getScalarValue();
    const double getValue(int i);
    vector <double> getValues();
    void addScalarValue(double scalar);
    void setScalarValue(double scalar);
    vector<double> adjustValues(vector<Item> items);
    void setIndex(int ind);
    const int getIndex();
};

#endif /* Item_hpp */
