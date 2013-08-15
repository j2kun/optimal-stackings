#include "hillclimb.h"
#include <iostream>

using std::cout;
using std::vector;

Position::Position() {}
Position::~Position() {}

/* Check all neighbors, and use the largest for the next step. 
 * Terminate early if no neighbors are larger. 
 * The caller retains ownership of the posn pointer. */
Position* hillclimb(Position* posn, const int numSteps) {
   double value = posn->value(), nextValue;
   int i;
   bool foundBigger;

   vector<Position *> *nbrs;
   vector<Position *>::iterator itr;

   for (i = 0; i < numSteps; i++) {
      nbrs = posn->neighbors(); // allocates neighboring Positions with new

      foundBigger = false;
      for (itr = nbrs->begin(); itr != nbrs->end(); itr++) {
         nextValue = (*itr)->value();

         if (nextValue > value) {
            foundBigger = true;
            value = nextValue;
            posn = *itr;
         }
      }
      
      // free the computed neighbors
      for (itr = nbrs->begin(); itr != nbrs->end(); itr++)
         if (*itr != posn)
            delete *itr;

      delete nbrs;

      if (!foundBigger)
         break;
   }

   return posn;
}

