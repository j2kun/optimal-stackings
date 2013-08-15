#include <stdio.h>
#include "arrays.h"
#include "poker.h"

using namespace std;

void init_deck(int *deck) {
   int i, j, n = 0, suit = 0x8000;

   for (i = 0; i < 4; i++, suit >>= 1)
      for (j = 0; j < 13; j++, n++)
         deck[n] = primes[j] | (j << 8) | suit | (1 << (16+j));
}

void print_hand(int *hand, int n) {
    int i;
    char suit;
    static string rank = "23456789TJQKA";

    for (i = 0; i < n; i++) {
        if (*hand & CLUB) suit = 'c';
        else if (*hand & DIAMOND) suit = 'd';
        else if (*hand & HEART) suit = 'h';
        else suit = 's';

        printf("%c%c ", rank[RANK(*hand)], suit);
        hand++;
    }
}

int hand_rank(short val) {
    if (val > 6185) return(HIGH_CARD);        // 1277 high card
    if (val > 3325) return(ONE_PAIR);         // 2860 one pair
    if (val > 2467) return(TWO_PAIR);         //  858 two pair
    if (val > 1609) return(THREE_OF_A_KIND);  //  858 three-kind
    if (val > 1599) return(STRAIGHT);         //   10 straights
    if (val > 322)  return(FLUSH);            // 1277 flushes
    if (val > 166)  return(FULL_HOUSE);       //  156 full house
    if (val > 10)   return(FOUR_OF_A_KIND);   //  156 four-kind
    return(STRAIGHT_FLUSH);                   //   10 straight-flushes
}

// This is a embarassingly slow method of determining the
// best five-card hand possible out of seven cards.
// But it will suffice.
short eval_7hand(int *hand) {
    int i, q, best = 9999;

	for (i = 0; i < 21; i++) {
		q = eval_5hand_fast(hand[perm7[i][0]], hand[perm7[i][1]], hand[perm7[i][2]], 
                          hand[perm7[i][3]], hand[perm7[i][4]]);

		if (q < best)
			best = q;
	}

	return(best);
}

// perfect hash
unsigned find_fast(unsigned u) {
    unsigned a, b, r;
    u += 0xe91aaa35;
    u ^= u >> 16;
    u += u << 8;
    u ^= u >> 4;
    b  = (u >> 8) & 0x1ff;
    a  = (u + (u << 2)) >> 19;
    r  = a ^ hash_adjust[b];
    return r;
}

int eval_5hand_fast(int c1, int c2, int c3, int c4, int c5) {
    int q = (c1 | c2 | c3 | c4 | c5) >> 16;
    short s;
    if (c1 & c2 & c3 & c4 & c5 & 0xf000) return flushes[q]; // check for flushes and straight flushes
    if ((s = unique5[q]))                return s;          // check for straights and high card hands
    return hash_values[find_fast((c1 & 0xff) * (c2 & 0xff) * (c3 & 0xff) * (c4 & 0xff) * (c5 & 0xff))];
}
