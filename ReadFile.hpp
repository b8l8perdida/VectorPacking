//
//  ReadFile.hpp
//  VectorPacking
//
//  Created by Michael Kalab on 29.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//

#ifndef ReadFile_hpp
#define ReadFile_hpp

#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "Item.hpp"

using namespace std;

int ReadNumberItemsCapDim(string filename, double *, int *);
void ReadFile(string filename, double, int, int, Item *);

#endif /* ReadFile_hpp */
