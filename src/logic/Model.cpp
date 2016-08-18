#include "Model.h"
#include "Global.h"

#include <iostream>

namespace chess {
  namespace model {
    Model::Model()
    : isWhiteMove(true)
    , canCastlingLeft(true)
    , canCastlingRight(true)
    {
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

      std::vector<Position> positions = possibleMoves(startPos);
      for (auto i : positions) {
        if (i == endPos) {

      // xor. Если фигура белая и ход белых, или если черная и не ход белых
          if ((startCellData.color() == White) == isWhiteMove) {
            if (startCellData.piece() != Less) {

              attemptCastling(startCellData, startPos, endPos);

              place(startCellData, endPos);
              place(CellData(), startPos);
              if (startCellData.piece() == Rook && startPos == Position(H, Eight)) canCastlingLeft = false;
              mSnapshots.push({startPos, startCellData, endPos, endCellData});
              isWhiteMove = !isWhiteMove;
            }
          }
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

    std::vector<Position> Model::possibleMoves(const Position& position) {
      std::vector<Position> result;
      CellData chessman = cellDataFrom(position);
      Color ownColor = chessman.color();
      if (ownColor == White && !isWhiteMove || ownColor == Black && isWhiteMove) return result;
      switch (chessman.piece()) {
      case Pawn   :insertPawnPossiblePositions  (result, position, ownColor) ;break;
      case Knight :insertKnightPossiblePositions(result, position, ownColor) ;break;
      case Bishop :insertBishopPossiblePositions(result, position, ownColor) ;break;
      case Rook   :insertRookPossiblePositions  (result, position, ownColor) ;break;
      case Queen  :insertQueenPossiblePositions (result, position, ownColor) ;break;
      case King   :insertKingPossiblePositions  (result, position, ownColor) ;break;
      return result;
    }
  }

    void Model::autoFill() {
      clear();
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

    CellData Model::cellDataFrom(int x, int y) const {
        return cellDataFrom(Position(x, y));
    }

    void Model::insertPawnPossiblePositions(std::vector<Position> &result, const Position &position, Color ownColor) {
      int direction = (ownColor != White) ? 1 : -1;

      Position oneStepForward = Position(position.x, position.y + direction * 1);
      if (cellDataFrom(oneStepForward).color() == None) {
        result.push_back(oneStepForward);
        bool firstPawnMove = position.y == Two || position.y == Seven;
        Position twoStepForward = Position(position.x, position.y + direction * 2);
        if (firstPawnMove && cellDataFrom(twoStepForward).color() == None) {
          result.push_back(twoStepForward);
        }
      }

      Position rightEnemyPos(position.x + direction * 1, position.y + direction * 1);
      CellData rightEnemy = cellDataFrom(rightEnemyPos);
      if (rightEnemy.color() != ownColor && rightEnemy.color() != None) {
        result.push_back(rightEnemyPos);
      }
      Position leftEnemyPos(position.x - direction * 1, position.y + direction * 1);
      CellData leftEnemy = cellDataFrom(leftEnemyPos);
      if (leftEnemy.color() != ownColor && leftEnemy.color() != None) {
        result.push_back(leftEnemyPos);
      }
    }

    void Model::insertKnightPossiblePositions(std::vector<Position> &result, const Position &position, Color ownColor) {
      std::vector<Position> positions = {
       Position(position.x + 1, position.y + 2)
      ,Position(position.x - 1, position.y + 2)
      ,Position(position.x + 1, position.y - 2)
      ,Position(position.x - 1, position.y - 2)
      ,Position(position.x + 2, position.y + 1)
      ,Position(position.x - 2, position.y + 1)
      ,Position(position.x + 2, position.y - 1)
      ,Position(position.x - 2, position.y - 1) };

      for (auto i : positions) {
        Color enemyColor = cellDataFrom(i).color();
        if (enemyColor == None || enemyColor != ownColor) result.push_back(i);
      }
    }

    void Model::insertBishopPossiblePositions(std::vector<Position>& result, const Position& position,Color ownColor) {
      insertPosition(result, position,  1,  1, ownColor);
      insertPosition(result, position, -1,  1, ownColor);
      insertPosition(result, position,  1, -1, ownColor);
      insertPosition(result, position, -1, -1, ownColor);
    }

    void Model::insertRookPossiblePositions(std::vector<Position>& result, const Position& position, Color ownColor) {
      insertPosition(result, position,  0, 1, ownColor);
      insertPosition(result, position,  0,-1, ownColor);
      insertPosition(result, position,  1, 0, ownColor);
      insertPosition(result, position, -1, 0, ownColor);
    }

    void Model::insertQueenPossiblePositions(std::vector<Position> &result, const Position &position, Color ownColor) {
        insertBishopPossiblePositions(result, position, ownColor);
        insertRookPossiblePositions(result, position, ownColor);
    }

    void Model::insertKingPossiblePositions(std::vector<Position> &result, const Position &position, Color ownColor) {
      std::vector<Position> positions = {
       Position(position.x + 1, position.y + 0)
      ,Position(position.x - 1, position.y + 0)
      ,Position(position.x + 0, position.y + 1)
      ,Position(position.x - 0, position.y - 1)
      ,Position(position.x + 1, position.y + 1)
      ,Position(position.x - 1, position.y + 1)
      ,Position(position.x + 1, position.y - 1)
      ,Position(position.x - 1, position.y - 1)
      };


      Position castlingRight(position.x + 2, position.y);
      Position castlingLeft(position.x - 2, position.y);
      if (canCastlingRight && cellDataFrom(Position(position.x + 2, position.y)).piece() == Less) {
        result.push_back(castlingRight);
      }
      if (canCastlingLeft && cellDataFrom(Position(position.x - 2, position.y)).piece() == Less) {
        result.push_back(castlingLeft);
      }

      for (auto i : positions) {
        CellData enemy = cellDataFrom(i);
        if (enemy.color() == None || enemy.color() != ownColor) result.push_back(i);
      }
    }

    void Model::insertPosition(std::vector<Position> &result, Position position, int directionX, int directionY, Color ownColor) {
      for (int i = 1; i <= Width; ++i) {
        Position p = Position(position.x + directionX * i, position.y + directionY * i);
        CellData chessman = cellDataFrom(p);
        Color color = chessman.color();
        if (color == None) result.push_back(p);
        else if (color != ownColor) {
          result.push_back(p);
          break;
        }
        else break;
      }
    }

    void Model::attemptCastling(const CellData &startData, const Position &startPos, const Position &endPos) {

    }

    const std::vector<CellData> Model::getCells() const {
      return mCells;
    }

    int Model::to1d(const Position& pos) const {
      return pos.x + Height * pos.y;
    }
  }
}
