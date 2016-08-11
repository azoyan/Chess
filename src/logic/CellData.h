#ifndef CELLDATA_H
#define CELLDATA_H

#include "Global.h"

namespace chess {
namespace model {
class CellData{
public:
    CellData(Piece piece = Less, Color color = None);

    Color color();
    Piece piece();
public:
    Piece mPiece;
    Color mColor;
};
}
}
#endif //CELLDATA_H
