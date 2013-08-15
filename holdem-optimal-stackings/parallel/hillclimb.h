#include <vector>
#include <string>

class Position {
public:
   Position();
   virtual ~Position();
   virtual double value() = 0; 

   /* we require the inheriting class to dynamically allocate, and then 
    * relinquish ownership of the list of Position*s and each member 
    * Position* within the list.
    */
   virtual std::vector<Position *> *neighbors() = 0; 
   virtual std::string show() = 0;
};

// caller retains ownership of the given Position*
Position* hillclimb(Position* posn, const int numSteps);
