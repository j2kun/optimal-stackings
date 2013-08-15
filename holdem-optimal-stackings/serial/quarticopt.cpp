#include "quarticopt.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#define EPSILON 0.0001
#define NUM_TRIALS 50
#define TRIAL_LENGTH 10000

using std::cout;
using std::vector;
using std::string;
using std::stringstream;

XValue::XValue(double givenX) {
   x = givenX;
}

XValue::~XValue() {}

vector<Position *> *XValue::neighbors() {
   vector<Position *> *listOfNeighbors = new vector<Position *>;
   
   Position *left = new XValue(x - EPSILON);
   Position *right = new XValue(x + EPSILON);

   listOfNeighbors->push_back(left);
   listOfNeighbors->push_back(right);

   return listOfNeighbors;
}

double XValue::value() {
   return -5*(x-2)*(2*x-1)*(x+1)*x;
}

string XValue::show() {
   stringstream ss;
   ss << x;
   return ss.str();
}

int main(void) {
   srand(time(0));

   double actualBest = 1.5117273230597213;

   XValue *bestSoFar = new XValue(0);
   XValue *result;

   int i;
   for (i = 0; i < NUM_TRIALS; i++) {
      // random starting position btw -5 and 5
      double initX = 10 * ((double)rand() / (double)RAND_MAX) - 5; 
      
      XValue startingPosn(initX);
      result = (XValue *)hillclimb(&startingPosn, TRIAL_LENGTH);

      if (bestSoFar->value() < result->value()) {
         cout << "found new best result: " << result->x << "\n";
         delete bestSoFar;
         bestSoFar = result;
      } else {
         delete result;
      }
   }

   cout << "Final answer: " << bestSoFar->x << "\n";
   cout << "Error: " << (bestSoFar->x - actualBest) << "\n";

   delete bestSoFar;

   return 0;
}
