#include "Model.h"
#include "Global.h"

#include <iostream>

namespace chess {
namespace model {
Model::Model() : isWhiteMove(true) {
    mCells.resize(Total);
    autoFill();
}

void Model::place(const CellData& chessman, const Position &pos) {
    int i = index(pos);
    if (i < mCells.size()) mCells.at(i) = chessman;
}

void Model::place(const CellData& chessman, int x, int y) {
    place(chessman, Position(x, y));
}

void Model::move(const Position& startPos, const Position& endPos) {
    if (startPos == endPos) return;
    CellData startCell = cellDataFrom(startPos);
    CellData endCell = cellDataFrom(endPos);
    if (startCell.color() == endCell.color()) return;

    if ((startCell.color() == White) == isWhiteMove) {
        if (startCell.piece() != Less) {
            place(startCell, endPos);
            place(CellData(), startPos);
            isWhiteMove = !isWhiteMove;
        }
    }
}

void Model::autoFill() {    
    place(CellData(Rook   ,White) ,A ,One);
    place(CellData(Knight ,White) ,B ,One);
    place(CellData(Bishop ,White) ,C ,One);
    place(CellData(Queen  ,White) ,D ,One);
    place(CellData(King   ,White) ,E ,One);
    place(CellData(Bishop ,White) ,F ,One);
    place(CellData(Knight ,White) ,G ,One);
    place(CellData(Rook   ,White) ,H ,One);
    for (int i = 0; i < 8; ++i) place(CellData(Pawn, White), i, Two);
    place(CellData(Rook   ,Black) ,A ,Eight);
    place(CellData(Knight ,Black) ,B ,Eight);
    place(CellData(Bishop ,Black) ,C ,Eight);
    place(CellData(Queen  ,Black) ,D ,Eight);
    place(CellData(King   ,Black) ,E ,Eight);
    place(CellData(Bishop ,Black) ,F ,Eight);
    place(CellData(Knight ,Black) ,G ,Eight);
    place(CellData(Rook   ,Black) ,H ,Eight);
    for (int i = 0; i < 8; ++i) place(CellData(Pawn, Black), i, Seven);
}

void Model::print() {
    int num = 1;
    for (int i = 0; i < Total; ++i) {
        if (0 == i % 8) printf("\n %d | ", num++);
        if (i < getCells().size()) {
            model::CellData ch = getCells().at(i);
            printf("%d%d ", ch.piece(), ch.color());
        }
    }
    printf("\n");
}

CellData Model::cellDataFrom(const Position& position) const {
    int pos = index(position);
    if ((pos >= 0) && (Total > pos)) return mCells.at(pos);
    else                            return CellData();
}

std::vector<CellData> Model::getCells() const {
    return mCells;
}

int Model::index(const Position& pos) const {
    return pos.x + Height * pos.y;
}

}
}
