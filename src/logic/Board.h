#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Chesscell.h"

namespace nsChess {
  class Board {
  public:
    Board();
  public:
    void place(const Chesscell& chessman, const Coordinate& pos);
    void place(const Chesscell& chessman, int x, int y);
    void move (const Coordinate& start,   const Coordinate& end);

    Chesscell get(const Coordinate& position) const;
    std::vector<Chesscell> getChesscells() const;
  private:
    enum { Width = 8, Height = 8, Total = Width * Height };
    std::vector<Chesscell> mChesscells;
    int index(const Coordinate& pos) const;
  };
}
#endif // BOARD_H
