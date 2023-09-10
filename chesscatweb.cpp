#include "chesscat.hpp"
#include <emscripten/bind.h>

using namespace emscripten;

using namespace chesscat;

int add(int a, int b){
    return a + b;
}

class MoveIterator{
    private:
        std::vector<Move> moves;
    public:
        MoveIterator(std::vector<Move> moves_) : moves(moves_) {}
        unsigned int getNumMoves(){
            return moves.size();
        }
        Move getMoveAt(int index){
            return moves[index];
        }
};

MoveIterator getLegalMovesFromSquareOnPosition(Position &pos, Square square){
    std::vector<Move> moves;
    pos.iterateLegalMovesFromSquare(square, [&moves, &square](Square to) -> chesscat::internal::MoveIterationResult{
        moves.push_back({.from = square, .to = to});
        return chesscat::internal::ContinueMoveIteration;
    });
    MoveIterator iterator(moves);
    return iterator;
}

EMSCRIPTEN_BINDINGS(my_module){
    enum_<PieceType>("PieceType")
        .value("Empty", Empty)
        .value("Pawn", Pawn)
        .value("Knight", Knight)
        .value("Bishop", Bishop)
        .value("Rook", Rook)
        .value("Queen", Queen)
        .value("King", King)
        ;
    enum_<Color>("Color")
        .value("White", White)
        .value("Black", Black)
        ;
    value_array<Piece>("Piece")
        .element(&Piece::color)
        .element(&Piece::type)
        ;
    value_array<Square>("Square")
        .element(&Square::row)
        .element(&Square::col)
        ;
    value_array<Move>("Move")
        .element(&Move::from)
        .element(&Move::to)
        ;
    enum_<PositionState>("PositionState")
        .value("Normal", Normal)
        .value("Check", Check)
        .value("Checkmate", Checkmate)
        .value("Stalemate", Stalemate)
        ;
    class_<Board>("Board")
        .constructor<>()
        .function("getNumCols", &Board::getNumCols)
        .function("getNumRows", &Board::getNumRows)
        .function("resize", &Board::resize)
        .function("setPiece", &Board::setPiece)
        .function("getPiece", &Board::getPiece)
        .function("shift", &Board::shift)
        ;

    class_<Position>("Position")
        .constructor<>()
        .function("getBoard", &Position::getBoard)
        .function("getLegalMovesFromSquare", &getLegalMovesFromSquareOnPosition)
        .function("playMove", &Position::playMove)
        .function("getState", &Position::getState)
        ;
    class_<MoveIterator>("MoveIterator")
        .function("getMoveAt", &MoveIterator::getMoveAt)
        .function("getNumMoves", &MoveIterator::getNumMoves)
        ;
}
