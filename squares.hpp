#ifndef SQUARES_HPP
#define SQUARES_HPP

namespace chesscat{
    namespace internal{
        enum MoveIterationResult
        {
            ContinueMoveIteration,
            BreakMoveIteration
        };
    }
    struct Square
    {
        int row;
        int col;
    };

    struct Move
    {
        Square from;
        Square to;
    };
}

#endif
