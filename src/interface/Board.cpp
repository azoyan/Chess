#include "Board.h"

#include "../logic/Global.h"
#include "Draggable.h"
#include "TweenOutline.h"
#include "shared.h"

class DraggableSprite : public Sprite {
public:
    DraggableSprite() { drag.init(this); }
    Draggable drag;
};

bool touched = false;
namespace chess {
void Board::init(int w, int h) {
    mView = new ColorRectSprite();
    mModel = new model::Model;
    mModel->autoFill();


    resources = new Resources;
    resources->loadXML("/home/i/projects/c++/Chess/res/resurces.xml");

    touched = false;
    mView->setSize(getStage()->getWidth(), getStage()->getHeight());
    mView->setColor(Color::Wheat);
    mView->setTouchEnabled(false, true);
    drawBlackCells();
    drawChessmans();
}

void Board::drawChessmans() {
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
            spSprite piece = new DraggableSprite();
            piece->addEventListener(TouchEvent::TOUCH_DOWN ,CLOSURE(this, &Board::onMouseDown));
            piece->addEventListener(TouchEvent::TOUCH_UP   ,CLOSURE(this, &Board::onMouseUp));            
            pieces.push_back(piece);
            piece->setResAnim(resources->getResAnim("pieces"), column, row);
            piece->setSize(cellWidth(), cellWidth());
            piece->setTouchEnabled(true);
            drawPiece(piece, i);            
            piece->setName("piece");
            piece->attachTo(mView);
            }
        }
}

void Board::drawPiece(spSprite piece, int position) {
    double offsetX = position % model::Width;
    double offsetY = position / model::Height;
    piece->setPosition(mView->getX() + piece->getWidth()  * offsetX,
                       mView->getY() + piece->getHeight() * offsetY);
}

void Board::drawBlackCells() {
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

void Board::doUpdate(const UpdateState& us) {
    if (!touched) drawChessmans();
}

spActor Board::getView() {
    return mView;
}

void Board::onMouseUp(Event* event) {
    spSprite actor = safeSpCast<Sprite>(event->currentTarget);
    spTween t = actor->getTween("color", ep_ignore_error);
    if (t) actor->removeTween(t);
    actor->setColor(Color::White);
    actor->setPosition(alignToGrid(actor->getPosition()));

    model::Position start = extractPosition(startPos);
    model::Position end   = extractPosition(actor->getPosition());
    mModel->move(start, end);
    mModel->print();
    touched = false;
    mView->setCallbackDoUpdate(CLOSURE(this, &Board::doUpdate));
}

void Board::onMouseDown(Event* event) {
    spActor actor = safeSpCast<Actor>(event->currentTarget);
    spTween t = actor->addTween(Sprite::TweenColor(Color::LightGreen), 500, -1, true);
    startPos  = actor->getPosition();
    t->setName("color");
    touched = true;
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
    resources->free();
}
}
