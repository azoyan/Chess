#pragma once
#include <vector>
#include "oxygine-framework.h"

#include "../logic/Board.h"

using namespace oxygine;

struct space {
    Point pos;

};

class Board {
public:
    Board();

    void init(int w, int h);
    void drawBlackCells();
    void drawChessmans();

    void drawPiece(spSprite piece, int position);

    void free();

    spActor getView();


private:
    void update(const UpdateState& us);

    void touched(Event*);
    nsChess::Board* mBoardModel;

    spSprite            mBoardArea;
    Point               _size;
};
