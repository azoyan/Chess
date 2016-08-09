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


    void cleanBoard();

private:
    double cellWidth();
    nsChess::Coordinate extractCoordinate(Vector2 position);
    Vector2 startPos;

    int tmpCorrector;

    nsChess::Board* mBoardModel;
    Resources* resources;
    spSprite mBoard;
    spSprite mBoardArea;
    std::vector<spSprite> pieces;
    spSprite lay;
    Vector2 tmpPos;
};
