#include "Board.h"
#include "Global.h"

#include <iostream>

namespace nsChess {
Board::Board() : isWhiteMove(true) {
    mChesscells.resize(Total);
}

void Board::place(const Chesscell& chessman, const Coordinate& pos) {
    mChesscells.at(index(pos)) = chessman;
}

void Board::place(const Chesscell& chessman, int x, int y) {
    place(chessman, Coordinate(x, y));
}

bool Board::move(const Coordinate& start, const Coordinate& end) {
    if (start == end) return false;
    Chesscell chessman = get(start);
    if (chessman.piece() != Empty) {
        if ((chessman.color() == White) == isWhiteMove) {
        place(chessman, end);
        place(Chesscell(), start);
        isWhiteMove = !isWhiteMove;
        return true;
        }
    }
    return false;
}

void Board::autoFill() {
    enum { a, b, c, d, e, f, g, h };
    place(Chesscell(Rook   ,White) ,a ,0);
    place(Chesscell(Knight ,White) ,b ,0);
    place(Chesscell(Bishop ,White) ,c ,0);
    place(Chesscell(Queen  ,White) ,d ,0);
    place(Chesscell(King   ,White) ,e ,0);
    place(Chesscell(Bishop ,White) ,f ,0);
    place(Chesscell(Knight ,White) ,g ,0);
    place(Chesscell(Rook   ,White) ,h ,0);
    for (int i = 0; i < 8; ++i) place(Chesscell(Pawn, White), i, 1);
    place(Chesscell(Rook   ,Black) ,a ,7);
    place(Chesscell(Knight ,Black) ,b ,7);
    place(Chesscell(Bishop ,Black) ,c ,7);
    place(Chesscell(Queen  ,Black) ,d ,7);
    place(Chesscell(King   ,Black) ,e ,7);
    place(Chesscell(Bishop ,Black) ,f ,7);
    place(Chesscell(Knight ,Black) ,g ,7);
    place(Chesscell(Rook   ,Black) ,h ,7);
    for (int i = 0; i < 8; ++i) place(Chesscell(Pawn, Black), i, 6);
}

void Board::print() {
  int num = 1;
  for (int i = 0; i < 64; ++i) {
      if (0 == i % 8) printf("\n %d | ", num++);
      nsChess::Chesscell ch = getChesscells().at(i);
      printf("%d%d ", ch.piece(), ch.color());
  }
  printf("\n");
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
