#include "Board.h"
#include "Global.h"

#include <iostream>

namespace nsChess {
Board::Board() {
    mChesscells.resize(Total);
}

void Board::place(const Chesscell& chessman, const Coordinate& pos) {
    mChesscells.at(index(pos)) = chessman;
}

void Board::place(const Chesscell& chessman, int x, int y) {
    place(chessman, Coordinate(x, y));
}

void Board::move(const Coordinate& start, const Coordinate& end) {
    Chesscell chessman = get(start);
    if (chessman.piece() != Empty) {
        place(chessman, end);
        place(Chesscell(), start);
    }
}


Chesscell Board::get(const Coordinate& position) const {
    int pos = index(position);
    if ((0 < pos) && (Total > pos)) return mChesscells.at(pos);
    else                            return Chesscell();
}

std::vector<Chesscell> Board::getChesscells() const {
    return mChesscells;
}

int Board::index(const Coordinate& pos) const {
    return pos.x() + Height * pos.y();
}

}
