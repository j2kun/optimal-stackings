#include <stdio.h>
#include <iostream>
#include "poker.h"

using namespace std;

// performs at 14.438 million hands per second with -O2
// performs at 15.288 million hands per second with -O3

int main(void) {
   int deck[52], freq[10];
   int a, b, c, d, e, i, j;
   int count = 0;

   init_deck(deck);

   cout << "deck is: {\n";
   for (i = 0; i < 52; i++) {
      cout << deck[i] << ", ";
   }
   cout << "};\n";

   for (i = 0; i < 10; i++)
      freq[i] = 0;

   // loop over every possible five-card hand
   for(a = 0; a < 48; a++) {
      for(b = a + 1; b < 49; b++) {
         for(c = b + 1; c < 50; c++) {
            for(d = c + 1; d < 51; d++) {
               for(e = d + 1; e < 52; e++) {
                  i = eval_5hand_fast(deck[a], deck[b], deck[c], deck[d], deck[e]);
                  j = hand_rank(i);
                  freq[j]++;
                  count++;
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
