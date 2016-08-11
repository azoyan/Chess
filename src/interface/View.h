#pragma once
#include <vector>
#include "oxygine-framework.h"

#include "../logic/Model.h"

using namespace oxygine;

class View : public ColorRectSprite {
public:    
    void init(model::Model* model);
    std::vector<spSprite> pieces;
    void drawChessmans();
    void showModel();
private:
    double cellWidth();
    void drawBlackCells();
    void drawPiece(spSprite piece, int position);
private:
    model::Position extractPosition(Vector2 position);
    model::Model* mModel;
    Resources* resources;

public:
    void setTouchDownCallback(const EventCallback& callback);

private:

    EventCallback touchDownCallback;
};
