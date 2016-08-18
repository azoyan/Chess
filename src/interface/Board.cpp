#include "Board.h"

#include "../logic/Global.h"
#include "Draggable.h"
#include "TweenOutline.h"

#include "core/oxygine.h"
#include "SDL.h"
#include "SDL_keyboard.h"

using namespace oxygine;


namespace chess {


  Board::Board()
  : mView     (new ColorRectSprite())
  , mModel    (new model::Model)
  , mResources(new Resources())
  {
    mResources->loadXML("data/resources.xml");
    mView->setSize(getStage()->getWidth(), getStage()->getHeight());
    mView->setColor(Color::Tan);
    mView->setTouchEnabled(false, true);
    oxygine::core::getDispatcher()->addEventListener(oxygine::core::EVENT_SYSTEM
                                                    ,CLOSURE(this, &Board::onEvent));
    colorizeCells();
    createChessmans();
  }

  void Board::createChessmans() {
      cleanBoard();
      model::Color color;
      for (int i = 0; i < mModel->getCells().size(); ++i) {
        using namespace model;
        int row = -1;
        switch (mModel->getCells().at(i).color()) {
        case White: row = 1; color = White; break;
        case Black: row = 0; color = Black; break;
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
          intrusive_ptr<PieceSprite> piece = new PieceSprite();

          piece->addEventListener(TouchEvent::TOUCH_DOWN ,CLOSURE(this, &Board::onMouseDown));
          piece->addEventListener(TouchEvent::TOUCH_UP   ,CLOSURE(this, &Board::onMouseUp));
          pieces.push_back(piece);
          piece->setResAnim(mResources->getResAnim("pieces"), column, row);
          piece->setSize(cellWidth(), cellWidth());
          piece->setTouchEnabled(true);
          piece->setPieceColor(color);
          createPiece(piece, i);
          piece->setName("piece");

          piece->attachTo(mView);
        }        
     }
  }

  void Board::createPiece(intrusive_ptr<PieceSprite> piece, int position) {
    double offsetX = position % model::Width;
    double offsetY = position / model::Height;
    double width  = piece->getWidth();
    double height = piece->getHeight();
    piece->setPosition(mView->getX() + width  * offsetX,
                       mView->getY() + height * offsetY);

    bool isWhiteMove = mModel->isWhiteMove;
    model::Color current = piece->getPieceColor();

    bool isDraggable = current == model::White && isWhiteMove
                    || current == model::Black && !isWhiteMove;

    piece->drag.setDragEnabled(isDraggable);

    if (isDraggable) {
      spTweenQueue tweenQueue = new TweenQueue();
      tweenQueue->add(Sprite::TweenRotationDegrees(-1), 100, 1, true);
      tweenQueue->add(Sprite::TweenRotationDegrees( 1), 100, 1, true);
      tweenQueue->setLoops(-1);
      piece->addTween(tweenQueue);
    }
  }

  void Board::colorizeCells() {
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
        blackCell->setColor(Color::Wheat);
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
    actor->setScale(1);

    model::Position start = extractPosition(mStartPos);
    model::Position end   = extractPosition(actor->getPosition());
    mModel->move(start, end);
    createChessmans();    

    while(mView->getChild("highlight").get() != nullptr) {
      mView->removeChild(mView->getChild("highlight"));
    }
  }

  void Board::onMouseDown(Event* event) {
    spActor actor = safeSpCast<Actor>(event->currentTarget);
    mStartPos  = actor->getPosition();
    std::vector<model::Position> highlights = mModel->possibleMoves(extractPosition(mStartPos));
    if (!highlights.empty()) {
      //spTween tween = actor->addTween(Sprite::TweenColor(Color::LightGreen), 500, -1, true);
      actor->setScale(1.1);
      //tween->setName("color");
    }

    for (auto i : highlights) {
      spSprite highlight = new ColorRectSprite();
      highlight->setSize(actor->getWidth(), actor->getHeight());
      highlight->setColor(Color::LightGreen);
      highlight->setPosition(extractCoordinate(i).x
                            ,extractCoordinate(i).y);
      highlight->setAlpha(96);
      highlight->setName("highlight");
      spTween tween = highlight->addTween(Sprite::TweenColor(Color::Green), 2500, -1, true);
      highlight->attachTo(mView);
    }
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
    result.x = floor((position.x + halfCellWidth()) / cellWidth()) * cellWidth();
    result.y = floor((position.y + halfCellWidth()) / cellWidth()) * cellWidth();
    return result;
  }

  void Board::cleanBoard() {
    for (auto i : pieces) mView->removeChild(mView->getChild("piece"));
    pieces.clear();
  }

  double Board::cellWidth() {
      return mView->getWidth() / model::Width;
  }

  double Board::halfCellWidth() {
    return cellWidth() / 2.0;
  }

  model::Position Board::extractPosition(const Vector2& position) {
      return model::Position (position.x / cellWidth(), position.y / cellWidth());
  }

  Vector2 Board::extractCoordinate(const model::Position &position) {
      Vector2 result;
      result.x = position.x * cellWidth();
      result.y = position.y * cellWidth();
      return result;
  }


  void Board::free() {
    mView->detach();
    mView = 0;
    mResources->free();
  }
}
