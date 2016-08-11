#pragma once
#include <vector>
#include "oxygine-framework.h"

#include "../logic/Model.h"


namespace chess {
  class Board : public oxygine::Actor {
  public:
    Board();
    void free();
    oxygine::spActor getView();

  private:
    void createBlackCells();
    void createChessmans();
    void createPiece(oxygine::spSprite piece, int position);

    void doUpdate(const oxygine::UpdateState& us);
    void onMouseUp(oxygine::Event* event);
    void onMouseDown(oxygine::Event* event);
    void onEvent(oxygine::Event* ev);

    model::Position extractPosition(oxygine::Vector2 position);
    oxygine::Vector2 alignToGrid(oxygine::Vector2 position);
    void cleanBoard();
    double cellWidth();
  private:
    oxygine::Vector2 startPos;
    model::Model* mModel;
    oxygine::Resources* resources;
    oxygine::spSprite mView;
    std::vector<oxygine::spSprite> pieces;
    bool mTouched;
  };
}

