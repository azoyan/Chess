#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "CellData.h"
#include <stack>

namespace chess {
  namespace model {
    struct Snapshot {
        std::vector<CellData> cells;
        bool canWhiteCastlingLeft;
        bool canWhiteCastlingRight;
        bool canBlackCastlingLeft;
        bool canBlackCastlingRight;
    };

    class Model {
    public:
      Model();
    public:
      void place(const CellData& chessman, const Position& pos);
      void place(const CellData& chessman, int x, int y);
      void move(const Position& startPos,  const Position& endPos);
      void undo();
      std::vector<Position> possibleMoves(const Position& position);

      bool isWhiteMove;
      bool canWhiteCastlingLeft;
      bool canWhiteCastlingRight;
      bool canBlackCastlingLeft;
      bool canBlackCastlingRight;

      void print();
      void autoFill();
      void clear();
      int to1d(const Position& pos) const;
      const std::vector<CellData> getCells() const;
    private:
      CellData cellDataFrom(const Position& position) const;
      CellData cellDataFrom(int x, int y) const;
    private:
      void insertPawnPossiblePositions(std::vector<Position>& result, const Position &position, Color ownColor);
      void insertKnightPossiblePositions(std::vector<Position>& result, const Position &position, Color ownColor);
      void insertBishopPossiblePositions(std::vector<Position>& result, const Position &position, Color ownColor);
      void insertRookPossiblePositions(std::vector<Position>& result, const Position &position, Color ownColor);
      void insertQueenPossiblePositions(std::vector<Position>& result, const Position &position, Color ownColor);
      void insertKingPossiblePositions(std::vector<Position>& result, const Position &position, Color ownColor);
      void insertPosition(std::vector<Position>& result, Position position, int directionX, int directionY, Color ownColor);
    private:
      bool attemptCastling(const Position& startPos, const Position& endPos, Color color);
      bool attemptCastlingLeft(const Position& startPos, const Position& endPos, Color color);
      bool attemptCastlingRight(const Position& startPos, const Position& endPos, Color color);
      void castling(const Position& startPos, const Position& endPos, const Position& rookStartPos, const Position& rookEndPos, Color color);
      Snapshot makeSnapshot();
    private:
      std::vector<CellData> mCells;
      std::stack<Snapshot> mSnapshots;
    };
  }
}
#endif
