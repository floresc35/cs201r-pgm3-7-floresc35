//Media.h
#pragma once
#include <string>
#include <iostream>
#include <vector>
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
    Media(char type, const string& title, const string& name, int rating, const string& genre, int length, int yearReleased) : type(type), title(title), name(name), rating(rating), genre(genre), length(length), yearReleased(yearReleased) {}

  virtual void print(ostream& out) const = 0;
  virtual ~Media() = default;
    
  //get
  char getType() const { return type; }
  string getTitle() const { return title; }
  string getName() const { return name; }
  int getRating() const { return rating; }
  string getGenre() const { return genre; }
  int getLength() const { return length; }
  int getYearReleased() const { return yearReleased; }

  //set
  void setType(char t) { type = t; }
  void setTitle(const string& t) { title = t; }
  void setName(const string& n) { name = n; }
  void setRating(int r) { rating = r; }
  void setGenre(const string& g) { genre = g; }
  void setLength(int l) { length = l; }
  void setYearReleased (int y) { yearReleased = y; }
};

class Movie : public Media {
private:
    vector<string> stars;
public:
    //constructor with parameters
    Movie(char type, string title, string name, int rating, string genre, int length, int yearReleased, const vector<string>& stars) : Media(type, title, name, rating, genre, length, yearReleased), stars(stars) {}

// get
vector<string> getStars() const { return stars; }
// set
void setStars(const vector<string>& s) { stars = s; }
// override
void print(ostream& os) const override;
};

class Book : public Media {
private:
    int weeksNYT; // weeks on the NYT bestseller list

public:
    // constructor
    Book(char type, const string& title, const string& keyName, int rating, const string& genre, int length, int yearReleased, int weeksNYT)
        : Media(type, title, keyName, rating, genre, length, yearReleased), weeksNYT(weeksNYT) {}

    // get
    int getWeeks() const { return weeksNYT; }

    // set
    void setWeeks(int weeks) { weeksNYT = weeks; }

    // override print
    void print(ostream& os) const override;
};


class Song : public Media {
private:
    bool top40; // whether the song is in the top 40 chart

public:
    // constructor
    Song(char type, const string& title, const string& keyName, int rating, const string& genre, int length, int yearReleased, bool top40)
        : Media(type, title, keyName, rating, genre, length, yearReleased), top40(top40) {}

    //get
    bool getTop40() const { return top40; }

    // set
    void setTop40(bool t) { top40 = t; }

    // override print
    void print(ostream& os) const override;
};
