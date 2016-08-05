#ifndef CHESSCELL_H
#define CHESSCELL_H

#include "Global.h"
namespace nsChess {
  class Chesscell {
  public:
    Chesscell();
    Chesscell(Piece piece, Color color);

    Color color();
    Piece piece();
  public:
    Piece mPiece;
    Color mColor;
  };
}
#endif // CHESSCELL_H
