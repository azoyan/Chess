#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Chesscell.h"

namespace model {
  class Model {
  public:
    Model();
  public:
    void place(const Chesscell& chessman, const Coordinate& pos);
    void place(const Chesscell& chessman, int x, int y);
    bool move(const Coordinate& start,   const Coordinate& end);

    void print();
    void autoFill();

    Chesscell get(const Coordinate& position) const;
    std::vector<Chesscell> getChesscells() const;
  private:    
    std::vector<Chesscell> mChesscells;
    int index(const Coordinate& pos) const;
    bool isWhiteMove;
  };
}
#endif // MODEL_H
