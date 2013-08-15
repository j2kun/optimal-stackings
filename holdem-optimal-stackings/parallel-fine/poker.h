#include "hillclimb.h"
#include <iostream>
#include <vector>
#include <string>

#define DECK_SIZE 52

#define RANK(x) ((x >> 8) & 0xF)
#define CLUB 0x8000
#define DIAMOND 0x4000
#define HEART 0x2000
#define SPADE 0x1000

#define STRAIGHT_FLUSH 1
#define FOUR_OF_A_KIND 2
#define FULL_HOUSE 3
#define FLUSH 4
#define STRAIGHT 5
#define THREE_OF_A_KIND 6
#define TWO_PAIR 7
#define ONE_PAIR 8
#define HIGH_CARD 9

// #defines above are indices to this array
static const std::string HAND_STRING[] = {
   "      ???      ",
   "Straight Flush ",
   "Four of a Kind ",
   "Full House     ",
   "Flush          ",
   "Straight       ",
   "Three of a Kind",
   "Two Pair       ",
   "One Pair       ",
   "High Card      "
};

static const int INITIAL_DECK[DECK_SIZE] = {
   98306, 164099, 295429, 557831, 1082379, 2131213, 4228625, 8423187, 16812055, 
   33589533, 67144223, 134253349, 268471337, 81922, 147715, 279045, 541447, 
   1065995, 2114829, 4212241, 8406803, 16795671, 33573149, 67127839, 134236965, 
   268454953, 73730, 139523, 270853, 533255, 1057803, 2106637, 4204049, 8398611, 
   16787479, 33564957, 67119647, 134228773, 268446761, 69634, 135427, 266757, 
   529159, 1053707, 2102541, 4199953, 8394515, 16783383, 33560861, 67115551, 
   134224677, 268442665
};

short eval7Hand(int *hand); 
unsigned findFast(unsigned u);
int eval5HandFast(int c1, int c2, int c3, int c4, int c5); 
std::string cardstr(int card);
int handRank(short val); 
std::string playHands(int *deck); // for human-readable verification

class Deck : public Position {
private:
   void shuffle();

public:
   int *cards;

   Deck();
   Deck(const Deck &);
   ~Deck();

   double value();
   std::vector<Position *> *neighbors();
   std::string show();

   bool operator==(const Deck& other); // equality up to cuts

   friend std::ostream& operator<<(std::ostream& os, const Deck& d);
};

