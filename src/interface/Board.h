#pragma once
#include <vector>
#include "oxygine-framework.h"

#include "../logic/Model.h"


namespace chess {
class PieceSprite : public oxygine::Sprite {
public:
  PieceSprite() { drag.init(this); }
   oxygine::Draggable drag;
  model::Color getPieceColor() const { return mColor; }
  void setPieceColor(model::Color color) { mColor = color; }
private:
  model::Color mColor;
};

  class Board : public oxygine::Actor {
  public:
    Board();
    void free();
    oxygine::spActor getView();

  private:
    void colorizeCells();
    void createChessmans();
    void createPiece(oxygine::intrusive_ptr<PieceSprite> piece, int position);

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
  };
}

