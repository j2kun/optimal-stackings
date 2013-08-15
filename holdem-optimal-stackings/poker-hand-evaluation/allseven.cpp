#include <stdio.h>
#include <iostream>
#include "poker.h"

using namespace std;

// performs at 14.438 million hands per second with -O2
// performs at 15.288 million hands per second with -O3

int main(void) {
   int deck[52], freq[10], hand[7];
   int a, b, c, d, e, f, g, i, j;
   int count = 0;

   init_deck(deck);

   cout << "deck is: {\n";
   for (i = 0; i < 52; i++) {
      cout << deck[i] << ", ";
   }
   cout << "};\n";

   for (i = 0; i < 10; i++)
      freq[i] = 0;

   // loop over every possible seven-card hand
   for(a = 0; a < 46; a++) {
      hand[0] = deck[a];
      for(b = a + 1; b < 47; b++) {
         hand[1] = deck[b]; 
         for(c = b + 1; c < 48; c++) {
            hand[2] = deck[c];
            for(d = c + 1; d < 49; d++) {
               hand[3] = deck[d];
               for(e = d + 1; e < 50; e++) {
                  hand[4] = deck[e]; 
                  for(f = e + 1; f < 51; f++) {
                     hand[5] = deck[f];
                     for(g = f + 1; g < 52; g++) {
                        hand[6] = deck[g]; 

                        i = eval_7hand(hand);
                        j = hand_rank(i);
                        freq[j]++;
                        count++;
                     }
                  }
               }
            }
         }
      }
   }

   for(i = 1; i <= 9; i++)
      printf("%15s: %8d\n", value_str[i].c_str(), freq[i]);

   printf("%d evaluations performed\n", count);
   return 0;
}
