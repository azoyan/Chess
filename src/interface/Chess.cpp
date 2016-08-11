#include "oxygine-framework.h"

#include "Board.h"
#include "shared.h"

using namespace oxygine;


chess::Board board;

void chess_preinit() { }

void chess_init() {
    //load xml file with resources definition
    board.init(2, 2);
    res.loadXML("/home/i/projects/c++/Chess/res/resurces.xml");
    spActor view = board.getView();
    view->setPosition(0, 0);
    getStage()->addChild(view);

}

void chess_destroy() {
    board.free();
    res.free();
}

void chess_update() { }
