// Name: Carlo Flores
// Program 3 - Distance
// get 2 values
//calc distance to origin & print
//repeat
//no while true statements - gladbach
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

/*int main() {
  //define values
  int x1, y1, x2, y2;
  float dist1, dist2;
  char userChoice = 'Y';
  char userChoice2;

  while(userChoice == 'Y' || userChoice == 'y'){
    cout << "Do you want to enter your own numbers? Y or N";
    cin >> userChoice2;
    userChoice2 = toupper(userChoice2);
    
    if (userChoice2 == 'Y') {
          // if user wants to put in their own numbahs
          cout << "Input your numbers like this: 'x1 y1' or '_ _'. With your"
                  "coordinates where the underscores are with a space inbetween\n";
          cout << "Please enter x & y coordinates (int) for point1: ";
          cin >> x1 >> y1;
          cout << "Please enter x & y coordinates (int) for point2: ";
          cin >> x2 >> y2;
        } else {
          // generate random coordinates for points
          x1 = rand() % 21 - 10;
          y1 = rand() % 21 - 10;
          x2 = rand() % 21 - 10;
          y2 = rand() % 21 - 10;
        }

        // distance formula
        dist1 = pow(pow(x1, 2) + pow(y1, 2), .5);
        dist2 = pow(pow(x2, 2) + pow(y2, 2), .5);
        cout << "Distance to origin for point 1: (" << x1 << "," << y1
             << ") is: " << dist1 << endl;
        cout << "Distance to origin for point 2: (" << x2 << "," << y2
             << ") is: " << dist2 << endl;

        // how to distinct which point is closer
        if (dist1 < dist2) {
          cout << "Point 1 is closer to the origin" << endl;
        } else if (dist2 < dist1) {
          cout << "Point 2 is closer to the origin" << endl;
        } else {
          cout << "Both points are equal distance from the origin" << endl;
        }

        // if the user wants to go again
        cout << "Go again? Y or N: ";
        cin >> userChoice;
        userChoice = toupper(userChoice);
      }
    }*/
