#include "oxygine-framework.h"
#include "Board.h"


chess::Board* board;

void chess_preinit() { }

void chess_init() {
  board = new chess::Board();
  oxygine::spActor view = board->getView();
  oxygine::getStage()->addChild(view);
}

void chess_destroy() {
  board->free();
}

void chess_update() { }
