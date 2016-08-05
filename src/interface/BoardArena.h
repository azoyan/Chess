#pragma once
#include <vector>
#include "oxygine-framework.h"
#include <vector>

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

    void free();

    spActor getView();


private:

    space* getSpace(const Point& pos, bool check = true);

    void update(const UpdateState& us);

    void touched(Event*);

    spSprite            mBoardArea;
    space*              _selected;
    spTween             _current;
    std::vector<space>  _field;
    Point               _size;
};
