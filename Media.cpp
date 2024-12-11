#include <iostream>
#include <sstream> 
#include <iomanip>
#include "Media.h"
using namespace std;
// movie::print
void Movie::print(ostream& os) const {
    os << setw(15) << left << title << " "
           << setw(10) << yearReleased << " "
           << setw(8) << rating << " "
           << setw(20) << genre << " "
           << "Stars: ";
        for (const auto& star : stars) {
            os << star << " ";
        }
        os << "\n";
    }

// book::print
void Book::print(ostream& os) const {
    os << setw(15) << left << " " << setw(30) << title
           << setw(10) << yearReleased << " "
           << setw(8) << rating << " "
           << setw(20) << genre << " "
           << "Weeks NYT: " << weeksNYT << endl;
    }

// song::print
void Song::print(ostream& os) const {
    os << setw(15) << left << " " << setw(30) << title
           << setw(10) << yearReleased << " "
           << setw(8) << rating << " "
           << setw(20) << genre << " "
           << (top40 ? "Top 40 Hit" : "---") << endl;
    }






