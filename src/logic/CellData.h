#ifndef CELLDATA_H
#define CELLDATA_H

#include "Global.h"

namespace chess {
  namespace model {
    class CellData{
    public:
      CellData(Piece piece = Less, Color color = None);
      const Color color() const;
      const Piece piece() const;
    private:
      Piece mPiece;
      Color mColor;
    };
  }
}
#endif //CELLDATA_H
