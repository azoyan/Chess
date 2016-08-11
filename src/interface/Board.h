#pragma once
#include <vector>
#include "oxygine-framework.h"

#include "../logic/Model.h"

using namespace oxygine;
namespace chess {
class Board : public Actor {
public:
    Board();
    void free();
    spActor getView();

private:
    void drawBlackCells();
    void drawChessmans();
    void drawPiece(spSprite piece, int position);

    void doUpdate(const UpdateState& us);
    void onMouseUp(Event* event);
    void onMouseDown(Event* event);
    void onEvent(Event* ev);

    model::Position extractPosition(Vector2 position);
    Vector2 alignToGrid(Vector2 position);
    void cleanBoard();
    double cellWidth();
private:
    Vector2 startPos;
    model::Model* mModel;
    Resources* resources;
    spSprite mView;
    std::vector<spSprite> pieces;
};
}
