// Media.h
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

class Media {
protected:
    char type;
    string title;
    string name;
    int rating;
    string genre;
    int length;
    int yearReleased;

public:
    Media(string t, string n, int r, string g, int l, int y);
    virtual ~Media() = default;
    
    char getType() const;
    string getTitle() const;
    string getName() const;
    int getRating() const;
    string getGenre() const;
    int getLength() const;
    int getYear() const;
    
    virtual void print(ostream& os) const;
};

class Movie : public Media {
private:
    vector<string> stars;

public:
    Movie(string t, string n, int r, string g, int l, int y, vector<string> s);
    void print(ostream& os) const override;
    bool hasActor(const string& actorName) const;
    const vector<string>& getStars() const { return stars; }
};

class Book : public Media {
private:
    int weeksNYT;

public:
    Book(string t, string n, int r, string g, int l, int y, int w);
    void print(ostream& os) const override;
    int getWeeksNYT() const { return weeksNYT; }
};

class Song : public Media {
private:
    bool top40;

public:
    Song(string t, string n, int r, string g, int l, int y, bool t40);
    void print(ostream& os) const override;
    bool getTop40() const { return top40; }
};