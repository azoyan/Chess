#include "oxygine-framework.h"

#include "BoardArena.h"
#include "shared.h"

using namespace oxygine;


Board board;

void chess_preinit() { }

void chess_init() {
    //load xml file with resources definition
    res.loadXML("resources.xml");

    spSprite bg = new Sprite;
    bg->setResAnim(res.getResAnim("bg"));
    bg->attachTo(getStage());

    board.init(6, 5);

    spActor view = board.getView();
    view->setPosition(getStage()->getSize() / 2 - view->getSize() / 2);
    getStage()->addChild(view);
}

void chess_destroy() {
    board.free();
    res.free();
}

void chess_update() { }