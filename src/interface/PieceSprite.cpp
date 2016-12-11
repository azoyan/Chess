#include "PieceSprite.h"

chess::PieceSprite::PieceSprite(oxygine::Resources* resources, chess::model::Piece piece, chess::model::Color color, bool draggable)
: mPiece(piece), mColor(color), mResources(resources) {
  setResAnim(mResources->getResAnim("pieces"),static_cast<int>(mPiece),static_cast<int>(mColor));
  if (draggable) drag.init(this);
}

chess::PieceSprite::PieceSprite(oxygine::Resources* resources, chess::model::CellData cellData, bool draggable)
: PieceSprite(resources, cellData.piece(),cellData.color(), draggable) { }

chess::model::Color chess::PieceSprite::getPieceColor() const { return mColor; }

void chess::PieceSprite::setPieceColor(chess::model::Color color) {
  mColor = color;
}

