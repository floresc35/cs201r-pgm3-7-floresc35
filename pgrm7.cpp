// CS201R-PGM7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>
#include "Functions.h"
#include "Media.h"

using namespace std;

int main() {

    ifstream inList, inCommands;
    ofstream outFile, outErr;

    try {
        inList.exceptions(ifstream::failbit);
        inList.open("mediaList.txt");
        inCommands.exceptions(ifstream::failbit);
        inCommands.open("mediaCommands.txt");
        outFile.exceptions(ofstream::failbit);
        outFile.open("mediaReport.txt");
        outErr.exceptions(ofstream::failbit);
        outErr.open("mediaError.txt");
    }
    catch (ifstream::failure& fail) {
        cout << "Could not open input file" << endl;
        cout << fail.what() << endl;
        cout << fail.code() << endl;
        cout << "This failed bruh" << endl;
        exit(1);
    }

    vector <Media*> myLib;

    // read data into myLib
    readMediaList(inList, outErr, myLib);
    inList.close();


    // prepare to read the commands
    string commandRecord;
    while (getline(inCommands, commandRecord)) {
        char choice = commandRecord[0];
        string params = commandRecord.substr(2);
        
        if (choice == 'Q') {
            cout << "Thank You for Using Media Everywhere" << endl;
            break;
        }
        else if (choice == 'A' || choice == 'M' || choice == 'B' || choice == 'S') {
            if (params.empty()) {
                printReport(commandRecord, outFile, outErr, myLib);
            } else if (isdigit(params[0])) {
                printReportRating(commandRecord, outFile, outErr, myLib);
            } else {
                printReportGenre(commandRecord, outFile, outErr, myLib);
            }
        } else if (choice == 'T') {
            printTotals(outFile, myLib);
        } else if (choice == 'N') {
            // add new content
            Media* newMedia = parseMedia(params, outErr);
            if (newMedia) {
                myLib.push_back(newMedia);
            }
        } else if (choice == 'L') {
            listStars(params, myLib);
        } else if (choice == 'F') {
            findMovies(params, myLib);
        } else if (choice == 'K') {
            findMediaByName(params, myLib);
        } else {
            outErr << "ERROR: Invalid command: " << commandRecord << endl;
        }
    }

    inCommands.close();
    outFile.close();
    outErr.close();

    //clean up memory
    for (auto media : myLib) {
        delete media;
    }
    return 0;
} 