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

   // cout << "Staring hill climb with position " << posn->show() << "\n";

   vector<Position *> *nbrs;
   vector<Position *>::iterator itr;

   for (i = 0; i < numSteps; i++) {
      // cout << "Next step.\n";
      nbrs = posn->neighbors(); // allocates neighboring Postions with new

      foundBigger = false;
      for (itr = nbrs->begin(); itr != nbrs->end(); itr++) {
         nextValue = (*itr)->value();
         // cout << "Nbr: " << (*itr)->show() << " Value: " << nextValue << "\n";

         if (nextValue > value) {
            foundBigger = true;
            // cout << "Found bigger neighbor:\n " << (*itr)->show() << " Value: " << nextValue << "\n";
            value = nextValue;
            posn = *itr;
         }
      }
      
      // free the computed neighbors
      for (itr = nbrs->begin(); itr != nbrs->end(); itr++)
         if (*itr != posn)
            delete *itr;

      delete nbrs;

      if (!foundBigger) {
         //cout << "No larger neighbors found. Terminating after " << i << " steps.\n";
         break;
      }
   }

   return posn;
}
