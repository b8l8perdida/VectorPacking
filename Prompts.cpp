//
//  Prompts.cpp
//  VectorPacking
//
//  Created by Michael Kalab on 28.01.18.
//  Copyright © 2018 Michael Kalab. All rights reserved.
//

#include "Prompts.hpp"
using namespace std;

// checks for a given path if it contains a file
/*bool is_file(string path) {
    struct stat buf;
    const char *cpath = path.c_str();
    stat(cpath, &buf);
    return S_ISREG(buf.st_mode);
}*/

// checks for a given path if it contains a directory
/*bool is_dir(string path) {
    struct stat buf;
    const char *cpath = path.c_str();
    stat(cpath, &buf);
    return S_ISDIR(buf.st_mode);
}*/
 

// handling of input and output names
int prompt(string *inputfilename, string *outputfilename, string *suffix, string *outputpath){
    string filename;
    string dummyname;
    string ynstring;
    
    cout << "Welcome to Vector packing! This program analyzes instances of Vector Packing using various algorithms. Please consider reading Readme.txt and/or 'Heuristics for Vector Bin Packing'!" << endl << endl;
    cout << "Please type the path and the name of your input file relative to the program and press Enter. Use slashes instead of backslashes. \n" << endl;
    cin >> *inputfilename;
    cout << "\n";
    ifstream file;
    file.open(*inputfilename, fstream::in);
    if(!file){
        cout << "File not found!\n\n" << endl;
        exit(1);
    }
        
    /*else if(!is_file(*inputfilename)){
        cout << "Given path is not a file!\n\n" << endl;
        exit(1);
    }*/

    cout << "Available algorithms: RandomFit FFDSum wFFDSum BF* BF BFDot* BFDot BFLINF* BFLINF\n" << endl;
    cout << "Available bounds for dimension d=2: LC L2 LH\n" << endl;
    cout << "Available bounds for dimension d > 2: LC" << endl;
    cout << "For each algorithm a seperate output file containing the solutions is generated. To avoid overloading each output file receives the suffix of the respective algorithm. One file contains all available and chosen bounds and receives the suffix 'bounds'." << endl;
    cout << "Please type a path for the output files and press 'Enter'!\n Example: If your output file should be stored in /Desktop then type '/Desktop'.\n" << endl;
    cin >> *outputpath;
    cout << "\n";
    
    /*if(!is_dir(*outputpath)){
        cout << "Given directory does not exist!\n" << endl;
        exit(1);
    }*/
    cout << "Please choose a name for the output files, do not add the suffix.\n Example: If your output file should be called 'test.txt' then type 'test'. \n" << endl;
    cin >> *outputfilename;
    cout << "\n";
    cout << "Please choose a suffix for your output files, do not write a dot. \n Example: If your output file should be called 'test.txt' then type 'txt'!\n\n";
    cin >> *suffix;
    *suffix = "."+*suffix;
    cout << "\n";
    cout << "If you would like all algorithms and bounds to be executed and then type 'A' and press 'Enter'!" << endl;
    cout << "Else type in a sequence of 'Y' and 'N' without space. Each of the Ys and Ns stands for the execution of algorithms and bounds in the following order:\n RandomFit wFFDSum FFDSum BF* BF BFDot* BFDot BFLINF* BFLINF LC L2 LH" << endl;
    cout << "Example: NYNNYNYYYNY executes and generates an output file for wFFDSum, BF, BFDot, BFLINF* BFLINF. It also generates an output file containing LC and (if the dimension is 2) L2 and LF. \n\n";
    cin >> ynstring;
    cout << endl;
    if(ynstring =="A")
        return 0xFFF;

    char yesno;
    int ynint = 0;
    bool not_understood = true;
    int length = (int)ynstring.length();
    if (ynstring.length() > 12){
        length = 12;
    }
    
    while(not_understood){
        not_understood = false;
        for(int i = 0; i < length; i++){
            yesno = ynstring.at(i);
            if (yesno != 'Y' && yesno != 'N'){
                not_understood = true;
                break;
            }
        }
        if(not_understood){
        cout << "Sorry, I did not understand your answer. Please type either 'A' or a sequence of Ys and Ns and press 'Enter'!\n" << endl;
        cin >> ynstring;
            if(ynstring == "A")
                return 0xFFF;
        cout << "\n";
        length = (int)ynstring.length();
        if (length > 12)
            length = 12;
        }
    }
    for (int i = 0; i < length; i++){
        yesno = ynstring.at(i);
        if (yesno == 'Y')
            ynint++;
        ynint <<= 1;
    }
    ynint >>=1;
    for (int i = 0; i < 12-length; i++){
        ynint <<= 1;
    }
    
    return ynint;
}
