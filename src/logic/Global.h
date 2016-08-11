#ifndef GLOBAL_H
#define GLOBAL_H

namespace chess {
namespace model {
enum Color { None, White, Black };
enum Piece { Less, Pawn, Knight, Bishop, Rook, Queen, King };
enum Size  { Width = 8, Height = 8, Total = Width * Height };
enum Columns { A, B, C, D, E, F, G, H };
enum Rows { Eight, Seven, Six, Five, Four, Three, Two, One };

struct Position {
public:
    Position(int x, int y) : x(x), y(y) { }
    bool operator==(const Position& p) const {
        return (x == p.x && y == p.y);
    }
    int x;
    int y;
};
}
}
#endif // GLOBAL_H
