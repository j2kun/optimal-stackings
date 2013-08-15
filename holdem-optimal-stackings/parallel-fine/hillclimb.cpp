#include "hillclimb.h"
#include <iostream>
#include <omp.h>

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
   unsigned int j, posnIndex = (unsigned int)-1;
   bool foundBigger;

   vector<Position *> *nbrs;

   for (i = 0; i < numSteps; i++) {
      nbrs = posn->neighbors(); // allocates neighboring Postions with new

      foundBigger = false;
      #pragma omp parallel private(nextValue) shared(foundBigger, nbrs, value, posn, posnIndex)
      {
         #pragma omp for schedule(static) nowait
         for (j = 0; j < nbrs->size(); j++) {
            nextValue = (*nbrs)[j]->value();

            #pragma omp critical
            {
               if (nextValue > value) {
                  foundBigger = true;
                  value = nextValue;
                  posn = (*nbrs)[j];
                  posnIndex = j;
               }
            }
         }
         
         // free the computed neighbors
         #pragma omp for schedule(static) nowait
         for (j = 0; j < nbrs->size(); j++)
            if (j != posnIndex)
               delete (*nbrs)[j];
      }

      delete nbrs;

      if (!foundBigger)
         break;
   }

   return posn;
}

