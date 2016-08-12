#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "CellData.h"
#include <stack>

namespace chess {
  namespace model {

  struct Snapshot {
      Position startPos;
      CellData startData;
      Position endPos;
      CellData endData;
  };

    class Model {
    public:
      Model();
    public:
      void place(const CellData& chessman, const Position& pos);
      void place(const CellData& chessman, int x, int y);
      void move(const Position& startPos,   const Position& endPos);
      void undo();

      void print();
      void autoFill();
      void clear();
      int to1d(const Position& pos) const;
      const std::vector<CellData> getCells() const;
    private:
      CellData cellDataFrom(const Position& position) const;
    private:
      std::vector<CellData> mCells;
      bool isWhiteMove;
      std::stack<Snapshot> history;
    };
  }
}
#endif
