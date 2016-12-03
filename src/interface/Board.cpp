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
  , mWaitNextClick(false)
  {
    mResources->loadXML("data/resources.xml");
    mView->setSize(getStage()->getWidth() / 2, getStage()->getWidth() / 2);
    mView->setColor(Color::Tan);
    mView->setTouchEnabled(false, true);
    //    mView->setRotationDegrees(45);
    mView->setAnchor(0.5f, 0.5f);
    oxygine::core::getDispatcher()->addEventListener(oxygine::core::EVENT_SYSTEM
                                                     ,CLOSURE(this, &Board::onEvent));
    colorizeCells();
    createChessmans();
    mView->setPosition(0.5f * getStage()->getWidth(), 0.5f * getStage()->getHeight());
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
        intrusive_ptr<PieceSprite> piece = new PieceSprite(mModel->getCells().at(i).piece(), mModel->getCells().at(i).color());

        piece->addEventListener(TouchEvent::TOUCH_DOWN ,CLOSURE(this, &Board::onMouseDown));
        piece->addEventListener(TouchEvent::TOUCH_UP   ,CLOSURE(this, &Board::onMouseUp));
        pieces.push_back(piece);
        piece->setResAnim(mResources->getResAnim("pieces"), column, row);
        piece->setSize(cellWidth(), cellWidth());
        piece->setTouchEnabled(true);
        piece->setPieceColor(color);
        createPiece(piece, i); //rename to "placePiece"
        piece->setName("piece");
        piece->attachTo(mView);
      }
    }
  }

  void Board::createPiece(intrusive_ptr<PieceSprite> piece, int position) {
    double offsetX = position % model::Width;
    double offsetY = position / model::Height;
    double width  = halfCellWidth() + piece->getWidth()  * offsetX;
    double height = halfCellWidth() + piece->getHeight() * offsetY;
    piece->setPosition(width, height);

    bool isWhiteMove = mModel->isWhiteMove;
    model::Color current = piece->getPieceColor();

    bool isDraggable = current == model::White && isWhiteMove
                       || current == model::Black && !isWhiteMove;

    piece->drag.setDragEnabled(isDraggable);
    piece->setAnchor(0.5f, 0.5f);
    piece->setPriority(254);
  }

  void Board::colorizeCells() {
    for (int i = 0; i < model::Height; ++i) {
      for (int j = 0; j < model::Width ; j += 2) {
        bool black = true;
        spSprite blackCell = new ColorRectSprite();
        blackCell->setSize(cellWidth(), cellWidth());

        double posX = halfCellWidth() + blackCell->getWidth()  * i;
        double posY = halfCellWidth() + blackCell->getHeight() * j;

        if (black && (i % model::Black)) {
          posY += blackCell->getHeight();
          black = !black;
        }
        blackCell->setPosition(posX, posY);
        blackCell->setAnchor(0.5f, 0.5f);
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
    actor->setScale(1);
    actor->setPosition(alignToGrid(actor->getPosition()));

    model::Position start = extractPosition(mStartPos);
    model::Position end   = extractPosition(actor->getPosition());

    if (!mWaitNextClick && start == end) {
      mWaitNextClick = true;
      return;
    }
    else {
      mModel->move(start, end);
      if (mModel->isPromotion()) showPromotionMenu();
      createChessmans();
      highlightLastMove((start), (end));
      checkSlatecheck();
      mWaitNextClick = false;
    }



  }

  void Board::onMouseDown(Event* event) {
    spActor actor = safeSpCast<Actor>(event->currentTarget);
    mStartPos  = actor->getPosition();
    std::vector<model::Position> highlights = mModel->possibleMoves(extractPosition(mStartPos));

    if (!highlights.empty()) {
      actor->setScale(1.1);
    }

    for (auto i : highlights) {
      spSprite highlight = new ColorRectSprite();
      highlight->setSize(actor->getWidth() * 0.125f, actor->getHeight() * 0.125f);
      highlight->setAnchor(0.5f, 0.5f);
      highlight->setColor(Color::Teal);
      highlight->setRotationDegrees(45);
      highlight->setPosition(halfCellWidth() + extractCoordinate(i).x
                             ,halfCellWidth() + extractCoordinate(i).y);
      //        highlight->setAlpha(96);
      highlight->setName("highlight");
      highlight->attachTo(mView);
    }    
  }

  void Board::onEvent(Event* ev) {
    SDL_Event* event = (SDL_Event*)ev->userData;
    if (event->type == SDL_KEYDOWN) {
      switch (event->key.keysym.sym) {
      case SDLK_n    : mModel->autoFill(); break;
      case SDLK_LEFT : mModel->undo();     break;
      }
      createChessmans();
    }
//    showPromotionMenu();
  }

  Vector2 Board::alignToGrid(Vector2 position) {
    Vector2 result;
    result.x = floor((position.x  ));
    result.y = floor((position.y  ));
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

  void Board::showLastMove(Vector2 startPos, Vector2 endPos) {

  }

  void Board::showPromotionMenu() {




    spSprite promotionMenu = new ColorRectSprite();
    promotionMenu->setAlpha(225);
    promotionMenu->setColor(Color::White);
    promotionMenu->setSize(mView->getSize());
    promotionMenu->setAnchor(mView->getAnchor());
    promotionMenu->setPosition(mView->getPosition());




    float height = promotionMenu->getPosition().y - promotionMenu->getHeight() / 3;

    model::Color color = mModel->isWhiteMove ? model::White : model::Black;
    int row = mModel->isWhiteMove ? 0 : 1;

    intrusive_ptr<PieceSprite> knight = new PieceSprite(model::Knight, color);
    knight->setResAnim(mResources->getResAnim("pieces"), 1 + model::Knight, row);
    knight->setPosition(promotionMenu->getPosition().x - (promotionMenu->getWidth()  / 2 + knight->getWidth() * 2),
                        height);

    knight->addClickListener(CLOSURE(this, &Board::choose));

    intrusive_ptr<PieceSprite> bishop = new PieceSprite(model::Bishop, color);
    bishop->setResAnim(mResources->getResAnim("pieces"), 1 + model::Bishop, row);
    bishop->setPosition(promotionMenu->getPosition().x - promotionMenu->getWidth()  / 2 - bishop->getWidth(),
                        height);

    bishop->addClickListener(CLOSURE(this, &Board::choose));

    intrusive_ptr<PieceSprite> rook = new PieceSprite(model::Rook, color);
    rook->setResAnim(mResources->getResAnim("pieces"), 2, row);
    rook->setPosition(promotionMenu->getPosition().x - promotionMenu->getWidth()  / 2,
                        height);

    rook->addClickListener(CLOSURE(this, &Board::choose));

    intrusive_ptr<PieceSprite> queen = new PieceSprite(model::Queen, color);
    queen->setResAnim(mResources->getResAnim("pieces"), 1, row);
    queen->setPosition(promotionMenu->getPosition().x - promotionMenu->getWidth()  / 2 + queen->getWidth(),
                       height);

    queen->addClickListener(CLOSURE(this, &Board::choose));

    knight->drag.setDragEnabled(false);
    bishop->drag.setDragEnabled(false);
    rook->drag.setDragEnabled(false);
    queen->drag.setDragEnabled(false);

    promotionMenu->addChild(bishop);
    promotionMenu->addChild(knight);
    promotionMenu->addChild(queen);
    promotionMenu->addChild(rook);

//    mModel->setPromotionPiece(model::Queen);
//    mView->addChild(promotionMenu);
    promotionMenu->setName("promotion");
    promotionMenu->attachTo(mView);
//    mView->removeChild(mView->getChild("promotion"));
  }

void Board::choose(Event* event) {
  intrusive_ptr<PieceSprite> actor = safeSpCast<PieceSprite>(event->currentTarget);

  printf("%d %d\n", actor->mPiece ,actor->mColor);
  mModel->setPromotionPiece(actor->mPiece);


  createChessmans();
  while(mView->getChild("promotion").get() != nullptr) {
    mView->removeChild(mView->getChild("promotion"));
  }

}

  void Board::highlightLastMove(model::Position start, model::Position end) {
    mView->removeChild(mView->getChild("lastMove1"));
    mView->removeChild(mView->getChild("lastMove2"));

    spColorRectSprite lastMoveSprite1 = new ColorRectSprite();
    lastMoveSprite1->setColor(Color::GreenYellow);
    lastMoveSprite1->setAlpha(164);
    lastMoveSprite1->setSize(cellWidth(), cellWidth());
    lastMoveSprite1->setPosition(extractCoordinate(start));
    lastMoveSprite1->setName("lastMove1");
    mView->addChild(lastMoveSprite1);

    spColorRectSprite lastMoveSprite2 = new ColorRectSprite();
    lastMoveSprite2->setColor(Color::Green);
    lastMoveSprite2->setAlpha(128);
    lastMoveSprite2->setSize(cellWidth(), cellWidth());

    lastMoveSprite2->setPosition(extractCoordinate(end));
    lastMoveSprite2->setName("lastMove2");

    mView->addChild(lastMoveSprite2);
    while(mView->getChild("highlight").get() != nullptr) {
      mView->removeChild(mView->getChild("highlight"));
    }
  }

  void Board::checkSlatecheck() {
    if (mModel->isCheckmate()) {
      spColorRectSprite textField = new ColorRectSprite();
      textField->setSize(mView->getSize());
      textField->setPosition(mView->getPosition() * 2);
      textField->setColor(Color::Red);
      textField->setAnchor(0.5f, 0.5f);
      textField->setAlpha(100);
      spTextField text = new TextField();
      text->setText("CHECKMATE");
      textField->addChild(text);
      textField->setPriority(255);
      text->setPosition(textField->getPosition() * 1.5);
      TextStyle* styl = new TextStyle();
      styl->color = Color::Black;
      styl->fontSize = 18;
      text->setStyle(*styl);
      mView->addChild(text);
    }
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
