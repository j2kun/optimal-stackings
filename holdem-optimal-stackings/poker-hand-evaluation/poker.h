#include <string>

#define STRAIGHT_FLUSH 1
#define FOUR_OF_A_KIND 2
#define FULL_HOUSE 3
#define FLUSH 4
#define STRAIGHT 5
#define THREE_OF_A_KIND 6
#define TWO_PAIR 7
#define ONE_PAIR 8
#define HIGH_CARD 9

#define RANK(x) ((x >> 8) & 0xF)

#define CLUB 0x8000
#define DIAMOND 0x4000
#define HEART 0x2000
#define SPADE 0x1000

//   A card is a sequence of four bytes. The high-order
//   bytes are used to hold the rank bit pattern, whereas
//   the low-order bytes hold the suit/rank/prime value
//   of the card.
//
//   +--------+--------+--------+--------+
//   |xxxbbbbb|bbbbbbbb|cdhsrrrr|xxpppppp|
//   +--------+--------+--------+--------+
//
//   p = prime number of rank (deuce=2,trey=3,four=5,five=7,...,ace=41)
//   r = rank of card (deuce=0,trey=1,four=2,five=3,...,ace=12)
//   cdhs = suit of card
//   b = bit turned on depending on rank of card (ith bit for card of rank i)

static const std::string value_str[] = {
   "",
   "Straight Flush",
   "Four of a Kind",
   "Full House",
   "Flush",
   "Straight",
   "Three of a Kind",
   "Two Pair",
   "One Pair",
   "High Card"
};

void init_deck(int *deck);
void print_hand(int *hand, int n);
int hand_rank(short val);
short eval_7hand(int *hand);
unsigned find_fast(unsigned u);
int eval_5hand_fast(int c1, int c2, int c3, int c4, int c5);
