const board = document.getElementsByClassName("board")[0];

function samesquare(s1, s2){
    return ((s1[0] == s2[0]) && (s1[1] == s2[1]));
}

function getSquare(row, col){
    return board.children[row].children[col];
}

function getSquarePosition(square_elem){
    let col = Array.from(square_elem.parentElement.children).indexOf(square_elem);
    let row = Array.from(square_elem.parentElement.parentElement.children).indexOf(square_elem.parentElement);

    return [row, col];
}

function getSquareParent(elem){
    if(!board.contains(elem)) return null;
    if(elem.classList.contains("square")) return elem;
    if(elem == board) return null;
    return getSquareParent(elem.parentElement);
}

function getPieceImage(piece){
    pieceName = "";
    switch(piece[1]){
        case Module.PieceType.Bishop:
            pieceName+="b";
            break;
        case Module.PieceType.King:
            pieceName+="k";
            break;
        case Module.PieceType.Knight:
            pieceName+="n";
            break;
        case Module.PieceType.Pawn:
            pieceName+="p";
            break;
        case Module.PieceType.Queen:
            pieceName+="q";
            break;
        case Module.PieceType.Rook:
            pieceName+="r";
            break;
        default:
            break;
    }
    if(piece[0] == Module.Color.White){
        pieceName += "w";
    }
    else if(piece[0] == Module.Color.Black){
        pieceName += "b";
    }
    return pieceName + ".png";
}

let game;

function resetBoard(){
    board.innerHTML = "";

    const gameBoard = game.getBoard();

    for(let rownum = 0; rownum < gameBoard.getNumRows(); rownum++){
        const row = document.createElement("div");
        row.classList.add("row");
        board.appendChild(row);
        for(let col = 0; col < gameBoard.getNumCols(); col++){
            const square = document.createElement("div");
            square.classList.add("square");
            row.appendChild(square);
        }
    }

    refreshBoardPieces();
}

function resetGame(){
    if(game){
        game.delete();
        game = new Module.Position();
    }
    else{
        game = new Module.Position();
    }
    resetBoard();
}

function refreshBoardPieces(){
    const gameBoard = game.getBoard();

    for(let rownum = 0; rownum < gameBoard.getNumRows(); rownum++){
        for(let col = 0; col < gameBoard.getNumCols(); col++){
            const piece = gameBoard.getPiece([rownum, col]);
            const square = getSquare(rownum, col);
            square.innerHTML = "";
            if(piece[1] != Module.PieceType.Empty){
                const image = document.createElement("img");
                image.src = "images/pieces/" + getPieceImage(piece);
                square.appendChild(image);
            }
        }
    }
    const posState = game.getState();
    if(posState == Module.PositionState.Checkmated){
        alert("Checkmate!");
    }
    else if(posState == Module.PositionState.Stalemated){
        alert("Stalemate!")
    }
}

function setTargetSquare(square){
    let squares = document.querySelectorAll(".square.target");
    squares.forEach((removing) => {
        if(removing != square){
            removing.classList.remove("target");
        }
    });
    if(square && !square.classList.contains("target")){
        square.classList.add("target");
        let squarePos = getSquarePosition(square);
        let possible_move_squares = document.querySelectorAll(".square.possible-move");
        possible_move_squares.forEach((square) => {
            square.classList.remove("possible-move");
        });
        let moves = game.getLegalMovesFromSquare(squarePos);
        for(let i = 0; i < moves.getNumMoves(); i++){
            const move = moves.getMoveAt(i);
            const origin = move.getOrigin();
            const destination = move.getDestination();
            getSquare(destination[0], destination[1]).classList.add("possible-move");
        }
        moves.delete();
    }
    else if(!square){
        let possible_move_squares = document.querySelectorAll(".square.possible-move");
        possible_move_squares.forEach((square) => {
            square.classList.remove("possible-move");
        });
    }
}

function clickSquare(square){
    if(!square.classList.contains("possible-move")){
        setTargetSquare(square);
    }
    else{
        square_from = document.querySelector(".square.target");
        if(square_from){
            square_to_pos = getSquarePosition(square);
            square_from_pos = getSquarePosition(square_from);
            let move;
            const moves = game.getLegalMovesFromSquare(square_from_pos);
            for(let i = 0; i < moves.getNumMoves(); i++){
                const m = moves.getMoveAt(i);
                const origin = m.getOrigin();
                const destination = m.getDestination();
                if(samesquare(destination, square_to_pos)){
                    move = m;
                }
            }
            moves.delete();
            if(!move){throw "Move not found!";}
            game.playMove(move);
            setTargetSquare(null);
            refreshBoardPieces();
        }
    }
}

Module.onRuntimeInitialized = function () {
    resetGame();
};

board.addEventListener("mousedown", (e) => {
    const targetSquare = getSquareParent(e.target);
    if(targetSquare){
        clickSquare(targetSquare);
    }
});
