#include "CellData.h"

namespace chess {
namespace model {
CellData::CellData(Piece piece, Color color)
    : mPiece(piece)
    , mColor(color) { }

Color CellData::color() {
    return mColor;
}

Piece CellData::piece() {
    return mPiece;
}
}
}
