#ifndef GLOBAL_H
#define GLOBAL_H

namespace nsChess {
  enum Color { None, White, Black };
  enum Piece { Empty, Pawn, Knight, Bishop, Rook, Queen, King };
  enum Size  { Width = 8, Height = 8, Total = Width * Height };

  class Coordinate {
  public:
    Coordinate(int x, int y) : mX(x), mY(y) { }
     int x() const { return mX; }
     int y() const { return mY; }
     bool operator==(const Coordinate& c) const {
        return (mX == c.x() && mY ==c.y());
  }
  private:
    int mX;
    int mY;
  };
}
#endif // GLOBAL_H
