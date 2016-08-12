#include "Model.h"
#include "Global.h"

#include <iostream>

namespace chess {
  namespace model {
    Model::Model() : isWhiteMove(true) {
      clear();
      autoFill();
    }

    void Model::place(const CellData& chessman, const Position &pos) {
      int i = to1d(pos);
      if (i < mCells.size()) mCells.at(i) = chessman;
    }

    void Model::place(const CellData& chessman, int x, int y) {
      place(chessman, Position(x, y));
    }

    void Model::move(const Position& startPos, const Position& endPos) {
      if (startPos == endPos) return;
      CellData startCellData = cellDataFrom(startPos);
      CellData endCellData = cellDataFrom(endPos);
      if (startCellData.color() == endCellData.color()) return;

      // xor. Если фигура белая и ход белых, или если черная и не ход белых
      if ((startCellData.color() == White) == isWhiteMove) {
          if (startCellData.piece() != Less) {
              place(startCellData, endPos);
              place(CellData(), startPos);
              mSnapshots.push({startPos, startCellData, endPos, endCellData});
              isWhiteMove = !isWhiteMove;
          }
      }
    }

    void Model::undo() {
      if (!mSnapshots.empty()) {
        Snapshot s = mSnapshots.top();
        place(s.startData, s.startPos);
        place(s.endData, s.endPos);
        mSnapshots.pop();
        isWhiteMove = !isWhiteMove;
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
      for (int i = 0; i < Width; ++i) place(CellData(Pawn, White), i, Two);
      place(CellData(Rook   ,Black) ,A ,Eight);
      place(CellData(Knight ,Black) ,B ,Eight);
      place(CellData(Bishop ,Black) ,C ,Eight);
      place(CellData(Queen  ,Black) ,D ,Eight);
      place(CellData(King   ,Black) ,E ,Eight);
      place(CellData(Bishop ,Black) ,F ,Eight);
      place(CellData(Knight ,Black) ,G ,Eight);
      place(CellData(Rook   ,Black) ,H ,Eight);
      for (int i = 0; i < Width; ++i) place(CellData(Pawn, Black), i, Seven);
    }

    void Model::clear() {
      isWhiteMove = true;
      mCells.clear();
      mCells.resize(Total);
    }

    void Model::print() {
      int num = 1;
      for (int i = 0; i < Total; ++i) {
        if (0 == i % Width) printf("\n %d | ", num++);
          if (i < getCells().size()) {
              model::CellData ch = getCells().at(i);
              printf("%d%d ", ch.piece(), ch.color());
        }
      }
      printf("\n");
    }

    CellData Model::cellDataFrom(const Position& position) const {
      int pos = to1d(position);
      if ((pos >= 0) && (Total > pos)) return mCells.at(pos);
      else                             return CellData();
    }

    const std::vector<CellData> Model::getCells() const {
      return mCells;
    }

    int Model::to1d(const Position& pos) const {
      return pos.x + Height * pos.y;
    }
  }
}
