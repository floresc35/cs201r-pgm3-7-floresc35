#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Media.h"
using namespace std;

//pre: input string exists and contains characters
//post: returns true if digits
bool isDigits(string str1);

//pre: input line not empty and outErr stream open
//post: returns valid media pointer
Media* parseMedia(const string& line, ostream& outErr);

//pre: file exists and readable, vector initialized for storage
//post: media objects stored properly
void readMediaData(const string& filename, vector<Media*>& mediaLib, ostream& errorFile);

//pre: all parameters have values and errorFile stream open
//post: returns validation status true/false
bool validateMedia(const char type, const string& title, const string& keyName, int rating, const string& genre,
                   int length, int yearReleased, int additionalField, ostream& errorFile);

//pre: media library contains data and output streams open
//post: prints formatted media report
void printReport(const string& currRecord, ostream& outFile, ostream& errFile, const vector<Media*>& mediaLib);

//pre: genre parameter valid and media library not empty
//post: prints genre-filtered report
void printReportGenre(const string& currRecord, ostream& outFile, ostream& errFile, const vector<Media*>& mediaLib);

//pre: rating value valid and media library contains entries
//post: prints rating-filtered report
void printReportRating(const string& currRecord, ostream& outFile, ostream& errFile, const vector<Media*>& mediaLib);

//pre: media library initialized and outFile stream open
//post: prints media type counts
void printTotals(ostream& outFile, const vector<Media*>& mediaLib);

//pre: record format valid and streams are ready
//post: adds new media entry
void addContent(const string& currRecord, ostream& outFile, ostream& errFile, vector<Media*>& mediaLib);

//pre: search term not empty and media library contains movies
//post: lists movies with star
void listStars(const string& searchTerm, const vector<Media*>& mediaLib, ofstream& outFile, ofstream& outErr);

//pre: actor name valid and movie entries exist
//post: lists matching movies
void findMovies(const string& searchTerm, const vector<Media*>& mediaLib, ofstream& outFile, ofstream& outErr);

//pre: name parameter exists and media library not empty
//post: lists media by name
void findMediaByName(const string& searchTerm, const vector<Media*>& mediaLib, ofstream& outFile, ofstream& outErr);

//pre: command file is open and streams ready for io
//post: processes all valid commands
void executeCommands(ifstream& inFile, vector<Media*>& mediaLib, ofstream& outFile, ofstream& outErr);