//
//  ReadFile.cpp
//  VectorPacking
//
//  Created by Michael Kalab on 29.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//

#include "ReadFile.hpp"
#include "Item.hpp"


using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
// ReadNumberItemsCapDim
// reads number of items, capacity and dimension of a file
// @param filename: name of the file
// @param *cap: pointer to capacity
// @param *dim: pointer to dimension
// @return: returns number of items



int ReadNumberItemsCapDim(string filename, double *cap, int *dim){
    ifstream file;
    file.open(filename, ios::in);
    if (!(file.is_open())){
        std::cerr << "There was a problem opening the input file! Process aborted!\n";
        exit(1);//exit or do additional error checking
    }
    int number_of_items = 0;
    double i;
    double cap_double;
    if(!(file >> *dim) || !(file >> cap_double)){
        cerr << "Input file has too few arguments. Process aborted!\n";
        exit(1);
    }
    *cap = (int)cap_double;
    while (file >> i){
        number_of_items++;
    }
    if(*dim < 1){
        cerr << "Dimension must be positive! Process aborted! \n";
        exit(1);
    }
    if (*cap < 0){
        cerr << "Capacity must be non-negative! Process aborted! \n";
        exit(1);
    }
    
    file.close();
    return number_of_items/(*dim);
}


////////////////////////////////////////////////////////////////////////////////////////
// ReadFile
// stores items to an array
// @param filename: name of the file
// @param cap: capacity
// @param dim: dimension
// @param *base: points to first element in array

void ReadFile(string filename, double cap, int dim, int number_of_items, Item *base){
    
    ifstream file(filename, ios::in);
    vector<double> itemvalues;
    int number_of_values = dim  * number_of_items;
    itemvalues.reserve(number_of_values);
    Item dummyitem;
    double num;
    file >> num;
    file >> num;
    
    int pos = 2;
    string yes;
    
    //keep storing values from the text file as long as data are provided:
    while (file >> num) {
        pos++;
        itemvalues.push_back(num);
        if (num > cap){
            cerr << "Value in position " << pos << " is bigger than capacity. Process aborted" << endl;
            exit(1);
        }
        if (num < 0){
            cerr << "Value in position " << pos << " is negative. Process aborted!";
                exit(1);
            }
        }

    
    number_of_values -= (number_of_values %dim);
    
    int j = 0;
    for (int i = 0; i < number_of_values; i++) {
        (base+j)->_values.push_back(itemvalues[i]);
        if (i%dim == dim -1){
            (base+j)->setIndex(j);
            j++;
        }
    }
    file.close();
}
