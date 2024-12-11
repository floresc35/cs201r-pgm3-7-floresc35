// CS201R-PGM7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Functions.h"
#include "Media.h"

using namespace std;

int main() {
    ifstream inCommands;
    ofstream outFile, outErr;
    vector <Media*> myLib;

    try {
        inCommands.exceptions(ifstream::failbit);
        outFile.exceptions(ofstream::failbit);
        outErr.exceptions(ofstream::failbit);

        outFile.open("mediaReport.txt");
        outErr.open("mediaError.txt");
        
        // read data into myLib
        readMediaData("mediaList.txt", myLib, outErr);

        inCommands.open("mediaCommands.txt");
        executeCommands(inCommands, myLib, outFile, outErr);

        inCommands.close();
        outFile.close();
        outErr.close();

        //clean up memory
        for (Media* item : myLib) {
            delete item;
        }
        myLib.clear();

    } catch (ifstream::failure& e) {
        cout << "Error opening/reading file: " << e.what() << endl;
        cout << e.code() << endl;
        cout << "This failed bruh" << endl;
        return 1;
    }

    return 0;
    
} 