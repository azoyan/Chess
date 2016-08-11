#include "oxygine-framework.h"
#include "Board.h"

using namespace oxygine;
chess::Board* board;

void chess_preinit() {

}

void chess_init() {
    board = new chess::Board();
    spActor view = board->getView();
    getStage()->addChild(view);
}

void chess_destroy() {
    board->free();
}

void chess_update() { }
