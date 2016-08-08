#pragma once
#include <vector>
#include "oxygine-framework.h"

#include "../logic/Board.h"

using namespace oxygine;

struct space {
    Point pos;

};

class Board : public Actor {
public:
    void init(int w, int h);
    void drawBlackCells();
    void drawChessmans();

    void drawPiece(spSprite piece, int position);

    void free();

    spActor getView();

private:
    void doUpdate(const UpdateState& us);
    void onMouseUp(Event* event);
    void onMouseDown(Event* event);
    Vector2 alignToGrid(Vector2 position);

    nsChess::Board* mBoardModel;

    spSprite mBoardArea;
    std::vector<spSprite> pieces;
};
