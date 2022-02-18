//
//  main.cpp
//  VectorPacking
//
//  Created by Michael Kalab on 29.11.17.
//  Copyright © 2017 Michael Kalab. All rights reserved.
//
// Heuristics for Vector Packing
//
//
//
// Reads input for vector bin packing from files and stores them into an arrray. Please consider reading Readme.txt
//

#include <iostream>
#include <vector>
#include <fstream>
#include "Item.hpp"
#include "FFD_BFD.hpp"
#include "Bin.hpp"
#include "ReadFile.hpp"
#include <string>
#include <time.h>
#include "Bounds.hpp"
#include <math.h>
#include "Prompts.hpp"

using namespace std;

int main(int argc, const char * argv[]){
    // open files
    
    bool output_y_n = true;
    /*bool output_experiments = true;*/
    string outputfilename;
    string inputfilename;
    string path;
    string suffix;
    
    
    int ynint = prompt(&inputfilename, &outputfilename, &suffix, &path);
    
    
    /*int a_int = 0;
    int b_int = 0;
    int number_of_files = 500;
    
    string dummyfilename[33];
    string filename;
    
    
    // all file names
    for (int i = 0; i < 6; i++){
        switch (i) {
            case 0:
                a_int = 1;
                b_int = 1000;
                break;
            case 1:
                a_int = 100;
                b_int = 400;
                break;
            case 2:
                a_int = 200;
                b_int = 800;
                break;
            case 3:
                a_int = 50;
                b_int = 200;
                break;
            case 4:
                a_int = 25;
                b_int = 100;
                break;
            case 5:
                a_int = 20;
                b_int = 100;
        }
        for (int dim = 2; dim < 5; dim++)
            dummyfilename[6*(dim-2)+i] = "dim" + to_string(dim) + "[" + to_string(a_int) + "," + to_string(b_int) +  "]";
    }
    for (int dim = 2; dim < 5; dim++){
        dummyfilename[18+dim-2] = "dim" + to_string(dim) + "neq1_";
        dummyfilename[21+dim-2] = "dim" + to_string(dim) + "neq2_";
        dummyfilename[24+dim-2] = "dim" + to_string(dim) + "neq3_";
        dummyfilename[27+dim-2] = "dim" + to_string(dim) + "negcorr";
        dummyfilename[30+dim-2] = "dim" + to_string(dim) + "poscorr";
    }
    
    for (int i = 0; i < 33; i++)
        cout << dummyfilename[i] << endl;

    // all already existing files are used for the experiment
    
    for (int type = 0; type < 33; type++){
        
        cout << dummyfilename[type] << endl;*/
    
        /*string outputfile = "Results100" + dummyfilename[type] + ".csv";
        string output = "LC, L2, LH, LMax, RandomFit, FFDnoavg, FFD, BFD, BFDNoUpdate, BFDLINF, BFDLINFNoUpdate, BFDdot, BFDdotNoUpdate, RandomFittime, FFDnoavgtime, FFDtime, BFDtime, BFDNoUpdatetime, BFDLINFtime, BFDLINFNoUpdatetime, BFDdottime, BFDdotNoUpdatetime\n";*/

        string s;
        int k_FFD, k_FFDnoavg, k_BFDNoUpdate, k_BFD, k_BFDLinf, k_BFDLinfNoUpdate, k_BFDdot, k_BFDdotNoUpdate, k_Random;
        clock_t t_FFD, t_FFDnoavg, t_BFD, t_BFDNoUpdate, t_BFDLinf, t_BFDLinfNoUpdate, t_BFDdot, t_BFDdotNoUpdate;
        clock_t t_Random;
        
        int lc, l2, lh;
    
        /*for (int i = 1; i < number_of_files+1; i++)
            {
                
                cout << "file " << i << endl;
                filename = dummyfilename[type];
                s = to_string(i);
                filename += s;
                filename += ".txt";
    
                // read params from file
                // first parameter is dimension, second is capacity, following item weights irrespective of dimension
         */
                double cap;
                int dim;
                int number_of_items = ReadNumberItemsCapDim(inputfilename, &cap, &dim);
                //number_of_items = 100;
    
                Item *items = new Item[number_of_items];
                ReadFile(inputfilename, cap, dim, number_of_items, &items[0]);
    
    
                //cout << "l2 " << l2 << endl;
        
                /*t_Random = clock();
                k_Random = randomFit(outputfilename+ to_string(i) + "random.txt", dim, &items[0], number_of_items, nullptr, 1, cap, output_y_n);
                t_Random = clock() - t_Random;
        
                t_FFD = clock();
                k_FFD = FFD(outputfilename + to_string(i) + "wFFD.txt", dim, &items[0], number_of_items, &weightedSizes, 1, cap, output_y_n);
                t_FFD = clock() - t_FFD;
                
                qsort(&items[0], number_of_items, sizeof(Item), &randomcompare);
        
                t_FFDnoavg = clock();
                k_FFDnoavg = FFD(outputfilename + to_string(i) + "FFD.txt", dim, &items[0], number_of_items, &sizes, 1, cap, output_y_n);
                t_FFDnoavg = clock() - t_FFDnoavg;
                
    
                t_BFD = clock();
                k_BFD = BF(outputfilename + to_string(i) + "BF*.txt", dim, &items[0], number_of_items, &chooseL2, cap, output_y_n);
                t_BFD = clock() - t_BFD;
        
                t_BFDNoUpdate = clock();
                k_BFDNoUpdate =BFnoUpdate(outputfilename + to_string(i) + "BF.txt", dim, &items[0], number_of_items, &chooseL2, cap, output_y_n);
                t_BFDNoUpdate = clock() - t_BFDNoUpdate;
        
        
                t_BFDdot = clock();
                k_BFDdot = BF(outputfilename + to_string(i) + "BFDot*.txt", dim, &items[0], number_of_items, &chooseDot, cap, output_y_n);
                t_BFDdot = clock() - t_BFDdot;
        
        
                t_BFDdotNoUpdate = clock();
                k_BFDdotNoUpdate = BFnoUpdate(outputfilename + to_string(i) + "BFDot.txt", dim, &items[0], number_of_items, &chooseDot, cap, output_y_n);
                t_BFDdotNoUpdate = clock() - t_BFDdotNoUpdate;
                
                t_BFDLinf = clock();
                k_BFDLinf = BF(outputfilename + to_string(i) + "BFLinf*.txt", dim, &items[0], number_of_items, &chooseLINF, cap, output_y_n);
                t_BFDLinf = clock() - t_BFDLinf;
        
        
                t_BFDLinfNoUpdate = clock();
                k_BFDLinfNoUpdate = BFnoUpdate(outputfilename + to_string(i) + "BFLinf.txt", dim, &items[0], number_of_items, &chooseLINF, cap, output_y_n);
                t_BFDLinfNoUpdate = clock() - t_BFDLinfNoUpdate;*/
    
    if(ynint & 0x800){
        t_Random = clock();
        k_Random = randomFit(path+outputfilename+"RandomFit"+suffix, dim, &items[0], number_of_items, nullptr, 1, cap, output_y_n);
        t_Random = clock() - t_Random;
    }
    
    if (ynint & 0x400){
    t_FFD = clock();
    k_FFD = FFD(path+outputfilename + "wFFDSum"+suffix, dim, &items[0], number_of_items, &weightedSizes, 1, cap, output_y_n);
    t_FFD = clock() - t_FFD;
    }
    
    if(ynint & 0x200){
        t_FFDnoavg = clock();
        k_FFDnoavg = FFD(path+outputfilename + "FFDSum"+suffix, dim, &items[0], number_of_items, &sizes, 1, cap, output_y_n);
        t_FFDnoavg = clock() - t_FFDnoavg;
    }
    
    if(ynint & 0x100){
        t_BFD = clock();
        k_BFD = BF(path+outputfilename + "BF*" + suffix, dim, &items[0], number_of_items, &chooseL2, cap, output_y_n);
        t_BFD = clock() - t_BFD;
    }
    
    
    if(ynint & 0x80){
        t_BFDNoUpdate = clock();
        k_BFDNoUpdate =BFnoUpdate(path+outputfilename + "BF"+suffix, dim, &items[0], number_of_items, &chooseL2, cap, output_y_n);
        t_BFDNoUpdate = clock() - t_BFDNoUpdate;
    }
    
    
    if(ynint & 0x40){
        t_BFDdot = clock();
        k_BFDdot = BF(path+outputfilename + "BFDot*"+suffix, dim, &items[0], number_of_items, &chooseDot, cap, output_y_n);
        t_BFDdot = clock() - t_BFDdot;
    }
    
    
    if(ynint & 0x20){
        t_BFDdotNoUpdate = clock();
        k_BFDdotNoUpdate = BFnoUpdate(path+outputfilename + "BFDot"+suffix, dim, &items[0], number_of_items, &chooseDot, cap, output_y_n);
        t_BFDdotNoUpdate = clock() - t_BFDdotNoUpdate;
    }
    
    if(ynint & 0x10){
        t_BFDLinf = clock();
        k_BFDLinf = BF(path+outputfilename + "BFLinf*" + suffix, dim, &items[0], number_of_items, &chooseLINF, cap, output_y_n);
        t_BFDLinf = clock() - t_BFDLinf;
    }
    
    
    if (ynint & 0x8){
        t_BFDLinfNoUpdate = clock();
        k_BFDLinfNoUpdate = BFnoUpdate(path+outputfilename + "BFLinf.txt"+suffix, dim, &items[0], number_of_items, &chooseLINF, cap, output_y_n);
        t_BFDLinfNoUpdate = clock() - t_BFDLinfNoUpdate;
    }
    lc = 0;
    l2 = 0;
    lh = 0;
    vector<int> bounds;
    if(ynint & 0x4)
        lc =LCBound(dim, &items[0], number_of_items, cap);
    if(dim == 2 && (ynint & 0x2))
        l2 = L2Bound(dim, &items[0], number_of_items, cap, 0);
    if(dim == 2 && (ynint & 0x1))
        lh = LHBound(dim, &items[0], number_of_items, cap);
    if(ynint & 0x7)
        generateBounds(path+outputfilename + "bounds" + suffix, lc,l2,lh, ynint, dim);


    
    
                delete[] items;
    
    
                // output for experiments
              /*  output += to_string(lc) + "," + to_string(l2) + "," + to_string(lh) + "," + to_string(max(lc,max(l2, lh))) +  ", " + to_string(k_Random) + "," + to_string(k_FFDnoavg) + "," + to_string(k_FFD) + "," + to_string(k_BFD) + "," + to_string(k_BFDNoUpdate) + "," + to_string(k_BFDLinf) + "," + to_string(k_BFDLinfNoUpdate) + "," + to_string(k_BFDdot) + "," + to_string(k_BFDdotNoUpdate) + "," + to_string(t_Random) +  "," + to_string(t_FFDnoavg) + "," + to_string(t_FFD) + "," + to_string(t_BFD) + "," + to_string(t_BFDNoUpdate) + "," + to_string(t_BFDLinf) + "," + to_string(t_BFDLinfNoUpdate) + ","+ to_string(t_BFDdot) + "," + to_string(t_BFDdotNoUpdate) +  "\n";
            }
            
            if(output_experiments){
            fstream file(outputfile, ios::out);
                if (!file.is_open())
                    cout << "ALERT" << endl;
            file << output;
            file.close();
            }
        }*/
    
    cout << "Output written to files in " << path << "." << endl << endl;
    
    return 0;
}
