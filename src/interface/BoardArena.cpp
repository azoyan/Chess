#include "BoardArena.h"

#include "../logic/Global.h"
#include "Draggable.h"
#include "TweenOutline.h"

class DraggableSprite : public Sprite {
public:
    DraggableSprite() { drag.init(this); }
    Draggable drag;
};

Board::Board(): _size(0, 0) {

}


void Board::init(int w, int h) {
    _size.x = w;
    _size.y = h;

    spSprite mBoard = new ColorRectSprite();

    mBoardArea = new ColorRectSprite();

    getStage()->addClickListener(CLOSURE(this, &Board::clicked));
    getStage()->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Board::moved));

    int x = getStage()->getWidth() / 2;
    int y = getStage()->getHeight() / 2;
    mBoardArea->setPosition(x, y);
    mBoardArea->setSize(x, x);
    mBoardArea->attachTo(mBoard);
    mBoardArea->setColor(Color::Wheat);    
    mBoardArea->setTouchEnabled(false, true);

    Vector2 padding(mBoardArea->getSize() / 16);
    mBoard->attachTo(getStage());
    mBoard->setSize(mBoardArea->getSize() + padding);
    mBoard->setPosition(mBoardArea->getX() / 2 - 16, mBoardArea->getY() / 4 - 16);
    mBoard->setColor(Color::DimGray);
    drawBlackCells();
    drawChessmans();
}



void Board::drawChessmans() {
    mBoardModel = new nsChess::Board;
    mBoardModel->autoFill();

    Resources* resources = new Resources;
    resources->loadXML("/home/i/projects/c++/Chess/res/resurces.xml");

    for (int i = 0; i < mBoardModel->getChesscells().size(); ++i) {
        using namespace nsChess;
        int row = -1;
        switch (mBoardModel->getChesscells().at(i).color()) {
        case White: row = 0; break;
        case Black: row = 1; break;
        }
        int column = -1;
        switch(mBoardModel->getChesscells().at(i).piece()) {
        case Pawn:   column = 5; break;
        case Knight: column = 3; break;
        case Bishop: column = 4; break;
        case Rook:   column = 2; break;
        case Queen:  column = 1; break;
        case King:   column = 0; break;
        }
        if (!(row < 0 && column < 0)) {
            spSprite piece = new DraggableSprite();
            piece->setResAnim(resources->getResAnim("pieces"), column, row);
            piece->setSize(60, 60);
            piece->setTouchEnabled(true);
            drawPiece(piece, i);
            printf("%d%d ", mBoardModel->getChesscells().at(i).piece(), mBoardModel->getChesscells().at(i).color());
        }

    }
}

void Board::drawPiece(spSprite piece, int position) {
    using namespace nsChess;
    int offsetX = position % Width;
    int offsetY = position / Height;
    double coordX = mBoardArea->getX() - mBoardArea->getWidth();
    double coordY = coordX;
    piece->setPosition(coordX + piece->getWidth()  * offsetX,
                       coordY + piece->getHeight() * offsetY);
    piece->attachTo(mBoardArea);
}

void Board::drawBlackCells() {
    double coordX = mBoardArea->getX() - mBoardArea->getWidth();
    double coordY = coordX;

    for (int i = 0; i < nsChess::Height; ++i) {
        for (int j = 0; j < nsChess::Width ; j += 2) {
            bool black = true;
            spSprite blackCell = new ColorRectSprite();
            blackCell->setSize(mBoardArea->getSize() / nsChess::Width);

            double posX = coordX + blackCell->getWidth()  * i;
            double posY = coordY + blackCell->getHeight() * j;

            if (black && (i % 2)) {
                posY += blackCell->getHeight();
                black = !black;
            }

            blackCell->setPosition(posX, posY);
            blackCell->setColor(Color::Tan);
            blackCell->setTouchEnabled(false);
            blackCell->attachTo(mBoardArea);            
        }
    }
}


void Board::update(const UpdateState& us) {
    PointerState* ps = Input::instance.getTouchByIndex(1);
    if( ps->isPressed()) {
        Vector2 pos = ps->getPosition();
    }
}

void Board::touched(Event* event) {

}

spActor Board::getView() {
    return mBoardArea;
}

void Board::clicked(Event* e) {
    log::messageln("%d %d ", Input::instance.getTouchByIndex(1)->getPosition().x,
                             Input::instance.getTouchByIndex(1)->getPosition().y);
}

void Board::moved(Event* e) {
}

void Board::free() {
    mBoardArea->detach();
    mBoardArea = 0;
}
