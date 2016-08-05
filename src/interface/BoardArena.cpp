#include "BoardArena.h"

#include "../logic/Global.h"
#include "../logic/Board.h"

Board::Board(): _size(0, 0), _selected(0) {

}

void Board::init(int w, int h) {
    _size.x = w;
    _size.y = h;

    mBoardArea = new ColorRectSprite();
    int x = getStage()->getWidth() / 2;
    int y = getStage()->getHeight() / 2;
    mBoardArea->setPosition(x, y);
    mBoardArea->setSize(x, x);
    mBoardArea->attachTo(getStage());
    mBoardArea->setColor(Color::Wheat);

    drawBlackCells();
    drawChessmans();
}

void Board::drawChessmans() {
    nsChess::Board board;
    board.autoFill();
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
            blackCell->attachTo(mBoardArea);
        }
    }
}

struct swapData: public Object {
    swapData(space* A, space* B): a(A), b(B) {}
    space* a;
    space* b;
};



space* Board::getSpace(const Point& pos, bool check) {

}

void Board::update(const UpdateState& us) {

}

void Board::touched(Event* event) {

}

spActor Board::getView() {
    return mBoardArea;
}

void Board::free() {
    _field.clear();
    mBoardArea->detach();
    mBoardArea = 0;
    _current = 0;
}
