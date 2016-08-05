#include "Chesscell.h"

namespace nsChess {
  Chesscell::Chesscell()
  : mPiece(Empty)
  , mColor(None) { }

  Chesscell::Chesscell(Piece piece, Color color)
  : mPiece(piece)
  , mColor(color) { }

  Color Chesscell::color() {
    return mColor;
  }

  Piece Chesscell::piece() {
    return mPiece;
  }
}
