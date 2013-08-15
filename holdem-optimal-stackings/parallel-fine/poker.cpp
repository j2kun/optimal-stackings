#include "poker.h"
#include "arrays.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#define DEFAULT_TRIAL_LENGTH 100
#define DEFAULT_NUM_TRIALS 10
#define DEFAULT_NUM_THREADS 12

using namespace std;

Deck::Deck() {
   cards = new int[DECK_SIZE];
   memcpy(cards, INITIAL_DECK, DECK_SIZE*sizeof(int));
   shuffle();
}

Deck::Deck(const Deck& other) {
   cards = new int[DECK_SIZE];
   memcpy(cards, other.cards, DECK_SIZE*sizeof(int));
}

Deck::~Deck() {
   delete [] cards;
}

void Deck::shuffle() {
   int i, j;
   for (i = 0; i < DECK_SIZE; i++) {
      j = (rand() % (DECK_SIZE - i)) + i; // in [i, 51]
      int temp = cards[i];
      cards[i] = cards[j];
      cards[j] = temp;
   }
}

// equality up to cuts
bool Deck::operator==(const Deck& other) {
   int cutPosn, i;
   bool equalForThisCut;

   for (cutPosn = 0; cutPosn < DECK_SIZE; cutPosn++) {
      equalForThisCut = true;

      for (i = 0; i < DECK_SIZE; i++) {
         if (cards[(cutPosn + i) % DECK_SIZE] != other.cards[i]) {
            equalForThisCut = false;
            break;
         }
      }

      if (equalForThisCut)
         return true;
   }

   return false;
}

string cardstr(int card) {
   static const string rank = "23456789TJQKA";
   stringstream ss;

   ss << rank[RANK(card)];
   
   if (card & CLUB) ss << "c";
   else if (card & DIAMOND) ss << "d";
   else if (card & HEART) ss << "h";
   else ss << "s";

   return ss.str();
}

int handRank(short val) {
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

ostream& operator<<(ostream &os, Deck& d) {
   return os << d.show();
}

string Deck::show() {
   stringstream ss;
   int i;
   ss << "[" << cardstr(cards[0]);
   for (i = 1; i < DECK_SIZE; i++) 
      ss << cardstr(cards[i]);
   ss << "]";
   return ss.str();
}

vector<Position *> *Deck::neighbors() {
   vector<Position *> *listOfNeighbors = new vector<Position *>;
   int i, j, temp;

   #pragma omp parallel private(i, j, temp) shared(listOfNeighbors)
   {
      #pragma omp for schedule(static) nowait
      for (i = 0; i < DECK_SIZE - 1; i++) {
         for (j = i + 1; j < DECK_SIZE; j++) {
            // copy to a new Deck
            Deck *neighboringDeck = new Deck(*this);

            // swap a pair of cards
            temp = neighboringDeck->cards[i];
            neighboringDeck->cards[i] = neighboringDeck->cards[j];
            neighboringDeck->cards[j] = temp;

            #pragma omp critical
            listOfNeighbors->push_back(neighboringDeck);
         }
      }
   }

   return listOfNeighbors;
}

double Deck::value() {
   int cutPosn, count = 0, score1, score2;
   int p1hand[7], p2hand[7];
   
   for (cutPosn = 0; cutPosn < DECK_SIZE; cutPosn++) {
      if (cutPosn < DECK_SIZE - 11) {
         // no wrap around necessary (save hundreds of divisions in the inner most loop)
         p1hand[0] = cards[cutPosn];
         p1hand[1] = cards[cutPosn + 2];
         p1hand[2] = cards[cutPosn + 5];
         p1hand[3] = cards[cutPosn + 6];
         p1hand[4] = cards[cutPosn + 7];
         p1hand[5] = cards[cutPosn + 9];
         p1hand[6] = cards[cutPosn + 11];
         
         p2hand[0] = cards[cutPosn + 1];
         p2hand[1] = cards[cutPosn + 3];
         p2hand[2] = cards[cutPosn + 5];
         p2hand[3] = cards[cutPosn + 6];
         p2hand[4] = cards[cutPosn + 7];
         p2hand[5] = cards[cutPosn + 9];
         p2hand[6] = cards[cutPosn + 11];
      } else {
         p1hand[0] = cards[cutPosn];
         p1hand[1] = cards[(cutPosn + 2) % DECK_SIZE];
         p1hand[2] = cards[(cutPosn + 5) % DECK_SIZE];
         p1hand[3] = cards[(cutPosn + 6) % DECK_SIZE];
         p1hand[4] = cards[(cutPosn + 7) % DECK_SIZE];
         p1hand[5] = cards[(cutPosn + 9) % DECK_SIZE];
         p1hand[6] = cards[(cutPosn + 11) % DECK_SIZE];
         
         p2hand[0] = cards[(cutPosn + 1) % DECK_SIZE];
         p2hand[1] = cards[(cutPosn + 3) % DECK_SIZE];
         p2hand[2] = cards[(cutPosn + 5) % DECK_SIZE];
         p2hand[3] = cards[(cutPosn + 6) % DECK_SIZE];
         p2hand[4] = cards[(cutPosn + 7) % DECK_SIZE];
         p2hand[5] = cards[(cutPosn + 9) % DECK_SIZE];
         p2hand[6] = cards[(cutPosn + 11) % DECK_SIZE];
      }
      
      score1 = eval7Hand(p1hand);
      score2 = eval7Hand(p2hand);

      if (score1 < score2)
         count++;
   }

   return count;
}

// This is a embarassingly slow method of determining the
// best five-card hand possible out of seven cards.
// But it will suffice.
short eval7Hand(int *hand) {
    int i, q, best = 9999;

	for (i = 0; i < 21; i++) {
		q = eval5HandFast(hand[perm7[i][0]], hand[perm7[i][1]], hand[perm7[i][2]], 
                          hand[perm7[i][3]], hand[perm7[i][4]]);

		if (q < best)
			best = q;
	}

	return(best);
}

// perfect hash
unsigned findFast(unsigned u) {
    unsigned a, b, r;
    u += 0xe91aaa35;
    u ^= u >> 16;
    u += u << 8;
    u ^= u >> 4;
    b  = (u >> 8) & 0x1ff;
    a  = (u + (u << 2)) >> 19;
    r  = a ^ hashAdjust[b];
    return r;
}

int eval5HandFast(int c1, int c2, int c3, int c4, int c5) {
    int q = (c1 | c2 | c3 | c4 | c5) >> 16;
    short s;
    if (c1 & c2 & c3 & c4 & c5 & 0xf000) return flushes[q]; // check for flushes and straight flushes
    if ((s = unique5[q]))                return s;          // check for straights and high card hands
    return hashValues[findFast((c1 & 0xff) * (c2 & 0xff) * (c3 & 0xff) * (c4 & 0xff) * (c5 & 0xff))];
}

string playHands(int *cards) {
   int cutPosn, score1, score2;
   int p1hand[7], p2hand[7];
   stringstream output;   

   for (cutPosn = 0; cutPosn < DECK_SIZE; cutPosn++) {
      p1hand[0] = cards[cutPosn];
      p1hand[1] = cards[(cutPosn + 2) % DECK_SIZE];
      p1hand[2] = cards[(cutPosn + 5) % DECK_SIZE];
      p1hand[3] = cards[(cutPosn + 6) % DECK_SIZE];
      p1hand[4] = cards[(cutPosn + 7) % DECK_SIZE];
      p1hand[5] = cards[(cutPosn + 9) % DECK_SIZE];
      p1hand[6] = cards[(cutPosn + 11) % DECK_SIZE];
      
      p2hand[0] = cards[(cutPosn + 1) % DECK_SIZE];
      p2hand[1] = cards[(cutPosn + 3) % DECK_SIZE];
      p2hand[2] = cards[(cutPosn + 5) % DECK_SIZE];
      p2hand[3] = cards[(cutPosn + 6) % DECK_SIZE];
      p2hand[4] = cards[(cutPosn + 7) % DECK_SIZE];
      p2hand[5] = cards[(cutPosn + 9) % DECK_SIZE];
      p2hand[6] = cards[(cutPosn + 11) % DECK_SIZE];
      
      output << cardstr(p1hand[0]) << " " << cardstr(p1hand[1]) << 
         " | " << cardstr(p2hand[0]) << " " << cardstr(p2hand[1]) << 
         " | " << cardstr(p1hand[2]) << " " << cardstr(p1hand[3]) << 
         " " << cardstr(p1hand[4]) << " " << cardstr(p1hand[5]) << " " << 
         cardstr(p1hand[6]) << " | ";

      score1 = eval7Hand(p1hand);
      score2 = eval7Hand(p2hand);

      output << HAND_STRING[handRank(score1)] <<
         " | " << HAND_STRING[handRank(score2)] << " |\t";

      if (score1 < score2)
         output << "1\n";
      else if (score2 < score1)
         output << "2\n";
      else
         output << "Tie.\n"; 
   }
   
   return output.str();
}
 
int main(int argc, char* argv[]) {
   // usage: $ poker [numTrials [trialLength [threadc]]] 
   const int numTrials = argc > 1 ? atoi(argv[1]) : DEFAULT_NUM_TRIALS;
   const int trialLength = argc > 2 ? atoi(argv[2]) : DEFAULT_TRIAL_LENGTH;
   const int threadc = argc > 3 ? atoi(argv[3]) : DEFAULT_NUM_THREADS;

   srand(time(0));
   omp_set_num_threads(threadc);

   vector<Deck *> optimalStackings;
   Deck *bestSoFar = new Deck(), *result;
   int i;

   for (i = 0; i < numTrials; i++) {
      Deck startingDeck;
      result = (Deck *)hillclimb(&startingDeck, trialLength);

      if (result->value() == 52) {
         optimalStackings.push_back(result);

         if (bestSoFar->value() < 52)
            delete bestSoFar;

         bestSoFar = result;
      } else if (bestSoFar->value() < result->value()) {
         delete bestSoFar;
         bestSoFar = result;
      } else {
         delete result;
      }
   }

   cout << "Found " << optimalStackings.size() << " non-distinct optimal stackings:\n";
   vector<Deck *>::iterator itr;
   for (itr = optimalStackings.begin(); itr != optimalStackings.end(); itr++) {
      cout << "\n" << **itr << "\n";
      cout << playHands((*itr)->cards) << "\n";
      delete *itr;
   }

   return 0;
}

