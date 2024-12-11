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
    stringstream stream(line);
    string typeStr, title, name, ratingStr, genre, lengthStr, yearStr;

    // Debugging: Print the line being processed
    //cout << "Parsing line: " << line << endl; // This will show us the line being processed

    

    // extract required fields
    if (!getline(stream, typeStr, ',') ||
          !getline(stream, title, ',') ||
          !getline(stream, name, ',') ||
          !getline(stream, ratingStr, ',') ||
          !getline(stream, genre, ',') ||
          !getline(stream, lengthStr, ',') ||
          !getline(stream, yearStr, ',')) {
        outErr << "ERROR: " << line << "\nPrevious record has an invalid stoi argument error\n\n";
        return nullptr;
    }

    try {
        int rating = stoi(ratingStr);
        int length = stoi(lengthStr);
        int year = stoi(yearStr);
        
        if (!validateMedia(typeStr[0], title, name, rating, genre, length, year, 0, outErr)) {
            return nullptr;
        }

        if (typeStr[0] == 'M') {
            vector<string> stars;
            string star;
            while (getline(stream, star, ',')) {
                if (!star.empty()) stars.push_back(star);
            }
            return new Movie(title, name, rating, genre, length, year, stars);
        }
        else if (typeStr[0] == 'B') {
            string weeks;
            getline(stream, weeks, ',');
            try {
                int weeksNYT = weeks.empty() ? 0 : stoi(weeks);
                return new Book(title, name, rating, genre, length, year, weeksNYT);
            }
            catch (...) {
                outErr << "Previous record has an invalid stoi argument error\n\n";
                return nullptr;
            }
        }
        else if (typeStr[0] == 'S') {
    string top40Str;
    getline(stream, top40Str);
    
    // Remove trailing commas and whitespace
    while (!top40Str.empty() && (top40Str.back() == ',' || isspace(top40Str.back()))) {
        top40Str.pop_back();
    }
    
    // Default to not top40 (0) if field is empty
    bool isTop40 = false;
    if (!top40Str.empty()) {
        if (top40Str != "0" && top40Str != "1") {
            outErr << "Previous record has an error in boolean value\n\n";
            return nullptr;
        }
        isTop40 = (top40Str == "1");
    }
    
    return new Song(title, name, rating, genre, length, year, isTop40);
}
    }
    catch (...) {
        outErr << "ERROR: " << line << "\n"
               << "Previous record has an invalid stoi argument error\n\n";
        return nullptr;
    }
    return nullptr;
}

// print all media or filtered by command (based on type)
void printReport(const string& currRecord, ostream& outFile, ostream& outErr, 
                const vector<Media*>& mediaLib) {
    char type = currRecord[0];
    outFile << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    
    string title;
    switch(type) {
        case 'M': outFile << "             YOUR MOVIE LIST\n\n"; break;
        case 'B': outFile << "             YOUR BOOK LIST\n\n"; break;
        case 'S': outFile << "             YOUR SONG LIST\n\n"; break;
        case 'A': outFile << "             YOUR MEDIA LIST\n\n"; break;
    }
    outFile << setw(35) << title << "\n\n";
    outFile << "#  TITLE                           YEAR    RATING      GENRE                    OTHER FIELDS\n";

    int count = 1;
    for (const auto& media : mediaLib) {
        if (type == 'A' || (type != 'A' && media->getType() == type)) {
            outFile << setw(3) << count++ << "  ";
            media->print(outFile);
        }
    }
    
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}


// print media by specified genre
void printReportGenre(const string& currRecord, ostream& outFile, ostream& outErr, 
                     const vector<Media*>& mediaLib) {
    char type = currRecord[0];
    string genre = currRecord.substr(2);
    
    outFile << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    string mediaType;
    switch(type) {
        case 'M': mediaType = "MOVIES"; break;
        case 'B': mediaType = "BOOKS"; break;
        case 'S': mediaType = "SONGS"; break;
        default: mediaType = "MEDIA";
    }
    outFile << "             " << mediaType << " WITH GENRE >= " << genre << "\n\n";
    outFile << "#  TITLE                           YEAR    RATING      GENRE                    OTHER FIELDS\n";

    int count = 1;
    bool found = false;
    for (const auto& media : mediaLib) {
        if ((type == 'A' || media->getType() == type) && media->getGenre() == genre) {
            outFile << setw(3) << count++ << "  ";
            media->print(outFile);
            found = true;
        }
    }
    
    if (!found) {
        if (genre == "none") {
            outErr << "Unable to process command. The genre: none is not found in your list\n\n";
        } else {
            outFile << "No items found with genre: " << genre << endl;
        }
    }
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}


// print media by rating
void printReportRating(const string& currRecord, ostream& outFile, ostream& outErr, 
                      const vector<Media*>& mediaLib) {
    char type = currRecord[0];
    int rating = stoi(currRecord.substr(2));
    
    outFile << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    string mediaType;
    switch(type) {
        case 'M': mediaType = "MOVIES"; break;
        case 'B': mediaType = "BOOKS"; break;
        case 'S': mediaType = "SONGS"; break;
        default: mediaType = "MEDIA";
    }
    outFile << "             " << mediaType << " WITH RATING >= " << rating << "\n\n";
    outFile << "#  TITLE                           YEAR    RATING      GENRE                    OTHER FIELDS\n";

    int count = 1;
    bool found = false;
    for (const auto& media : mediaLib) {
        if ((type == 'A' || media->getType() == type) && media->getRating() >= rating) {
            outFile << setw(3) << count++ << "  ";
            media->print(outFile);
            found = true;
        }
    }
    
    if (!found) {
        outFile << "No items found with rating >= " << rating << endl;
    }
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
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
    
    outFile << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    outFile << "             MEDIA COUNTS\n\n";
    outFile << "Movies: " << movies << "\n";
    outFile << "Books:  " << books << "\n";
    outFile << "Songs:  " << songs << "\n";
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}
// add new media based on command input
void addContent(const string& currRecord, ostream& outFile, ostream& outErr, vector<Media*>& mediaLib) {
    Media* newMedia = parseMedia(currRecord.substr(2), outErr);
    if (newMedia != nullptr) {
        mediaLib.push_back(newMedia);
        outFile << (newMedia->getType() == 'M' ? "Movie" : 
                                   newMedia->getType() == 'B' ? "Book" : "Song")
                << ": " << newMedia->getTitle() << "was added to your list" << "\n";
    }
}


void readMediaData(const string& filename, vector<Media*>& mediaLib, ostream& errorFile) {
    ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("Unable to open media list file.");
    }

    string line;
    while (getline(inputFile, line)) {
        if (line.empty() || line == "Q") continue;
        
        Media* newMedia = parseMedia(line, errorFile);
        if (newMedia != nullptr) {
            mediaLib.push_back(newMedia);
        }
    }
    inputFile.close();
}

bool validateMedia(const char type, const string& title, const string& keyName, int rating, const string& genre,
                   int length, int yearReleased, int additionalField, ostream& errorFile) {
    // rating (1-10)
    if (rating < 1 || rating > 10) {
        errorFile << "ERROR: " << type << "," << title << "," << keyName << "," << rating << "," << genre << "," << length << "," << yearReleased << "," << additionalField << "\nThere was an invalid value entered for rating." << endl;
        return false;
    }

    // year (1800-2024)
    if (yearReleased < 1800 || yearReleased > 2022) {
        errorFile << "ERROR: " << type << "," << title << "," << keyName << "," << rating << "," << genre << "," << length << "," << yearReleased << "," << additionalField << "\nThere was an invalid value entered for year." << endl;
        return false;
    }

    return true;
}

void listStars(const std::string& searchTerm, const vector<Media*>& mediaLib, ofstream& outFile, ofstream& outErr) {
    outFile << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    outFile << "MOVIES WITH STAR: " << searchTerm << "\n\n";
    outFile << "#  TITLE                           YEAR    RATING      GENRE                    OTHER FIELDS\n";
    
    int count = 1;
    bool found = false;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') {
            Movie* movie = static_cast<Movie*>(media);
            if (movie->hasActor(searchTerm)) {
                outFile << setw(3) << count++ << "  ";
                movie->print(outFile);
                found = true;
                }
            }
        }
        if (!found) {
            outErr << "Unable to process command. The movie: '" << searchTerm 
                << "' is not found in your list\n\n";
        }
        outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

void findMovies(const std::string& searchTerm, const std::vector<Media*>& mediaLib,ofstream& outFile, ofstream& outErr) {
    outFile << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    outFile << "MOVIES WITH ACTOR: " << searchTerm << "\n\n";
    outFile << "#  TITLE                           YEAR    RATING      GENRE                    OTHER FIELDS\n";
    
    int count = 1;
    bool found = false;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') {
            Movie* movie = static_cast<Movie*>(media);
            if (movie->hasActor(searchTerm)) {
                outFile << setw(3) << count++ << "  ";
                movie->print(outFile);
                found = true;
            }
        }
    }

    if (!found) {
        outErr << "Unable to process command. The star: " << searchTerm 
               << " is not found in any movies in your list\n\n";
    }
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}


void findMediaByName(const std::string& searchTerm, const std::vector<Media*>& mediaLib, ofstream& outFile, ofstream& outErr) {
    outFile << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    outFile << "MEDIA BY NAME: " << searchTerm << "\n\n";
    outFile << "#  TITLE                           YEAR    RATING      GENRE                    OTHER FIELDS\n";
    
    int count = 1;
    bool found = false;
    for (const auto& media : mediaLib) {
        if (media->getName() == searchTerm) {
            outFile << setw(3) << count++ << "  ";
            media->print(outFile);
            found = true;
        }
    }
    
    if (!found) {
        outErr << "Unable to process command. The name: " << searchTerm 
               << " was not found in your list\n\n";
    }
}

void executeCommands(ifstream& inFile, vector<Media*>& mediaLib, 
                    ofstream& outFile, ofstream& outErr) {
    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        
        char command = line[0];
        
        // handle Q command
        if (command == 'Q') {
            cout << "Thank You for Using Media Everywhere" << endl;
            break;
        }
        

        // basic commands without parameters
        if (line.length() == 1) {
            switch(command) {
                case 'A':
                case 'M':
                case 'B':
                case 'S':
                    printReport(string(1, command), outFile, outErr, mediaLib);
                    break;
                case 'T':
                    printTotals(outFile, mediaLib);
                    break;
            }
            continue;
        }

        // commands with parameters
        if (line.length() > 2) {
            string params = line.substr(2);
            switch(command) {
                case 'N':
                    addContent(line, outFile, outErr, mediaLib);
                    break;
                case 'L':
                    listStars(params, mediaLib, outFile, outErr);
                    break;
                case 'F':
                    findMovies(params, mediaLib, outFile, outErr);
                    break;
                case 'K':
                    findMediaByName(params, mediaLib, outFile, outErr);
                    break;
                case 'A':
                case 'M':
                case 'B':
                case 'S':
                    if (isDigits(params)) {
                        int rating = stoi(params);
                        if (rating < 1 || rating > 10) {
                            outErr << "The rating provided: " << rating << " is invalid\n\n";
                            continue;
                        }
                        printReportRating(line, outFile, outErr, mediaLib);
                    } else if (params == "none") {
                        outErr << "Unable to process command. The genre: none is not found in your list\n\n";
                    } else {
                        printReportGenre(line, outFile, outErr, mediaLib);
                    }
                    break;
            }
        }
    }
}