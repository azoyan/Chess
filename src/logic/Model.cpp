#include "Model.h"
#include "Global.h"

#include <iostream>

namespace chess {
  namespace model {
    Model::Model()
    : isWhiteMove(true)
    , canWhiteCastlingLeft(true)
    , canWhiteCastlingRight(true)
    , canBlackCastlingLeft(true)
    , canBlackCastlingRight(true)
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
          Piece piece = startCellData.piece();
          if (piece != Less) {
            Color color = startCellData.color();
            if ((color == White) == isWhiteMove) {
              mSnapshots.push(makeSnapshot());
             // clearEnpassants();
              if (piece == King) {
                if (!attemptCastling(startPos, endPos, color)) {
                  if (isWhiteMove) canWhiteCastlingLeft = canWhiteCastlingRight = false;
                  else             canBlackCastlingLeft = canBlackCastlingRight = false;
                }
              }
              if (piece == Rook) {
                if (startPos.x == A && startPos.y == One)   canWhiteCastlingLeft  = false;
                if (startPos.x == H && startPos.y == One)   canWhiteCastlingRight = false;
                if (startPos.x == A && startPos.y == Eight) canBlackCastlingLeft  = false;
                if (startPos.x == H && startPos.y == Eight) canBlackCastlingRight = false;
              }
              if (piece == Pawn) {
                bool isTwoSteps = (endPos.y - startPos.y) % 2 == 0;
                int direction = (endPos.y - startPos.y) < 0 ? 1 : -1;
                if (!mEnpassants.empty()) {
                  printf("1endPos: %d %d, endEnpassant: %d %d\n", endPos.x, endPos.y, mEnpassants.top().endEnpassant.x, mEnpassants.top().endEnpassant.y);
                  if (endPos == mEnpassants.top().endEnpassant) {
                    place(CellData(), mEnpassants.top().endPos);
                  }
                }
                if (isTwoSteps) {
                  Position endEnpassant(endPos.x, endPos.y + direction * 1);
                  clearEnpassants();
                  if ((cellDataFrom(endPos.x + 1, endPos.y).piece() == Pawn) ||
                      (cellDataFrom(endPos.x - 1, endPos.y).piece() == Pawn)) {
                    addEnpassant(endEnpassant, endPos);
                    printf("0endPos: %d %d, endEnpassant: %d %d\n", endPos.x, endPos.y, endEnpassant.x, endEnpassant.y);
                  }
                }
              }
              place(startCellData, endPos);
              place(CellData(), startPos);
              isWhiteMove = !isWhiteMove;
            }
          }
        }
      }
    }
    void Model::undo() {
      if (!mSnapshots.empty()) {
        mCells = mSnapshots.top().cells;
        canWhiteCastlingLeft  = mSnapshots.top().canWhiteCastlingLeft;
        canWhiteCastlingRight = mSnapshots.top().canWhiteCastlingRight;
        canBlackCastlingLeft  = mSnapshots.top().canBlackCastlingLeft;
        canBlackCastlingRight = mSnapshots.top().canBlackCastlingRight;
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
      while(!mSnapshots.empty()) mSnapshots.pop();
    }

    void Model::print() {
      int num = 1;
      for (int i = 0; i < Total; ++i) {
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
        bool canTwoStep = position.y == Two || position.y == Seven;
        Position twoStepForward = Position(position.x, position.y + direction * 2);
        if (canTwoStep && cellDataFrom(twoStepForward).color() == None) {
          result.push_back(twoStepForward);
        }
      }

      Position rightEnemyPos(position.x + direction * 1, position.y + direction * 1);      
      Position leftEnemyPos (position.x - direction * 1, position.y + direction * 1);

      insertPawnEnemyPosition(result, rightEnemyPos, ownColor);
      insertPawnEnemyPosition(result, leftEnemyPos, ownColor);

      if (!mEnpassants.empty()) {
        Position enpassantRight(position.x + 1, position.y + direction * 1);
        Position enpassantLeft (position.x - 1, position.y + direction * 1);
        Position endEnpassant = mEnpassants.top().endEnpassant;
        if (enpassantLeft == endEnpassant || enpassantRight == endEnpassant) {
          if (ownColor != cellDataFrom(mEnpassants.top().endPos).color()) {
            result.push_back(mEnpassants.top().endEnpassant);
          }
        }
      }
    }

    void Model::insertPawnEnemyPosition(std::vector<Position>& result, const Position& pos, Color own) {
        CellData enemy = cellDataFrom(pos);
        if (enemy.color() != own && enemy.color() != None) result.push_back(pos);

    }

    void Model::addEnpassant(const Position& endEnpassant, const Position &endPos) {
        mEnpassants.push({endEnpassant, endPos});
    }

    void Model::clearEnpassants() {
      while (!mEnpassants.empty()) mEnpassants.pop();
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

      Position castlingRight1(position.x + 1, position.y);
      Position castlingRight2(position.x + 2, position.y);
      Position castlingRight3(position.x + 3, position.y);
      if (cellDataFrom(castlingRight1).piece() == Less &&
          cellDataFrom(castlingRight2).piece() == Less &&
          cellDataFrom(castlingRight3).piece() == Rook) {
        if (canWhiteCastlingRight && ownColor == White ||
            canBlackCastlingRight && ownColor == Black) {
          result.push_back(castlingRight2);
        }
      }
      Position castlingLeft1(position.x - 1, position.y);
      Position castlingLeft2(position.x - 2, position.y);
      Position castlingLeft3(position.x - 3, position.y);
      Position castlingLeft4(position.x - 4, position.y);
      if (cellDataFrom(castlingLeft1).piece() == Less &&
          cellDataFrom(castlingLeft2).piece() == Less &&
          cellDataFrom(castlingLeft3).piece() == Less &&
          cellDataFrom(castlingLeft4).piece() == Rook) {
        if (canWhiteCastlingLeft && ownColor == White ||
            canBlackCastlingLeft && ownColor == Black) {
          result.push_back(castlingLeft2);
        }
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

    bool Model::attemptCastling(const Position &startPos, const Position &endPos, Color color) {
      bool ok = false;
      printf("startPosX: %d, endPosX: %d\n", startPos.x, endPos.x);
      if (endPos.x < startPos.x) {
        if ((canWhiteCastlingLeft && color == White) ||
            (canBlackCastlingLeft && color == Black)) {
          ok = attemptCastlingLeft(startPos, endPos, color);
        }
      }
      if (endPos.x > startPos.x) {
        if ((canWhiteCastlingRight && color == White) ||
          (canBlackCastlingRight && color == Black) &&
          (endPos.x > startPos.x)) {
        ok = attemptCastlingRight(startPos, endPos, color);
        }
      }
      return ok;
    }

    bool Model::attemptCastlingLeft(const Position &startPos, const Position &endPos, Color color) {
      Position rookStartPos(startPos.x - 4, startPos.y);
      Position rookEndPos(startPos.x - 1, startPos.y);
      bool canCastling =
              (Less == cellDataFrom(rookEndPos).piece())      &&
              (Less == cellDataFrom(startPos.x - 2, startPos.y).piece()) &&
              (Less == cellDataFrom(startPos.x - 3, startPos.y).piece()) &&
              (Rook == cellDataFrom(rookStartPos).piece());
      if (canCastling) castling(startPos, endPos, rookStartPos, rookEndPos, color);
      return canCastling;
    }

    bool Model::attemptCastlingRight(const Position& startPos, const Position& endPos, Color color) {
      Position rookStartPos(startPos.x + 3, startPos.y);
      Position rookEndPos(startPos.x + 1, startPos.y);

      bool canCastling = (cellDataFrom(rookStartPos).piece() == Rook) &&
                         (cellDataFrom(rookEndPos).piece()   == Less) &&
           (cellDataFrom(startPos.x + 2, startPos.y).piece() == Less);
      printf("endPosX: %d\n", rookEndPos.x);
      if (canCastling) castling(startPos, endPos, rookStartPos, rookEndPos, color);
      return canCastling;
    }

    void Model::castling(const Position& startPos, const Position& endPos, const Position& rookStartPos, const Position& rookEndPos, Color color) {
      CellData rook(Rook, color);
      CellData king(King, color);
      place(king, endPos);
      place(CellData(), startPos);

      place(rook, rookEndPos);
      place(CellData(), rookStartPos);

      if (color == White) canWhiteCastlingRight = canWhiteCastlingRight = false;
      if (color == Black) canBlackCastlingRight = canBlackCastlingRight = false;
    }

    Snapshot Model::makeSnapshot() {
        return { mCells,
                 canWhiteCastlingLeft,
                 canWhiteCastlingRight,
                 canBlackCastlingLeft,
                 canBlackCastlingRight
               };
    }

    const std::vector<CellData> Model::getCells() const {
      return mCells;
    }

    int Model::to1d(const Position& pos) const {
      return pos.x + Height * pos.y;
    }
  }
}
