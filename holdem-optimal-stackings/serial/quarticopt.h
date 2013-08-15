#include "hillclimb.h"
#include <vector>
#include <string>

class XValue : public Position {
public:
   double x;

   XValue(double x);
   ~XValue();

   double value();
   std::vector<Position *> *neighbors();
   std::string show();
};

