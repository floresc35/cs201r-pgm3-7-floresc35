#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Media.h"
using namespace std;

bool isDigits(string str1);

Media* parseMedia(const string& line, ostream& outErr);

// function to read media data from a file
void readMediaData(const string& filename, vector<Media*>& mediaList, ostream& errorFile);

// function to validate fields
bool validateMedia(const char type, const string& title, const string& keyName, int rating, const string& genre,
                   int length, int yearReleased, int additionalField, ostream& errorFile);

void printReport(const string& currRecord, ostream& outFile, ostream& errFile , const vector<Media*>& mediaLib);
//pre:  
//post: 
void printReportGenre(const string& currRecord, ostream& outFile, ostream& errFile, const vector<Media*>& mediaLib);
//pre:  
//post:
void printReportRating(const string& currRecord, ostream& outFile, ostream& errFile, const vector<Media*>& mediaLib);
//pre:  
//post:
void printTotals(ostream& outFile, const vector<Media*>& mediaLib);
//pre:  
//post:
void addContent(const string& currRecord, ostream& outFile, ostream& errFile, vector<Media*>& mediaLib);

void listStars(const std::string& searchTerm, const std::vector<Media*>& mediaList);
void findMovies(const std::string& searchTerm, const std::vector<Media*>& mediaList);
void findMediaByName(const std::string& searchTerm, const std::vector<Media*>& mediaList);

