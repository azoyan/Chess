#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "CellData.h"

namespace chess {
  namespace model {
    class Model {
    public:
        Model();
    public:
      void place(const CellData& chessman, const Position& pos);
      void place(const CellData& chessman, int x, int y);
      void move(const Position& startPos,   const Position& endPos);

      void print();
      void autoFill();
      void clear();
      int to1d(const Position& pos) const;
      const std::vector<CellData> getCells() const;
    private:
      CellData cellDataFrom(const Position &position) const;
    private:
      std::vector<CellData> mCells;
      bool isWhiteMove;
    };
  }
}
#endif
