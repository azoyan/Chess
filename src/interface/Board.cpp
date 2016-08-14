#include "Board.h"

#include "../logic/Global.h"
#include "Draggable.h"
#include "TweenOutline.h"

#include "core/oxygine.h"
#include "SDL.h"
#include "SDL_keyboard.h"

using namespace oxygine;

class PieceSprite : public Sprite {
public:
    PieceSprite() { drag.init(this); }
    Draggable drag;
};

namespace chess {
  Board::Board()
  : mView     (new ColorRectSprite())
  , mModel    (new model::Model)
  , mResources(new Resources())
  {
    mResources->loadXML("data/resources.xml");
    mView->setSize(getStage()->getWidth(), getStage()->getHeight());
    mView->setColor(Color::Wheat);
    mView->setTouchEnabled(false, true);
    oxygine::core::getDispatcher()->addEventListener(oxygine::core::EVENT_SYSTEM
                                                    ,CLOSURE(this, &Board::onEvent));
    createBlackCells();
    createChessmans();
  }

  void Board::createChessmans() {
      cleanBoard();
      for (int i = 0; i < mModel->getCells().size(); ++i) {
        using namespace model;
        int row = -1;
        switch (mModel->getCells().at(i).color()) {
        case White: row = 1; break;
        case Black: row = 0; break;
        }
        int column = -1;
        switch(mModel->getCells().at(i).piece()) {
        case Pawn:   column = 5; break;
        case Knight: column = 3; break;
        case Bishop: column = 4; break;
        case Rook:   column = 2; break;
        case Queen:  column = 1; break;
        case King:   column = 0; break;
        }
        if ((row >= 0 && column >= 0)) {
          spSprite piece = new PieceSprite();
          piece->addEventListener(TouchEvent::TOUCH_DOWN ,CLOSURE(this, &Board::onMouseDown));
          piece->addEventListener(TouchEvent::TOUCH_UP   ,CLOSURE(this, &Board::onMouseUp));
          pieces.push_back(piece);
          piece->setResAnim(mResources->getResAnim("pieces"), column, row);
          piece->setSize(cellWidth(), cellWidth());
          piece->setTouchEnabled(true);
          createPiece(piece, i);
          piece->setName("piece");
          piece->attachTo(mView);
        }
     }
  }

  void Board::createPiece(spSprite piece, int position) {
    double offsetX = position % model::Width;
    double offsetY = position / model::Height;
    piece->setPosition(mView->getX() + piece->getWidth()  * offsetX,
                       mView->getY() + piece->getHeight() * offsetY);
  }

  void Board::createBlackCells() {
    for (int i = 0; i < model::Height; ++i) {
      for (int j = 0; j < model::Width ; j += 2) {
        bool black = true;
        spSprite blackCell = new ColorRectSprite();
        blackCell->setSize(cellWidth(), cellWidth());

        double posX = mView->getX() + blackCell->getWidth()  * i;
        double posY = mView->getY() + blackCell->getHeight() * j;

        if (black && (i % model::Black)) {
            posY += blackCell->getHeight();
            black = !black;
        }

        blackCell->setPosition(posX, posY);
        blackCell->setColor(Color::Tan);
        blackCell->setTouchEnabled(false);
        blackCell->attachTo(mView);
      }
    }
  }

  spActor Board::getView() {
    return mView;
  }

  void Board::onMouseUp(Event* event) {
    spSprite actor = safeSpCast<Sprite>(event->currentTarget);
    spTween tween = actor->getTween("color", ep_ignore_error);
    if (tween) actor->removeTween(tween);
    actor->setColor(Color::White);
    actor->setPosition(alignToGrid(actor->getPosition()));

    model::Position start = extractPosition(mStartPos);
    model::Position end   = extractPosition(actor->getPosition());
    mModel->move(start, end);
    mModel->print();
    createChessmans();
  }

  void Board::onMouseDown(Event* event) {
    spActor actor = safeSpCast<Actor>(event->currentTarget);
    spTween tween = actor->addTween(Sprite::TweenColor(Color::LightGreen), 500, -1, true);
    mStartPos  = actor->getPosition();
    tween->setName("color");    
  }

  void Board::onEvent(Event* ev) {
    SDL_Event* event = (SDL_Event*)ev->userData;
    if (event->type == SDL_KEYDOWN) {
      switch (event->key.keysym.sym) {
      case SDLK_n    :mModel->autoFill(); break;
      case SDLK_LEFT :mModel->undo();     break;
      }
    createChessmans();
    }
  }

  Vector2 Board::alignToGrid(Vector2 position) {
    Vector2 result;
    result.x = floor((position.x + cellWidth() / 2) / cellWidth()) * cellWidth();
    result.y = floor((position.y + cellWidth() / 2) / cellWidth()) * cellWidth();
    return result;
  }

  void Board::cleanBoard() {
    for (auto i : pieces) mView->removeChild(mView->getChild("piece"));
    pieces.clear();
  }

  double Board::cellWidth() {
    return mView->getWidth() / model::Width;
  }

  model::Position Board::extractPosition(Vector2 position) {
    return model::Position (position.x / cellWidth(), position.y / cellWidth());
  }

  void Board::free() {
    mView->detach();
    mView = 0;
    mResources->free();
  }
}
