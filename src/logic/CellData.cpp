#include "CellData.h"

namespace chess {
  namespace model {
    CellData::CellData(Piece piece, Color color)
    : mPiece(piece)
    , mColor(color) { }

    const Color CellData::color() const {
      return mColor;
    }

    const Piece CellData::piece() const {
      return mPiece;
    }

    void CellData::setPiece(Piece piece) {
      mPiece = piece;
    }
  }
}
