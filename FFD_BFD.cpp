//
//  FFD_BFD.cpp
//  VectorPacking
//
//  Created by Michael Kalab on 30.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//

#include "FFD_BFD.hpp"
#include <random>


using namespace std;


// randomcompare is only used for shuffling the items
int randomcompare(const void *p1, const void *p2){
    mt19937 generator;
    uniform_int_distribution<int> dist(0,1);
    bool up = dist(generator);
    if (up)
        return 1;
    else
        return -1;
}

// compare is a routine used by the C++ built-in qsort method
int compare(const void *p1, const void *p2){
    if (( *(Item*)p1).getScalarValue() < ( *(Item*)p2).getScalarValue())
        return 1;
    else
        return -1;
}

// compare1st is a routine used by the C++ built-in qsort method
int compare1st(const void *p1, const void *p2){
    if((*(Item*)p1).getValue(0) < (*(Item*)p2).getValue(0))
        return 1;
    else
        return -1;
}

// as compar1st but for 2nd component
int compare2nd(const void *p1, const void *p2){
    if ((*(Item*)p1).getValue(1) < (*(Item*)p2).getValue(1))
        return  1;
    else
        return -1;
}


// writes the the output to a file

void generateFile(string filename, int* sigma, int number_of_items, int number_of_bins){
    string output = "number_of_bins, " +  to_string(number_of_bins) + "\n";
    ofstream file(filename, ios::out);
    if (!file.is_open()){
        cerr << "There was a problem generating the output file" << endl;
        exit(1);
    }
    for (int i = 0; i < number_of_items; i++){
        output += to_string(i+1) + ", " + to_string(*(sigma+i)+1) +  "\n";
    }
    file << output;
    file.close();
}

void generateBounds(string filename, int lc, int l2, int lh, int ynint, int dim){
    string output = "";
    fstream file(filename, ios::out);
    if (ynint & 0x4)
        output += "LC, " + to_string(lc) + "\n";
    if( ynint & 0x2 && dim ==2)
        output += "L2, " + to_string(l2) + "\n";
    if (ynint & 0x1 && dim == 2)
        output += "LH, " + to_string(lh) + "\n";
    file << output;
    file.close();
    
}

/******************************************************************************/
/******************************************************************************/
// Subroutines
/******************************************************************************/
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// fitBin
// check if item fits into bin
// @param bin: currently open bin
// @param item: item to be placed in the bin
// @param dim: dimension
//
// @return: returns true if the residual capacities are bigger than the item (componentwise)
//          returns false else


bool fitBin(int dim, Bin *bin, Item *item, double cap){
    for (int i =0; i< dim; i++){
        if (bin->getValue(i)+ item->getValue(i) > cap){
            return false;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// update total size of a bin in all dimensions
// @params item, bin: item is placed into bin and content of the bin is updated
// @param dim: dimension
//
void adjustBin(int dim, Bin *bin, Item *item){
    bin->setValues(item->_values);
}

////////////////////////////////////////////////////////////////////////////////
// calculates a = (a_i)_{i=1,..,d} for decreasing order of elements
// @param base: the first element of the list
// @param number_of_items: total number of items
// @return: vector containing the total demands for each dimension

vector<double> calculateA(int dim, Item *base, int number_of_items){
    vector<double> a;
    double a_i;
    for (int i = 0; i< dim; i++){
        a_i = 0;
        for (int j = 0; j < number_of_items; j++){
            a_i += (base+j)->getValue(i);
        }
        a.push_back(a_i);
    }
    return a;
}

/******************************************************************************/
/******************************************************************************/
// Methods for finding the item that fits best
/******************************************************************************/
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// chooseL2: finds fitting item which minimizes the weighted L2-norm of residual
// capacities
// @param bin: the currently open bin
// @param base: first item in the list
// @param number_of_items: number of items
// @param a: vector containing the (actualized) total demand in each dimension
// @param dim: dimension
//
// @return: returns best fitting item that still fits into param bin
//          returns nullptr if no more items fit into param bin

Item* chooseL2(int dim, Bin *bin, Item *base, int number_of_items, double cap, vector<double> a){
    vector<double> remaincaps;
    Item *chosen = nullptr;
    double remaincap;
    double minres= std::numeric_limits<long double>::max();
    double res;
    double add;
    for (int i = 0; i< dim; i++){
        remaincap = cap - bin->getValue(i);
        remaincaps.push_back(remaincap);
    }
    
    for (int i = 0; i < number_of_items; i++){
        if (fitBin(dim, bin, (base+i), cap)){
            res = 0;
            for (int j = 0; j < dim; j++){
                add = (remaincaps[j]-(base+i)->getValue(j))*(remaincaps[j]-(base+i)->getValue(j));
                res += a[j]*add;
            }
            if (res < minres){
                minres = res;
                chosen = base+i;
            }
        }
    }
    return chosen;
}


////////////////////////////////////////////////////////////////////////////////
// chooseDot: finds fitting item which maximizes the weighted scalar product of
// the item and the residual capacities
// @param bin: the currently open bin
// @param base: first item in the list
// @param number_of_items: number of items
// @param a: vector containing the (actualized) total demand in each dimension
// @param dim: dimension
//
// @return: returns best fitting item that fits into param bin
//          returns nullptr if no more item fits into param bin

Item* chooseDot(int dim, Bin *bin, Item *base, int number_of_items, double cap, vector<double> a){
    vector<double> remaincaps;
    Item *chosen = nullptr;
    double remaincap;
    double minres= 0;
    double res;
    double add;
    for (int i = 0; i< dim; i++){
        remaincap = cap - bin->getValue(i);
        remaincaps.push_back(remaincap);
    }
    
    for (int i = 0; i < number_of_items; i++){
        if (fitBin(dim, bin, (base+i), cap)){
            res = 0;
            for (int j = 0; j < dim; j++){
                add = remaincaps[j]*(base+i)->getValue(j);
                res += a[j]*add;
            }
            if (res > minres){
                minres = res;
                chosen = base+i;
            }
        }
    }
    return chosen;
}

////////////////////////////////////////////////////////////////////////////////
// chooseLINF: finds fitting item which minimizes the WEIGHTED LINF-norm of residual
// capacities
// @param bin: the currently open bin
// @param base: first item in the list
// @param number_of_items: number of items
// @param a: vector containing the (actualized) total demand in each dimension
// @param dim: dimension
//
// @return: returns best fitting item that still fits into bin

Item* chooseLINF(int dim, Bin *bin, Item *base, int number_of_items, double cap, vector<double> a){
    vector<double> remaincaps;
    Item *chosen = nullptr;
    double remaincap;
    double minres= std::numeric_limits<long double>::max();
    double res = 0;
    double Linf;
    for (int i = 0; i< dim; i++){
        remaincap = cap - bin->getValue(i);
        remaincaps.push_back(remaincap);
    }
    
    for (int i = 0; i < number_of_items; i++){
        if (fitBin(dim, bin, (base+i), cap)){
            for (int j = 0; j < dim; j++){
                Linf = (remaincaps[j]-(base+i)->getValue(j));
                res = max(a[j]*Linf, res);
            }
            if ( res < minres){
                minres = res;
                chosen = base+i;
            }
        }
    }
    return chosen;
}



/******************************************************************************/
/******************************************************************************/
// Choices for size
/******************************************************************************/
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// assigns size(I) to all items I, stored in _scalarvalue
//
// @param base: points to first item in list
// @param number_of_items: number of items
// @param cap: capacity of a bin in each dimension
// @param dim: dimension
//
// @scalarvalue: size of the item

void sizes(int dim, Item *base, int number_of_items, double cap){
    for (int i =0; i< number_of_items; i++){
        double scalarvalue = 0;
        for (int j = 0; j< dim; j++)
            scalarvalue += (base+i)->getValue(j); // no weights are used
        (base+i)->setScalarValue(scalarvalue);
    }
}

////////////////////////////////////////////////////////////////////////////////
// assigns weithedSize(I) to all items I
// @param base: points to first item in list
// @param number_of_items: number of items
// @param cap: capacity of a bin in each dimension
// @param dim: dimension
//
// @scalarvalue: size of the item


void weightedSizes(int dim, Item *base, int number_of_items, double cap){
    double a_i[dim];
    for (int i = 0; i< dim; i++)
        a_i[i]=0;
    for (int i = 0; i< number_of_items; i++){
        for (int j = 0; j< dim; j++){
            a_i[j] += (base+i)->getValue(j);
        }
    }
    
    for (int i = 0; i < number_of_items; i++){
        double scalarvalue = 0;
        for (int j = 0; j < dim; j++){
            scalarvalue+= a_i[j]*(base+i)->getValue(j); // weights are used for size
        }
        (base+i)->setScalarValue(scalarvalue);
    }
}

////////////////////////////////////////////////////////////////////////////////
// assigns size(I) to all items I
// @param base: points to first item in list
// @param number_of_items: number of items
// @param cap: capacity of a bin in each dimension, unused
//


void randomSizes(int dim, Item *base, int number_of_items, double cap){
    random_device device;
    uniform_int_distribution<int> distribution(0, number_of_items*1000);
    mt19937 generator(device());
    for (int i = 0; i < number_of_items; i++){
        (base+i)->setScalarValue(distribution(generator));
    }
}


/******************************************************************************/
// FirstFit routines
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// RandomFirstFit: takes items in the order of their appearance and assigns them
// greedily to the bins in which they fit first
//
// @param base: first item in the list
// @param number_of_items: number of items
// @param dim: dimension
// @param scalarfunction: either FFDsum or FFDavgsum
// @param cap: capacity of bin in each dimension
// @param ouput_y_n: decides if solution is written to an output file
// @param filename: name of the file where sigma is stored
//
// @return: returns number bins that have been needed packing the items

int randomFit(string filename, int dim, Item *base, int number_of_items, void (*scalarfunction)(int, Item*, int, double), bool quicksort, double cap, bool output_y_n){
    int number_of_bins = 0;
    
    vector<Bin> bins;
    bool bin_chosen = false;
    Bin bin;
    
    int *sigma = new int[number_of_items];
    
    for (int i=0; i < number_of_items; i++){
        bin_chosen = false;
        
        for(int j =0; j < number_of_bins; j++){
            if (fitBin(dim, &bins[j], (base+i), cap)){
                adjustBin(dim, &bins[j], (base+i));
                bin_chosen = true;
                sigma[(base+i)->getIndex()] = j;
                break;
            }
        }
        if (!bin_chosen){
            bin = Bin(*(base+i));
            bins.push_back(bin);
            sigma[(base+i)->getIndex()] = number_of_bins;
            number_of_bins++;
        }
    }
    bins.clear();
    bins.resize(0);
    
    if(output_y_n)
        generateFile(filename, &sigma[0], number_of_items, number_of_bins);
    
    delete[] sigma;
    
    return number_of_bins;
}


////////////////////////////////////////////////////////////////////////////////
// FirstFitDecreasing: sorts items by their size according to
// FFDsum or FFDavgsum and places them greedily into bins
// If an item can't be packed into an already open bin a new bin is opened
//
// @param base: first item in the list
// @param number_of_items: number of items
// @param dim: dimension
// @param scalarfunction: either FFDsum or FFDavgsum
// @param quicksort: decides wheter qsort or mergesort is used
// @param cap: capacity of bin in each dimension
// @param ouput_y_n: decides if solution is written to an output file
// @param filename: name of the file where sigma is stored
//
// @return: returns number bins that have been needed packing the items

int FFD(string filename, int dim, Item *base, int number_of_items, void (*scalarfunction)(int, Item*, int, double), bool quicksort, double cap, bool output_y_n){
    
    int number_of_bins = 0;
    
    scalarfunction(dim, base, number_of_items, cap);
    
    if(quicksort)
        qsort(base, number_of_items, sizeof(Item), compare);
    /*else
        mergesort(base, number_of_items, sizeof(Item), compare);*/ //does not work for windows...
                       
    vector<Bin> bins;
    bool bin_chosen = false;
    Bin bin;
    
    int *sigma = new int[number_of_items];
    
    for (int i=0; i < number_of_items; i++){
        bin_chosen = false;
        
        for(int j =0; j < number_of_bins; j++){
            if (fitBin(dim, &bins[j], (base+i), cap)){
                adjustBin(dim, &bins[j], (base+i));
                bin_chosen = true;
                sigma[(base+i)->getIndex()] = j;
                break;
            }
        }
        if (!bin_chosen){
            bin = Bin(*(base+i));
            bins.push_back(bin);
            sigma[(base+i)->getIndex()] = number_of_bins;
            number_of_bins++;
        }
    }
    
    if(output_y_n)
        generateFile(filename, &sigma[0], number_of_items, number_of_bins);
    bins.clear();
    bins.resize(0);
    
    delete[] sigma;
    return number_of_bins;
}



/******************************************************************************/
// BestFit routines
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// BF: finds item which fits best according to param choose and places
// it into currently open bin
// If no more items can be packed into the current bin the a_k's are updated i.e. the total demand per
// dimension gets lower.
//
// @param base: first item in the list
// @param number_of_items: number of items
// @param choose: one of the above routines in 'Choices for finding the best fitting item'
// may be used
// @param dim: dimension
// @param boolsort: decides wheter qsort or mergesort is used
// @param cap: capacity of bin in each dimension
// @param ouput_y_n: decides if solution is written to an output file
// @param filename: name of the file where sigma is stored
//
// @return: returns number bins that have been needed packing the items

int BF(string filename, int dim, Item *base, int number_of_items, Item* (choose)(int, Bin*, Item*, int, double, vector<double>), double cap, bool output_y_n){
    vector<double> dummyvalues;
    int unpacked_items = number_of_items;
    Item *chosen;
    Item dummyitem;
    int number_of_bins = min(number_of_items,1);
    for (int i = 0; i < dim; i++)
        dummyvalues.push_back(0);
    vector<double> a = calculateA(dim, base, number_of_items);
    Bin bin = Bin(dummyvalues);
    
    int *sigma = new int[number_of_items];

    while(unpacked_items){
        chosen = choose(dim, &bin, base, unpacked_items, cap, a);
        if (chosen != nullptr){
            adjustBin(dim, &bin, chosen);
            sigma[chosen->getIndex()] = number_of_bins-1;
            dummyitem = *chosen;
            *chosen = *(base+unpacked_items-1);
            *(base+unpacked_items-1) = dummyitem;
            unpacked_items--;
        }
        else{
            for (int i = 0; i < dim; i++){
                a[i] -= bin.getValue(i);
            }
            bin = Bin(dummyvalues);
            number_of_bins++;
        }
    }
    
    if(output_y_n)
        generateFile(filename, &sigma[0], number_of_items, number_of_bins);
    
    delete[] sigma;
    
    return number_of_bins;
}


////////////////////////////////////////////////////////////////////////////////
// BFDnoUpdate: finds item which fits best according to param choose and places
// it into currently open bin
//
// the a_k's are NOT updated
//
//
// @param base: first item in the list
// @param number_of_items: number of items
// @param choose: one of the above routines in 'Choices for finding the best fitting item'
// may be used
// @param dim: dimension
// @param boolsort: decides wheter qsort or mergesort is used
// @param cap: capacity of bin in each dimension
// @param ouput_y_n: decides if solution is written to an output file
// @param filename: name of the file where sigma is stored
//
// @return: returns number bins that have been needed packing the items


int BFnoUpdate(string filename, int dim, Item *base, int number_of_items, Item* (choose)(int, Bin*, Item*, int, double, vector<double>), double cap, bool output_y_n){
    vector<double> dummysizes;
    Item *chosen;
    Item dummyitem;
    int number_of_bins = min(number_of_items,1);
    for (int i = 0; i < dim; i++)
        dummysizes.push_back(0);
    vector<double> a = calculateA(dim, base, number_of_items);
    Bin bin = Bin(dummysizes);
    
    int *sigma = new int[number_of_items];
    
    int unpackeditems = number_of_items;
    
    while(unpackeditems){
        chosen = choose(dim, &bin, base, unpackeditems, cap, a);
        if (chosen != nullptr){
            adjustBin(dim, &bin, chosen);
            sigma[chosen->getIndex()] = number_of_bins-1;
            dummyitem = *chosen;
            *chosen = *(base+unpackeditems-1);
            *(base+unpackeditems-1) = dummyitem;
            unpackeditems--;
        }
        else{
            bin = Bin(dummysizes);
            number_of_bins++;
        }
    }
    
    
    if(output_y_n)
        generateFile(filename, &sigma[0], number_of_items, number_of_bins);
    
    delete[] sigma;
    
    return number_of_bins;
}
