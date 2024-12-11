#include "Media.h"

// base media class implementation
Media::Media(string t, string n, int r, string g, int l, int y)
    : title(t), name(n), rating(r), genre(g), length(l), yearReleased(y) {}

char Media::getType() const { return type; }
string Media::getTitle() const { return title; }
string Media::getName() const { return name; }
int Media::getRating() const { return rating; }
string Media::getGenre() const { return genre; }
int Media::getLength() const { return length; }
int Media::getYear() const { return yearReleased; }

void Media::print(ostream& os) const {
    os << setw(35) << left << title
       << setw(10) << yearReleased
       << setw(10) << rating
       << setw(20) << genre;
}

// movie class implementation
Movie::Movie(string t, string n, int r, string g, int l, int y, vector<string> s)
    : Media(t, n, r, g, l, y), stars(s) {
    type = 'M';
}

void Movie::print(ostream& os) const {
    Media::print(os);
    os << "Stars: ";
    for (size_t i = 0; i < stars.size(); ++i) {
        os << stars[i];
        if (i < stars.size() - 1) os << ", ";
    }
    os << "\n";
}

bool Movie::hasActor(const string& actorName) const {
    return find(stars.begin(), stars.end(), actorName) != stars.end();
}

// book class implementation
Book::Book(string t, string n, int r, string g, int l, int y, int w)
    : Media(t, n, r, g, l, y), weeksNYT(w) {
    type = 'B';
}

void Book::print(ostream& os) const {
    Media::print(os);
    os << "Weeks NYT: " << weeksNYT << "\n";
}

// song class implementation
Song::Song(string t, string n, int r, string g, int l, int y, bool t40)
    : Media(t, n, r, g, l, y), top40(t40) {
    type = 'S';
}

void Song::print(ostream& os) const {
    Media::print(os);
    os << (top40 ? "Top 40 Hit" : "---") << "\n";
}