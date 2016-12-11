#pragma once
#include <vector>
#include "oxygine-framework.h"

#include "../logic/Model.h"
#include "PieceSprite.h"

namespace chess {
  class Board : public oxygine::Actor {
  public:
    Board();
    void free();
    oxygine::spActor getView();
  private:
    void colorizeCells();
    void createChessmans();
    void placeChessman(oxygine::intrusive_ptr<PieceSprite> piece, int position);

    void onMouseUp(oxygine::Event* event);
    void onMouseDown(oxygine::Event* event);
    void onEvent(oxygine::Event* ev);

    model::Position extractPosition(const oxygine::Vector2& position);
    oxygine::Vector2 extractCoordinate(const model::Position& position);
    oxygine::Vector2 alignToGrid(oxygine::Vector2 position);
    void cleanBoard();
    double cellWidth();
    double halfCellWidth();
  private:
    oxygine::Vector2 mStartPos;
    model::Model* mModel;
    oxygine::Resources* mResources;
    oxygine::spSprite mView;
    std::vector<oxygine::spSprite> pieces;
    void showLastMove(oxygine::Vector2 startPos, oxygine::Vector2 endPos);
    void showPromotionMenu();
    void choose(oxygine::Event* event);
    void highlightLastMove(model::Position start, model::Position end);
    void checkSlatecheck();
    bool mWaitNextClick;
  };
}

