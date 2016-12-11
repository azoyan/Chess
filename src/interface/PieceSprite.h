#ifndef PIECESPRITE_H
#define PIECESPRITE_H

#include "core/oxygine.h"
#include "Draggable.h"
#include "../logic/Model.h"
#include <oxygine-framework.h>

namespace chess {
class PieceSprite : public oxygine::Sprite {
public:
  PieceSprite(oxygine::Resources* resources, model::Piece piece, model::Color color, bool draggable = true);
  PieceSprite(oxygine::Resources* resources, model::CellData cellData, bool draggable = true);
  oxygine::Draggable drag;
  model::Color getPieceColor() const;
  void setPieceColor(model::Color color);
public:
  model::Color mColor;
  model::Piece mPiece;
  oxygine::Resources* mResources;
};

}
#endif // PIECESPRITE_H
