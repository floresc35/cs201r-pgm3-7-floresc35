#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "Functions.h"
#include "Media.h"

using namespace std;

bool isDigits(string str1) {
    return (str1.find_first_not_of("1234567890") == string::npos);
}
//used llm to create parsing function
Media* parseMedia(const string& line, ostream& outErr) {
    istringstream stream(line);
    string typeStr, title, name, ratingStr, genre, lengthStr, yearReleasedStr, extraField;
    int rating, length, yearReleased;

    // Debugging: Print the line being processed
    //cout << "Parsing line: " << line << endl; // This will show us the line being processed

    // extract required fields
    if (!(getline(stream, typeStr, ',') &&
          getline(stream, title, ',') &&
          getline(stream, name, ',') &&
          getline(stream, ratingStr, ',') &&
          getline(stream, genre, ',') &&
          getline(stream, lengthStr, ',') &&
          getline(stream, yearReleasedStr, ','))) {
        outErr << "ERROR: Incomplete or malformed record: " << line << endl;
        return nullptr;
    }

    // attempt to convert string fields to integers
    try {
        rating = stoi(ratingStr);
        length = stoi(lengthStr);
        yearReleased = stoi(yearReleasedStr);
    } catch (const invalid_argument&) {
        outErr << "ERROR:" << line << "\nPrevious record has an invalid stoi argument error" << endl;
        return nullptr;
    }

    // validate numeric ranges
    if (rating < 1 || rating > 10) {
        outErr << "ERROR:" << line << "\nThere was an invalid value entered for rating" << endl;
        return nullptr;
    }
    if (length < 0) { 
        outErr << "ERROR:" << line << "\nThere was an invalid value entered for length" << endl;
        return nullptr;
    }
    if (yearReleased < 1920 || yearReleased > 2022) {
        outErr << "ERROR:" << line << "\nThere was an invalid value entered for rating" << endl;
        return nullptr;
    }
    // determine type and create the appropriate derived object
    char type = typeStr[0];
    if (type == 'M') {
        vector<string> stars;
        while (getline(stream, extraField, ',')) {
            stars.push_back(extraField);
        }
        return new Movie(type, title, name, rating, genre, length, yearReleased, stars);
    } else if (type == 'B') {
        int weeksNYT;
        if (!(stream >> weeksNYT)) {
            outErr << "ERROR:" << line << "\nMissing weeksNYT field in record " << endl;
            return nullptr;
        }
        return new Book(type, title, name, rating, genre, length, yearReleased, weeksNYT);
    } else if (type == 'S') {
        int top40;
        if (!(stream >> top40) || (top40 != 0 && top40 != 1)) {
            outErr << "ERROR:" << line << "\nInvalid top40 field in recorn" << endl;
            return nullptr;
        }
        return new Song(type, title, name, rating, genre, length, yearReleased, top40 == 1);
    } else {
        outErr << "ERROR:" << line << "\nInvalid type field in record" << endl;
        return nullptr;
    }
}

//readDataFile(inFile,errFile,vector<Media*>);
//readCommandFile(currRecord, outFile, errFile, vector<Media*>);
// print all media or filtered by command (based on type)
void printReport(const string& currRecord, ostream& outFile, ostream& outErr, const vector<Media*>& mediaLib) {
    char type = currRecord[0];

    if (type == 'M') outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n             YOUR MOVIE LIST\n\n";
    else if (type == 'B') outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n             YOUR BOOK LIST\n\n";
    else if (type == 'S') outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n             YOUR SONG LIST\n\n";
    else outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n             YOUR MEDIA LIST\n\n";

    outFile << "#           TITLE                           YEAR    RATING      GENRE						OTHER FIELDS\n";

    int count = 1;
    for (const auto& media : mediaLib) {
        if (type == 'A' || media->getType() == type) {
        outFile << setw(3) << count++ << "  ";
            media->print(outFile);
        }
    }
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}


// print media by specified genre
void printReportGenre(const string& command, ostream& outFile, ostream& outErr, const vector<Media*>& mediaLib) {
    char type = command[0];
    string genre = command.substr(2);  // Get the genre after the comma
    bool found = false;

    for (const auto& media : mediaLib) {
        if ((type == 'A' || media->getType() == type) && media->getGenre() == genre) {
            media->print(outFile);  // Log genre-based media to file
            found = true;
        }
    }

    if (!found) {
        outFile << "No items found with genre: " << genre << endl;
    }
}


// print media by rating
void printReportRating(const string& currRecord, ostream& outFile, ostream& outErr, const vector<Media*>& mediaLib) {
    char type = currRecord[0];
    int rating = stoi(currRecord.substr(2));
    bool found = false;
    for (const auto& media : mediaLib) {
        if ((type == 'A' || media->getType() == type) && media->getRating() >= rating) {
            media->print(outFile);
            found = true;
        }
    }
    if (!found) outFile << "No items found with rating >= " << rating << endl;
}

// print totals of each media type
void printTotals(ostream& outFile, const vector<Media*>& mediaLib) {
    int movies = 0, books = 0, songs = 0;
    for (const auto& media : mediaLib) {
        switch (media->getType()) {
            case 'M': ++movies; break;
            case 'B': ++books; break;
            case 'S': ++songs; break;
        }
    }
    outFile << "Movies: " << movies << ", Books: " << books << ", Songs: " << songs << endl;
}
// add new media based on command input
void addContent(const string& currRecord, ostream& outFile, ostream& outErr, vector<Media*>& mediaLib) {
    istringstream stream(currRecord.substr(2));
    char type;
    string title, name, genre, starsField;
    int rating, length, yearReleased, additionalField;

    stream >> type >> title >> name >> rating >> genre >> length >> yearReleased;

    if (type == 'M') {
        getline(stream, starsField);
        vector<string> stars;
        istringstream starStream(starsField);
        string star;
        while (getline(starStream, star, ',')) {
            stars.push_back(star);
        }
        mediaLib.push_back(new Movie(type, title, name, rating, genre, length, yearReleased, stars));
    } else if (type == 'B') {
        stream >> additionalField;
        mediaLib.push_back(new Book(type, title, name, rating, genre, length, yearReleased, additionalField));
    } else if (type == 'S') {
        stream >> additionalField;
        mediaLib.push_back(new Song(type, title, name, rating, genre, length, yearReleased, additionalField == 1));
    } else {
        outErr << "ERROR:" << type << "Invalid type for new content" << "\n";
    }
}


void readMediaData(const string& filename, vector<Media*>& mediaList, ostream& errorFile) {
    ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("Unable to open media list file.");
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        char type;
        string title, keyName, genre, starsField;
        int rating, length, yearReleased, additionalField = 0; // additionalField: weeksNYT or top40

        try {
            // parse data
            ss >> type >> title >> keyName >> rating >> genre >> length >> yearReleased;

            // additional field handling for each type
            if (type == 'M') { // Movie
                getline(ss, starsField); // comma-separated list of stars
            } else if (type == 'B') { // book
                ss >> additionalField; // weeksNYT
            } else if (type == 'S') { // song
                ss >> additionalField; // top40 (1 or 0)
            }

            // validate data
            if (!validateMedia(type, title, keyName, rating, genre, length, yearReleased, additionalField, errorFile)) {
                errorFile << "ERROR: Record skipped due to validation failure: " << line << endl;
                continue; // skip invalid entries
            }

            // create object dynamically and add to the list
            if (type == 'M') {
                vector<string> stars;
                stringstream starsStream(starsField);
                string star;
                while (getline(starsStream, star, ',')) {
                    stars.push_back(star);
                }
                mediaList.push_back(new Movie(type, title, keyName, rating, genre, length, yearReleased, stars));
            } else if (type == 'B') {
                mediaList.push_back(new Book(type, title, keyName, rating, genre, length, yearReleased, additionalField));
            } else if (type == 'S') {
                mediaList.push_back(new Song(type, title, keyName, rating, genre, length, yearReleased, additionalField == 1));
            }
        } catch (const exception& e) {
            errorFile << "Error parsing line: " << line << " - " << e.what() << endl;
        }
    }

    inputFile.close();
}

bool validateMedia(const char type, const string& title, const string& keyName, int rating, const string& genre,
                   int length, int yearReleased, int additionalField, ostream& errorFile) {
    bool isValid = true;
    // "Unable to process command. There was an invalid value entered for" << _____ << ": " << _____ << endl;
    // if statement for if rating isn't found for a movie, book, or song, then it is invalid
    if (type != 'M' && type != 'B' && type != 'S') {
        errorFile << "Invalid type: " << type << endl;
        isValid = false;
    }
    if (rating <= 0 || rating > 10) {
        errorFile << "The rating provided: " << rating << " is invalid" << endl;
        isValid = false;
    }
    if (yearReleased < 1920 || yearReleased > 2024) {
        errorFile << "Invalid year released for " << title << ": " << yearReleased << endl;
        isValid = false;
    }
    if (type == 'B' && additionalField <= 0) {
        errorFile << "Invalid weeksNYT for " << title << ": " << additionalField << endl;
        isValid = false;
    }
    if (type == 'S' && (additionalField != 0 && additionalField != 1)) {
        errorFile << "Invalid top40 value for " << title << ": " << additionalField << endl;
        isValid = false;
    }

    return isValid;
}

void listStars(const std::string& searchTerm, const vector<Media*>& mediaList) {
    for (const auto& media : mediaList) {
        if (media->getType() == 'M') {
            Movie* movie = static_cast<Movie*>(media);
            for (const auto& star : movie->getStars()) {
                if (star.find(searchTerm) != string::npos) {
                    std::cout << "Star found: " << star << " in movie: " << movie->getTitle() << "\n";
                }
            }
        }
    }
}

void findMovies(const std::string& searchTerm, const std::vector<Media*>& mediaList) {
    for (const auto& media : mediaList) {
        if (media->getType() == 'M') {
            Movie* movie = static_cast<Movie*>(media);
            if (movie->getTitle().find(searchTerm) != std::string::npos) {
                std::cout << "Movie found: " << movie->getTitle() << endl;
            }
            for (const auto& star : movie->getStars()) {
                if (star.find(searchTerm) != string::npos) {
                    std::cout << "Star match: " << star << " in movie: " << movie->getTitle() << "\n";
                }
            }
        }
    }
}

void findMediaByName(const std::string& searchTerm, const std::vector<Media*>& mediaList) {
    for (const auto& media : mediaList) {
        if (media->getName() == searchTerm) {
            std::cout << "Media found: ";
            media->print(std::cout);
        }
    }
}

void executeCommands(ifstream& inFile, vector<Media*>& mediaList, 
                    ofstream& outFile, ofstream& outErr) {
    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string command;
        getline(ss, command, ',');

        if (command == "Q") {
            cout << "Thank You for Using Media Everywhere" << endl;
            break;
        }
        // single letter (A, M, B, S, T)
        if (command.length() == 1) {
            switch(command[0]) {
                case 'A':
                case 'M':
                case 'B':
                case 'S':
                    printReport(line, outFile, outErr, mediaList);
                    break;
                case 'T':
                    printTotals(outFile, mediaList);
                    break;
            }
        }
        // commands with parameters
        else if (command.length() > 1) {
            char cmdType = command[0];
            switch(cmdType) {
                case 'N':
                    addContent(line, outFile, outErr, mediaList);
                    break;
                case 'L':
                    listStars(line.substr(2), mediaList);
                    break;
                case 'F':
                    findMovies(line.substr(2), mediaList);
                    break;
                case 'K':
                    findMediaByName(line.substr(2), mediaList);
                    break;
                case 'A':
                case 'M':
                case 'B':
                case 'S':
                    if (isDigits(line.substr(2)))
                        printReportRating(line, outFile, outErr, mediaList);
                    else
                        printReportGenre(line, outFile, outErr, mediaList);
                    break;
            }
        }
    }
}