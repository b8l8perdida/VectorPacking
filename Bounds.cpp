//
//  Bounds.cpp
//  VectorPacking
//
//  Created by Michael Kalab on 26.01.18.
//  Copyright © 2018 Michael Kalab. All rights reserved.
//

#include "Bounds.hpp"
#include <math.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// LCBound
// calculates LC for a given instance
// @param base: pointer to first item
// @param number_of_items: number of items
// @param dim: dimension
// @param cap: capacity
//
// @return: returns LC



int LCBound(int dim, Item *base, int number_of_items, double cap){
    if (!number_of_items)
        return 0;
    double lcbound = 0;
    double bound;
    for (int i = 0; i < dim; i++){
        bound = 0;
        for(int j = 0; j < number_of_items; j++){
            bound += (base+j)->getValue(i);
        }
        if (lcbound < bound)
            lcbound = bound;
    }
    
    
    return ceil(lcbound/cap);
}


////////////////////////////////////////////////////////////////////////////////
// LHBound
// calculates LH for a given instance
// @param base: pointer to first item
// @param number_of_items: number of items
// @param dim: dimension
// @param cap: capacity
//
// @return: returns LH

// LHBound takes FAR too much time, we only consider 9 different fixed values for a and b
int LHBound(int dim, Item *base, int number_of_items, double cap){
    int LHBound = 0;
    int LH;
    //double half = cap/2.0;
    
    vector<double> alpha;
    vector<double> beta;
    /*for(int i = 0; i < number_of_items; i++){
        (base+i)->setScalarValue(min( (base+i)->getValue(0), cap - (base+i)->getValue(0) ));
    }
    qsort(base, number_of_items, sizeof(Item), compare);
    
    double alpha1 = INFINITY;
    for (int i = 0; i < number_of_items; i++){
        if ((base+i)->getScalarValue() < alpha1){
            alpha1 = (base+i)->getScalarValue();
            alpha.push_back(alpha1);
        }
    }
    
    for(int i = 0; i < number_of_items; i++){
        (base+i)->setScalarValue(min( (base+i)->getValue(1), cap - (base+i)->getValue(1) ));
    }
    qsort(base, number_of_items, sizeof(Item), compare);
    
    double beta1 = INFINITY;
    for (int i = 0; i < number_of_items; i++){
        if ((base+i)->getScalarValue() < beta1){
            beta1 = (base+i)->getScalarValue();
            beta.push_back(beta1);
        }
    }*/
    
    int n1, n2;
    
    for (int i = 0; i < 9; i++){
        alpha.push_back((1+i)*cap/20);
        beta.push_back((1+i)*cap/20);
    }
    
    int alpha_len = (int)alpha.size();
    int beta_len = (int)beta.size();
    
    for(int i = 0; i < alpha_len; i++){
        //cout << i << endl;
        for(int j = 0; j < beta_len; j++){
            //cout << j << endl;
            N_partition1(&n1, &n2, base, number_of_items, alpha[i], beta[j], cap);
            LH = n1 + max(LCBound(dim, base+n1, n2, cap),L2Bound(dim, base+n1, n2, cap,0));
            LHBound = max(LH, LHBound);
            N_partition2(&n1, &n2, base, number_of_items, alpha[i], beta[j], cap);
            LH = L2Bound(dim, base, n1, cap, 0) +  max(LCBound(dim, base+n1, n2, cap), L2Bound(dim, base+n1, n2, cap,0));
            LHBound = max(LH, LHBound);
        }
    }
    return LHBound;
}




////////////////////////////////////////////////////////////////////////////////
// N_partition1
// determines N_1^1 and N_2^1
// @param n1: pointer to the number of items in N_1^1
// @param n2: pointer to the number of items in N_2^1
// @param base: pointer to first element
// @param number_of_items: number of items
// @param alpha: alpha to determine N_1^1 and N_2^1
// @param beta: alpha to determine N_1^1 and N_2^1
// @param cap: capacity
//

void N_partition1(int *n1, int *n2, Item *base, int number_of_items, double alpha, double beta, double cap){
    int n1_int = 0;
    int n2_int;
    Item dummyitem;
    alpha = cap - alpha;
    beta = cap - beta;
    for (int i = 0; i < number_of_items; i++){
        if ((base+i)->getValue(0) > alpha && (base+i)->getValue(1) > beta){
            dummyitem = *(base+i);
            *(base+i) = *(base+n1_int);
            *(base+n1_int) = dummyitem;
            n1_int++;
        }
    }
    n2_int = n1_int;
    alpha = cap - alpha;
    beta = cap - beta;
    
    for (int i = n1_int; i < number_of_items; i++){
        if((base+i)->getValue(0) >= alpha || (base+i)->getValue(1) >= beta){
            dummyitem = *(base+i);
            *(base+i)=*(base+n2_int);
            *(base+n2_int) = dummyitem;
            n2_int++;
        }
    }
    *n1 = n1_int;
    *n2 = n2_int-n1_int;
}



////////////////////////////////////////////////////////////////////////////////
// N_partition2
// determines N_1^2 and N_2^2
// @param n1: pointer to the number of items in N_1^2
// @param n2: pointer to the number of items in N_2^2
// @param base: pointer to first element
// @param number_of_items: number of items
// @param alpha: alpha to determine N_1^2 and N_2^^2
// @param beta: alpha to determine N_1^2 and N_2^2
// @param cap: capacity
//


void N_partition2(int *n1, int *n2, Item *base, int number_of_items, double alpha, double beta, double cap){
    int n1_int = 0;
    int n2_int;
    Item dummyitem;
    alpha = cap - alpha;
    beta = cap - beta;
    for (int i = 0; i < number_of_items; i++){
        if ((base+i)->getValue(0) > alpha || (base+i)->getValue(1) > beta){
            dummyitem = *(base+i);
            *(base+i) = *(base+n1_int);
            *(base+n1_int) = dummyitem;
            n1_int++;
        }
    }
    n2_int = n1_int;
    alpha = cap - alpha;
    beta = cap - beta;
    
    for (int i = n1_int; i < number_of_items; i++){
        if((base+i)->getValue(0) >= alpha && (base+i)->getValue(1) >= beta){
            dummyitem = *(base+i);
            *(base+i)=*(base+n2_int);
            *(base+n2_int) = dummyitem;
            n2_int++;
        }
    }
    *n1 = n1_int;
    *n2 = n2_int-n1_int;
}


////////////////////////////////////////////////////////////////////////////////
// L2Bound
// calculates L2 for a given instance
// @param base: pointer to first item
// @param number_of_items: number of items
// @param dim: dimension
// @param cap: capacity
//
// @return: returns LC


int L2Bound(int dim, Item *base, int number_of_items, double cap, bool increase){
    int S_card = determineS(dim, base, number_of_items, cap, increase); //correct
    
    qsort(base, S_card, sizeof(Item), compare1st);

    
    int maxmatching = 0;
    int matched;
    Item dummyitem;
    double bestmatch;
    Bin bin;
    int logfound;
    
    bool matchednodes[S_card];
    
    for (int i = 0; i < S_card; i++)
        matchednodes[i] = false;
    
    
    for (int i = 0; i < S_card; i++){
        if (matchednodes[i])
            continue;
        matched = 0;
        bestmatch = 0;
        bin = Bin(*(base+i));
        logfound = logfind(base, base+i, i+1, S_card-1, cap);
        for (int j = logfound; j < S_card; j++){
            if(matchednodes[j])
                continue;
            if ((base+j)->getValue(1) > bestmatch && fitBin(dim, &bin, base+j, cap)){
                    bestmatch = (base+j)->getValue(1);
                    matched = j;
                }
            }
        if(matched){
            maxmatching++;
            matchednodes[matched] = true;
        }
    }
    return S_card-maxmatching;
}


// makes determineS slightly faster, looks for first fitting item
int logfind(Item *base, Item *candidate, int lower, int upper, double cap){

    if((upper-lower) < 2)
        return lower;
    
    double fitfirst = candidate->getValue(0);
    int find = (upper+lower)/2;
    
    if((base+find)->getValue(0) + fitfirst > cap)
        return logfind(base, candidate, find, upper, cap);
    else
        return logfind(base, candidate, lower, find, cap);
}




////////////////////////////////////////////////////////////////////////////////
// determineS
// @param dim: dimension
// @param base: pointer to first element
// @param number_of_items: number of items
// @param cap: capacity
// @param increase: 0 if called by LH, 1 if called by LH
//
// @return: returns number of items in S

int determineS(int dim, Item *base, int number_of_items, double cap, bool increase){
    Item dummyitem;
    double third = cap/3.0;
    bool append;
    int S_card = 0;
    for (int i = 0; i < number_of_items; i++){
        append = true;
        for(int j = 0; j < dim; j++){
            if((base+i)->getValue(j) <= third)
                append = false;
        }
        if(append){
            dummyitem = *(base+i);
            *(base+i) = *(base+S_card);
            *(base+S_card) = dummyitem;
            S_card++;
        }
    }
    if(!increase)
        return S_card;
    
    vector<double> a = calculateA(dim, base, number_of_items);
    vector<int> appended;
    for (int i = 0; i < S_card; i++)
        appended.push_back(i);
    
    for (int i = S_card; i < number_of_items; i++)
        (base+i)->setScalarValue(a[0] * (base+i)->getValue(0)+a[1]*(base+i)->getValue(1));
    
    qsort((base+S_card), (number_of_items-S_card), sizeof(Item), compare);
    
    for (int i = S_card; i < number_of_items; i++){
        append = true;
        for (int j = 0; j < appended.size(); j++){
            for (int k = 0; k < j; k++){
                for (int d = 0; d < dim; d++){
                    if((base+i)->getValue(d)+(base+appended[j])->getValue(d)+(base+appended[k])->getValue(d) < cap){
                        append = false;
                        break;
                    }
                }
                if (!append)
                    break;
            }
            if (!append)
                break;
        }
        if (append){
            appended.push_back(i);
        }
    }
    
    for (int i = 0; i < appended.size(); i++){
        dummyitem = *(base+appended[i]);
        *(base+appended[i]) = *(base+S_card+i);
        *(base+S_card+i) = dummyitem;
    }
    
    return (int)(S_card + appended.size());
}


