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
      , mIsPromotion(false)
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

      Positions possiblePositions = possibleMoves(startPos);
      for (auto possiblePosition : possiblePositions) {
        if (possiblePosition == endPos) {
          Piece piece = startCellData.piece();
          if (piece != Piece::None) {
            Color color = startCellData.color();
            if ((color == Color::White) == isWhiteMove) {
              mSnapshots.push_back(makeSnapshot());
              // clearEnpassants();
              if (piece == Piece::King) {
                if (!attemptCastling(startPos, endPos, color)) {
                  if (isWhiteMove) canWhiteCastlingLeft = canWhiteCastlingRight = false;
                  else             canBlackCastlingLeft = canBlackCastlingRight = false;
                }
                if (isWhiteMove) whiteKingPositioin = endPos;
                else blackKingPositioin = endPos;
              }
              if (piece == Piece::Rook) {
                if (startPos.x == Column::A && startPos.y == Row::One)   canWhiteCastlingLeft  = false;
                if (startPos.x == Column::H && startPos.y == Row::One)   canWhiteCastlingRight = false;
                if (startPos.x == Column::A && startPos.y == Row::Eight) canBlackCastlingLeft  = false;
                if (startPos.x == Column::H && startPos.y == Row::Eight) canBlackCastlingRight = false;
              }
              if (piece == Piece::Pawn) {
                bool isTwoSteps = (endPos.y - startPos.y) % 2 == 0;
                int direction = (endPos.y - startPos.y) < 0 ? 1 : -1;
                if (!mEnpassants.empty()) {
                  if (endPos == mEnpassants.top().endEnpassant) {
                    place(CellData(), mEnpassants.top().endPos);
                  }
                }
                if (isTwoSteps) {
                  Position endEnpassant(endPos.x, endPos.y + direction * 1);
                  clearEnpassants();
                  if ((cellDataFrom(endPos.x + 1, endPos.y).piece() == Piece::Pawn) ||
                      (cellDataFrom(endPos.x - 1, endPos.y).piece() == Piece::Pawn)) {
                    addEnpassant(endEnpassant, endPos);
                  }
                }
                mIsPromotion = endPos.y == One || endPos.y == Eight;
                if (mIsPromotion) mPromotionPosition = endPos;
              }
              place(startCellData, endPos);
              place(CellData(), startPos);
              isWhiteMove = !isWhiteMove;
              if (isCheckmate()) printf("checkmate\n");
            }
          }
        }
      }
    }
    void Model::undo() {
      if (!mSnapshots.empty()) {
        mCells = mSnapshots.back().cells;
        canWhiteCastlingLeft  = mSnapshots.back().canWhiteCastlingLeft;
        canWhiteCastlingRight = mSnapshots.back().canWhiteCastlingRight;
        canBlackCastlingLeft  = mSnapshots.back().canBlackCastlingLeft;
        canBlackCastlingRight = mSnapshots.back().canBlackCastlingRight;
        mSnapshots.pop_back();
        isWhiteMove = !isWhiteMove;
      }
    }

    void Model::autoFill() {
      clear();
      place(CellData(Piece::Rook   ,Color::White) ,A ,One);
      place(CellData(Piece::Knight ,Color::White) ,B ,One);
      place(CellData(Piece::Bishop ,Color::White) ,C ,One);
      place(CellData(Piece::Queen  ,Color::White) ,D ,One);
      place(CellData(Piece::King   ,Color::White) ,E ,One);
      place(CellData(Piece::Bishop ,Color::White) ,F ,One);
      place(CellData(Piece::Knight ,Color::White) ,G ,One);
      place(CellData(Piece::Rook   ,Color::White) ,H ,One);
      for (int i = 0; i < Size::Width; ++i) place(CellData(Piece::Pawn, Color::White), i, Two);
      place(CellData(Piece::Rook   ,Color::Black) ,A ,Eight);
      place(CellData(Piece::Knight ,Color::Black) ,B ,Eight);
      place(CellData(Piece::Bishop ,Color::Black) ,C ,Eight);
      place(CellData(Piece::Queen  ,Color::Black) ,D ,Eight);
      place(CellData(Piece::King   ,Color::Black) ,E ,Eight);
      place(CellData(Piece::Bishop ,Color::Black) ,F ,Eight);
      place(CellData(Piece::Knight ,Color::Black) ,G ,Eight);
      place(CellData(Piece::Rook   ,Color::Black) ,H ,Eight);
      for (int i = 0; i < Width; ++i) place(CellData(Piece::Pawn, Color::Black), i, Seven);

      whiteKingPositioin = Position(E, One);
      blackKingPositioin = Position(E, Eight);
    }

    void Model::clear() {
      isWhiteMove = true;
      mCells.clear();
      mCells.resize(Total);
      while(!mSnapshots.empty()) mSnapshots.pop_back();
    }

    void Model::print() const {
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

    std::vector<Position> Model::possibleMoves(const Position& position) {
      std::vector<Position> result;
      CellData chessman = cellDataFrom(position);
      Color ownColor = chessman.color();
      if (ownColor == currentColor()) {
        result = extractPossiblePositions(position, chessman.piece(), ownColor);
      }
      return filtIllegals(result, position, chessman);
    }

    Model::Positions Model::extractPossiblePositions(const Position& position, Piece piece, Color color) {
      switch (piece) {
      case Piece::Pawn   : return extractPawnPossiblePositions  (position, color);
      case Piece::Knight : return extractKnightPossiblePositions(position, color);
      case Piece::Bishop : return extractBishopPossiblePositions(position, color);
      case Piece::Rook   : return extractRookPossiblePositions  (position, color);
      case Piece::Queen  : return extractQueenPossiblePositions (position, color);
      case Piece::King   : return extractKingPossiblePositions  (position, color);
      default: return Positions();
      }
      return Positions();
    }

    Model::Positions Model::extractPawnPossiblePositions(const Position &position, Color ownColor) {
      Positions result;
      int direction = (ownColor != Color::White) ? 1 : -1;

      Position oneStepForward = Position(position.x, position.y + direction * 1);
      if (cellDataFrom(oneStepForward).color() == Color::None) {
        result.push_back(oneStepForward);
        bool canTwoStep = position.y == Two || position.y == Seven;
        Position twoStepForward = Position(position.x, position.y + direction * 2);
        if (canTwoStep && cellDataFrom(twoStepForward).color() == Color::None) {
          result.push_back(twoStepForward);
        }
      }

      Position rightEnemyPos(position.x + direction * 1, position.y + direction * 1);
      Position leftEnemyPos (position.x - direction * 1, position.y + direction * 1);

      Positions right = extractPawnEnemyPosition(rightEnemyPos, ownColor);
      Positions left  = extractPawnEnemyPosition(leftEnemyPos, ownColor);

      result.insert(result.end(), right.begin(), right.end());
      result.insert(result.end(), left.begin(), left.end());

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
      return filtOutsides(result);
    }

    Model::Positions Model::extractPawnEnemyPosition(const Position& pos, Color own) const {
      Positions result;
      CellData enemy = cellDataFrom(pos);
      if (enemy.color() != own && enemy.color() != Color::None) result.push_back(pos);
      return result;
    }

    void Model::addEnpassant(const Position& endEnpassant, const Position &endPos) {
      mEnpassants.push({endEnpassant, endPos});
    }

    void Model::clearEnpassants() {
      while (!mEnpassants.empty()) mEnpassants.pop();
    }

    Color Model::currentColor() const {
      return isWhiteMove ? Color::White : Color::Black;
    }

    Model::Positions Model::filtOutsides(const std::vector<Position>& positions) {
      std::vector<Position> result;
      for (auto i : positions) {
        if (i.y <= One && i.x <= H && i.y >= A && i.x >= Eight) result.push_back(i);
      }
      return result;
    }

    Model::Positions Model::filtIllegals(const Model::Positions& positions, const Position& position, CellData chessman) {
      Positions result;
      for (auto i : positions) {
        if (!isKingUnderCheck(position, i, chessman)) result.push_back(i);
      }
      return result;
    }

    bool Model::isKingUnderCheck(const Position& startPos, const Position& endPos, CellData chessman) {
      bool result = false;
      Snapshot snap = makeSnapshot();

      place(chessman, endPos);
      place(CellData(), startPos);
      Position kingPosition = isWhiteMove ? whiteKingPositioin : blackKingPositioin;
      Color kingColor = chessman.color();
      if (chessman.piece() == Piece::King) {
        kingPosition = endPos;
      }
      Positions queen = extractQueenPossiblePositions(kingPosition, kingColor);

      for (auto i : queen) {
        CellData enemy = cellDataFrom(i);
        Piece enemyPiece = enemy.piece();
        Color enemyColor = enemy.color();
        bool bishop = (enemyPiece == Piece::Bishop);
        bool rook = enemyPiece == Piece::Rook;
        bool q = enemyPiece == Piece::Queen;
        if ((enemyColor != kingColor) && (bishop || rook || q)) {          
          result = true;
          printf("queen\n");
        }
      }

      Positions knight = extractKnightPossiblePositions(kingPosition, kingColor);
      for (auto i : knight) {
        CellData enemy = cellDataFrom(i);
        Piece enemyPiece = enemy.piece();
        Color enemyColor = enemy.color();
        if ((enemyPiece == Piece::Knight) && enemyColor != kingColor) {
          result = true;
          printf("knight\n");
        }
      }

      Positions pawn = extractPawnPossiblePositions(kingPosition, kingColor);
      for (auto i : pawn) {
        CellData enemy = cellDataFrom(i);
        Piece enemyPiece = enemy.piece();
        Color enemyColor = enemy.color();
        if ((enemyPiece == Piece::Pawn) && enemyColor != kingColor) {
          result = true;
          printf("pawn\n");
        }
      }

      Positions king = extractKingPossiblePositions(kingPosition, kingColor);
      for (auto i : king) {
        CellData enemy = cellDataFrom(i);
        Piece enemyPiece = enemy.piece();
        Color enemyColor = enemy.color();
        if ((enemyPiece == Piece::King) && enemyColor != kingColor) {
          result = true;
          printf("King\n");
        }
      }

      mCells = snap.cells;
      return result;
    }


    bool Model::isCheckmate() {
      bool result = true;
      Positions positions;
      for (int i = 0; i < mCells.size(); ++i) {
        if (mCells.at(i).color() == currentColor()) {
          positions.push_back(Position(i % Width, i / Height));
        }
      }

      for (auto i : positions) {
        if (!possibleMoves(i).empty()) return false;
      }

      return result;
    }

    void Model::setPromotionPiece(Piece piece) {
      printf("%d\n", piece);
      if (mIsPromotion) {
        mCells.at(to1d(mPromotionPosition)).setPiece(piece);
        mCells.at(to1d(mPromotionPosition)).piece();
      }
      mIsPromotion = false;
    }

    Model::Positions Model::extractKnightPossiblePositions(const Position &position, Color ownColor) const {
      Positions result;
      std::vector<Position> positions = {
        Position(position.x + 1, position.y + 2)
        ,Position(position.x - 1, position.y + 2)
        ,Position(position.x + 1, position.y - 2)
        ,Position(position.x - 1, position.y - 2)
        ,Position(position.x + 2, position.y + 1)
        ,Position(position.x - 2, position.y + 1)
        ,Position(position.x + 2, position.y - 1)
        ,Position(position.x - 2, position.y - 1)
      };

      for (auto i : positions) {
        Color enemyColor = cellDataFrom(i).color();
        if (enemyColor == Color::None || enemyColor != ownColor) result.push_back(i);
      }
      return filtOutsides(result);
    }

    Model::Positions Model::extractBishopPossiblePositions(const Position& position, Color ownColor) {
      Positions result;
      Positions topRight    = extractPositionsByDirection(position,  1,  1, ownColor);
      Positions bottomRight = extractPositionsByDirection(position, -1,  1, ownColor);
      Positions topLeft     = extractPositionsByDirection(position,  1, -1, ownColor);
      Positions bottomLeft  = extractPositionsByDirection(position, -1, -1, ownColor);
      result.insert(result.end(), topRight.begin(), topRight.end());
      result.insert(result.end(), bottomRight.begin(), bottomRight.end());
      result.insert(result.end(), topLeft.begin(), topLeft.end());
      result.insert(result.end(), bottomLeft.begin(), bottomLeft.end());
      return result;
    }

    Model::Positions Model::extractRookPossiblePositions(const Position& position, Color ownColor) {
      Positions result;
      Positions right = extractPositionsByDirection(position,  0, 1, ownColor);
      Positions left  = extractPositionsByDirection(position,  0,-1, ownColor);
      Positions up    = extractPositionsByDirection(position,  1, 0, ownColor);
      Positions down  = extractPositionsByDirection(position, -1, 0, ownColor);

      result.insert(result.end(), right.begin(), right.end());
      result.insert(result.end(), left.begin(), left.end());
      result.insert(result.end(), up.begin(), up.end());
      result.insert(result.end(), down.begin(), down.end());
      return result;
    }

    Model::Positions Model::extractQueenPossiblePositions(const Position &position, Color ownColor) {
      Positions result;
      Positions bishop = extractBishopPossiblePositions(position, ownColor);
      Positions rook   = extractRookPossiblePositions(position, ownColor);
      result.insert(result.end(), bishop.begin(), bishop.end());
      result.insert(result.end(), rook.begin(), rook.end());
      return result;
    }

    Model::Positions Model::extractKingPossiblePositions(const Position &position, Color ownColor) const {
      std::vector<Position> result;
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
      if (cellDataFrom(castlingRight1).piece() == Piece::None &&
          cellDataFrom(castlingRight2).piece() == Piece::None &&
          cellDataFrom(castlingRight3).piece() == Piece::Rook) {
        if (canWhiteCastlingRight && ownColor == Color::White ||
            canBlackCastlingRight && ownColor == Color::Black) {
          result.push_back(castlingRight2);
        }
      }
      Position castlingLeft1(position.x - 1, position.y);
      Position castlingLeft2(position.x - 2, position.y);
      Position castlingLeft3(position.x - 3, position.y);
      Position castlingLeft4(position.x - 4, position.y);
      if (cellDataFrom(castlingLeft1).piece() == Piece::None &&
          cellDataFrom(castlingLeft2).piece() == Piece::None &&
          cellDataFrom(castlingLeft3).piece() == Piece::None &&
          cellDataFrom(castlingLeft4).piece() == Piece::Rook) {
        if (canWhiteCastlingLeft && ownColor == Color::White ||
            canBlackCastlingLeft && ownColor == Color::Black) {
          result.push_back(castlingLeft2);
        }
      }

      for (auto i : positions) {
        CellData enemy = cellDataFrom(i);
        if (enemy.color() == Color::None || enemy.color() != ownColor) result.push_back(i);
      }
      return filtOutsides(result);
    }

    Model::Positions Model::extractPositionsByDirection(Position position, int directionX, int directionY, Color ownColor) const {
      Positions result;
      for (int i = 1; i <= Width; ++i) {
        Position p = Position(position.x + directionX * i, position.y + directionY * i);
        CellData chessman = cellDataFrom(p);
        Color color = chessman.color();
        if (color == Color::None) result.push_back(p);
        else if (color != ownColor) {
          result.push_back(p);
          break;
        }
        else break;
      }
      return filtOutsides(result);
    }

    bool Model::attemptCastling(const Position &startPos, const Position &endPos, Color color) {
      bool ok = false;
      if (endPos.x < startPos.x) {
        if ((canWhiteCastlingLeft && color == Color::White) ||
            (canBlackCastlingLeft && color == Color::Black)) {
          ok = attemptCastlingLeft(startPos, endPos, color);
        }
      }
      if (endPos.x > startPos.x) {
        if ((canWhiteCastlingRight && color == Color::White) ||
            (canBlackCastlingRight && color == Color::Black) &&
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
          (Piece::None == cellDataFrom(rookEndPos).piece())                 &&
          (Piece::None == cellDataFrom(startPos.x - 2, startPos.y).piece()) &&
          (Piece::None == cellDataFrom(startPos.x - 3, startPos.y).piece()) &&
          (Piece::Rook == cellDataFrom(rookStartPos).piece());
      if (canCastling) castling(startPos, endPos, rookStartPos, rookEndPos, color);
      return canCastling;
    }

    bool Model::attemptCastlingRight(const Position& startPos, const Position& endPos, Color color) {
      Position rookStartPos(startPos.x + 3, startPos.y);
      Position rookEndPos(startPos.x + 1, startPos.y);

      bool canCastling = (cellDataFrom(rookStartPos).piece() == Piece::Rook) &&
                         (cellDataFrom(rookEndPos).piece()   == Piece::None) &&
                         (cellDataFrom(startPos.x + 2, startPos.y).piece() == Piece::None);
      if (canCastling) castling(startPos, endPos, rookStartPos, rookEndPos, color);
      return canCastling;
    }

    void Model::castling(const Position& startPos, const Position& endPos, const Position& rookStartPos, const Position& rookEndPos, Color color) {
      CellData rook(Piece::Rook, color);
      CellData king(Piece::King, color);
      place(king, endPos);
      place(CellData(), startPos);

      place(rook, rookEndPos);
      place(CellData(), rookStartPos);

      if (color == Color::White) canWhiteCastlingRight = canWhiteCastlingRight = false;
      if (color == Color::Black) canBlackCastlingRight = canBlackCastlingRight = false;
    }

    Snapshot Model::makeSnapshot() const {
      return {
        mCells,
            canWhiteCastlingLeft,
            canWhiteCastlingRight,
            canBlackCastlingLeft,
            canBlackCastlingRight
      };
    }

    const std::vector<CellData> Model::getCells() const {
      return mCells;
    }

    bool Model::isPromotion() {
      return mIsPromotion;
    }

    int Model::to1d(const Position& pos) {
      return pos.x + Height * pos.y;
    }

  }
}
