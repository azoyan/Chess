#include <iostream>

#include "logic/Board.h"
#include "logic/Global.h"
#include "logic/Chesscell.h"

#include "oxygine-framework.h"

using namespace std;
using namespace nsChess;

void print(nsChess::Board board) {
  for (int i = 0; i < 64; ++i) {
      if (0 == i % 8) cout << "\n";
      nsChess::Chesscell ch = board.getChesscells().at(i);
      cout << ch.piece() << ch.color() << " ";
  }
  std::cout << "\n";
}

enum { a, b, c, d, e, f, g, h };

void fill(nsChess::Board& board) {
    board.place(Chesscell(Rook   ,White) ,a ,0);
    board.place(Chesscell(Knight ,White) ,b ,0);
    board.place(Chesscell(Bishop ,White) ,c ,0);
    board.place(Chesscell(Queen  ,White) ,d ,0);
    board.place(Chesscell(King   ,White) ,e ,0);
    board.place(Chesscell(Bishop ,White) ,f ,0);
    board.place(Chesscell(Knight ,White) ,g ,0);
    board.place(Chesscell(Rook   ,White) ,h ,0);
    for (int i = 0; i < 8; ++i) board.place(Chesscell(Pawn, White), i, 1);
    board.place(Chesscell(Rook   ,Black) ,a ,7);
    board.place(Chesscell(Knight ,Black) ,b ,7);
    board.place(Chesscell(Bishop ,Black) ,c ,7);
    board.place(Chesscell(Queen  ,Black) ,d ,7);
    board.place(Chesscell(King   ,Black) ,e ,7);
    board.place(Chesscell(Bishop ,Black) ,f ,7);
    board.place(Chesscell(Knight ,Black) ,g ,7);
    board.place(Chesscell(Rook   ,Black) ,h ,7);
    for (int i = 0; i < 8; ++i) board.place(Chesscell(Pawn, Black), i, 6);
}

int main() {
  nsChess::Board board;
  fill(board);



  print(board);
  board.move(Coordinate(e, 1), Coordinate(e, 3));
  print(board);
  board.move(Coordinate(d, 6), Coordinate(d, 4));
  print(board);
  board.move(Coordinate(e, 3), Coordinate(d, 4));
  print(board);
  board.move(Coordinate(e, 1), Coordinate(c, 1));

  std::cout << std::endl;
  print(board);

  return 0;
}
