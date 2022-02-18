//
//  FFD_BFD.hpp
//  VectorPacking
//
//  Created by Michael Kalab on 30.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//

#ifndef FFD_BFD_hpp
#define FFD_BFD_hpp

#include <stdio.h>
#include <fstream>
#include "Item.hpp"
#include "Bin.hpp"
#include <vector>
#include <iostream>
#include <numeric>
#include <math.h>


int FFD(string filename, int dim, Item* base, int number_of_items, void(*scalarfunction)(int, Item *base, int, double), bool quicksort, double cap, bool output);
int randomFit(string filename, int dim, Item *base, int number_of_items, void (*scalarfunction)(int, Item*, int, double), bool quicksort, double cap, bool output_y_n);
Item* chooseL2(int dim, Bin *bin, Item *base, int number_of_items, double cap, vector<double> a);
Item* chooseDot(int dim, Bin *bin, Item *base, int number_of_items, double cap, vector<double> a);
Item* chooseLINF(int dim, Bin *bin, Item *base, int number_of_items, double cap, vector<double> a);
int BF(string filename, int dim, Item *base, int number_of_items, Item* (choose)(int, Bin*, Item*, int, double, vector<double> a), double cap, bool output);
int BFnoUpdate(string filename, int dim, Item *base, int number_of_items, Item* (choose)(int, Bin*, Item*, int, double, vector<double>), double cap, bool output);
int compare(const void *p1, const void *p2);
int compare1st(const void *p1, const void *p2);
int compare2nd(const void *p1, const void *p2);
int randomcompare(const void *p1, const void *p2);
bool fitBin(int dim, Bin *bin, Item *item, double cap);

void adjustBin(int dim, Bin *bin, Item *item);
void sizes(int dim, Item *base, int number_of_items, double cap);
void weightedSizes(int dim, Item *base, int number_of_items, double cap);
void randomSizes(int dim, Item *base, int number_of_items, double cap);

void generateFile(string filename, int* sigma, int number_of_items, int number_of_bins);
void generateBounds(string filename, int lc, int l2, int lh, int ynint, int dim);


vector<double> calculateA(int, Item*, int);


#endif /* FFD_BFD.hpp */
