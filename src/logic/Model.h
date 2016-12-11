#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "CellData.h"
#include <stack>
#include <list>

namespace chess {
  namespace model {
    struct Snapshot {
        std::vector<CellData> cells;
        bool canWhiteCastlingLeft;
        bool canWhiteCastlingRight;
        bool canBlackCastlingLeft;
        bool canBlackCastlingRight;
    };

    struct Enpassant { Position endEnpassant; Position endPos; };

    class Model {
    public:
      using Positions = std::vector<Position>;
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

      void print() const;
      void autoFill();
      void clear();
      static int to1d(const Position& pos);
      const std::vector<CellData> getCells() const;
      bool isPromotion();
      bool isCheckmate();
      void setPromotionPiece(Piece piece);
    private:
      CellData cellDataFrom(const Position& position) const;
      CellData cellDataFrom(int x, int y) const;
    private:
      Positions extractPossiblePositions(const Position& position, Piece piece, Color color);
      Positions extractPawnPossiblePositions(const Position &position, Color ownColor);
      Positions extractPawnEnemyPosition(const Position& pos, Color own) const;
      Positions extractKnightPossiblePositions(const Position &position, Color ownColor) const;
      Positions extractBishopPossiblePositions(const Position &position, Color ownColor);
      Positions extractRookPossiblePositions(const Position &position, Color ownColor);
      Positions extractQueenPossiblePositions(const Position &position, Color ownColor);
      Positions extractKingPossiblePositions(const Position &position, Color ownColor) const;
      Positions extractPositionsByDirection(Position position, int directionX, int directionY, Color ownColor) const;
    private:
      bool attemptCastling(const Position& startPos, const Position& endPos, Color color);
      bool attemptCastlingLeft(const Position& startPos, const Position& endPos, Color color);
      bool attemptCastlingRight(const Position& startPos, const Position& endPos, Color color);
      void castling(const Position& startPos, const Position& endPos, const Position& rookStartPos, const Position& rookEndPos, Color color);
      Snapshot makeSnapshot() const;
    private:
      void addEnpassant(const Position& endEnpassant, const Position& endPos);
      void clearEnpassants();
      Color currentColor() const;
      static Positions filtOutsides(const std::vector<Position>& positions);
      Positions filtIllegals(const Positions& positions, const Position& position, CellData chessman);
      bool isKingUnderCheck(const Position& startPos, const Position& endPos, CellData chessman);
      bool mIsPromotion;
    private:
      std::vector<CellData> mCells;
      std::list<Snapshot> mSnapshots;
      std::stack<Enpassant> mEnpassants;
      Position whiteKingPositioin;
      Position blackKingPositioin;
      Position mPromotionPosition;
    };
  }
}
#endif
