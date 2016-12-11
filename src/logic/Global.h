#ifndef GLOBAL_H
#define GLOBAL_H

namespace chess {
  namespace model {
    enum class Color  { None = -1, White = 1, Black = 0 };
    enum class Piece  { None = -1, Pawn = 5, Knight = 3, Bishop = 4, Rook = 2, Queen = 1, King = 0 };
    enum Size   { Width = 8, Height = Size::Width, Total = Size::Width * Size::Height, PiecesCount = Size::Total / 2 };
    enum Row    { Eight, Seven, Six, Five, Four, Three, Two, One };
    enum Column { A = 0, B, C, D, E, F, G, H };



    struct Position {
      Position(int x = Column::A, int y = Row::Eight) : x(x), y(y) { }
      bool operator==(const Position& p) const {
          return (x == p.x && y == p.y);
      }
      int y;
      int x;
    };
  }
}
#endif // GLOBAL_H
