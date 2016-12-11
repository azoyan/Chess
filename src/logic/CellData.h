#ifndef CELLDATA_H
#define CELLDATA_H

#include "Global.h"

namespace chess {
  namespace model {
    class CellData{
    public:
      CellData(Piece piece = Piece::None, Color color = Color::None);
      const Color color() const;
      const Piece piece() const;
      void setPiece(Piece piece);
    private:
      Piece mPiece;
      Color mColor;
    };
  }
}
#endif //CELLDATA_H
