#ifndef CHESSCAT_HPP
#define CHESSCAT_HPP

#include <vector>
#include <memory>
#include <functional>

#include "pieces.hpp"
#include "squares.hpp"

namespace chesscat {

    enum PositionState {
        Normal,
        Check,
        Checkmate,
        Stalemate
    };

    class Board {
    private:
        unsigned short num_cols;
        unsigned short num_rows;
        std::unique_ptr<Piece[]> board_array;
    public:
        Board(unsigned short cols, unsigned short rows);
        Board(const Board &copyfrom);
        Board();
        unsigned short getNumCols();
        unsigned short getNumRows();
        void resize(unsigned short new_cols, unsigned short new_rows);
        void setPiece(Square square, Piece piece);
        Piece getPiece(Square square);
        void shift(int vertical, int horizontal);
        bool squareInBounds(Square square);
    };

    class Position {
    private:
        Board board;
        Color to_move;
        Move last_move;

        bool colorCanCapturePiece(Color color, Piece captured); //Check if a color is allowed to capture a piece
        void iteratePossibleMovesFromSquare(Square square, const std::function<internal::MoveIterationResult(const Square)> func);
        void iterateAllPossibleMoves(const std::function<internal::MoveIterationResult(const Move)> func);
        void setNextToMove();
        bool squareIsOnPromotionRank(Square square);
        void playMoveNoConfirm(Move move, PieceType pawn_promotion); //Play a move without confirming its legality
        bool isCheck();
        bool movesIntoCheck(Move move); //Check if a move moves into check
        Square findKing(Color color);
    public:
        Position(Board board);
        Position();
        Position(const Position &copyfrom);
        ~Position();

        Board& getBoard();
        bool isMoveLegal(Move move);
        void iterateLegalMovesFromSquare(Square square, const std::function<internal::MoveIterationResult(const Square)> func);
        void iterateAllLegalMoves(const std::function<internal::MoveIterationResult(const Move)> func);
        void playMove(Move move, PieceType pawn_promotion);
        PositionState getState();
    };

}

#endif
