#include "View.h"
#include "shared.h"

class DraggableSprite : public Sprite {
public:
    DraggableSprite() { drag.init(this); }
    Draggable drag;
};

void View::init(model::Model* model) {
    mModel = model;
    resources = new Resources();
    resources->loadXML("/home/i/projects/c++/Chess/res/resurces.xml");
}

void View::drawBlackCells() {
    double coordX = this->getX() - this->getWidth();
    double coordY = coordX;

    for (int i = 0; i < model::Height; ++i) {
        for (int j = 0; j < model::Width ; j += 2) {
            bool black = true;
            spSprite blackCell = new ColorRectSprite();
            blackCell->setSize(getSize() / model::Width);

            double posX = coordX + blackCell->getWidth()  * i;
            double posY = coordY + blackCell->getHeight() * j;

            if (black && (i % 2)) {
                posY += blackCell->getHeight();
                black = !black;
            }
            blackCell->setPosition(posX, posY);
            blackCell->setColor(Color::Tan);
            blackCell->setTouchEnabled(false);
            blackCell->attachTo(this);
        }
    }
}

void View::drawChessmans() {
    assert(mModel != nullptr);
    for (int i = 0; i < mModel->getCells().size(); ++i) {
        int row = -1;
        switch (mModel->getCells().at(i).color()) {
        case model::White: row = 1; break;
        case model::Black: row = 0; break;
        }
        int column = -1;
        switch(mModel->getCells().at(i).piece()) {
        case model::Pawn:   column = 5; break;
        case model::Knight: column = 3; break;
        case model::Bishop: column = 4; break;
        case model::Rook:   column = 2; break;
        case model::Queen:  column = 1; break;
        case model::King:   column = 0; break;
        }
        if (!(row < 0 && column < 0)) {
            spSprite piece = new DraggableSprite();
            piece->setResAnim(resources->getResAnim("pieces"), column, row);

            piece->addTouchDownListener(touchDownCallback);
            piece->setSize(getSize() / model::Width);
            piece->setTouchEnabled(true);
            drawPiece(piece, i);
            piece->setName("piece");
            piece->attachTo(this);
        }
    }
}

void View::showModel() {
    drawBlackCells();
    drawChessmans();
}

void View::drawPiece(spSprite piece, int position) {
    using namespace model;
    double offsetX = position % Width;
    double offsetY = position / Height;
    double coordX = getX() - getWidth();
    double coordY = coordX;
    piece->setPosition(coordX + piece->getWidth()  * offsetX,
                       coordY + piece->getHeight() * offsetY);
}

void View::setTouchDownCallback(const EventCallback &callback) {
    touchDownCallback = callback;
}

double View::cellWidth(){
    return this->getWidth() / model::Width;
}

