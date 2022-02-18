//
//  Bounds.hpp
//  VectorPacking
//
//  Created by Michael Kalab on 26.01.18.
//  Copyright © 2018 Michael Kalab. All rights reserved.
//

#ifndef Bounds_hpp
#define Bounds_hpp

#include "Item.hpp"
#include <stdio.h>
#include "FFD_BFD.hpp"
#include "Bin.hpp"

int LCBound(int dim, Item *base, int number_of_items, double cap);
int L2Bound(int dim, Item *base, int number_of_items, double cap, bool increase);
int LHBound(int dim, Item *base, int number_of_items, double cap);
void N_partition1(int *n1, int *n2, Item *base, int number_of_items, double alpha, double beta, double cap);
void N_partition2(int *n1, int *n2, Item *base, int number_of_items, double alpha, double beta, double cap);
int determineS(int dim, Item *base, int number_of_items, double cap, bool increase);
int logfind(Item *base, Item *candidate, int lower, int upper, double cap);


#endif /* Bounds_hpp */
