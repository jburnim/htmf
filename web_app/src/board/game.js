angular.module('myApp', [])
  .factory('Game', function() {
    var Game = function() {
      var N = 8;
      // Initialize game state.
      this.numberOfPlayers = 2;
      this.currentPlayer = 0;
      this.scores = [0, 0];
      this.pieces = [[{player: 0}, {player: 0}, {player: 0}, {player: 0}],
                     [{player: 1}, {player: 1}, {player: 1}, {player: 1}]];
      this.board = function() {
        var remainingTiles = {1: 30, 2: 20, 3: 10};
        var numRemainingTiles = 60;
        var board = new Array(N);
        for (var r = 0; r < N; r++) {
          board[r] = new Array(N - (r % 2));
          for (var c = 0; c < board[r].length; c++) {
            var rnd = Math.floor(Math.random() * numRemainingTiles);
            for (tile in remainingTiles) {
              if (rnd < remainingTiles[tile]) {
                board[r][c] = { r: r, c: c, tile: tile };
                remainingTiles[tile] -= 1;
                numRemainingTiles -= 1;
                break;
              } else {
                rnd -= remainingTiles[tile];
              }
            }
          }
        }
        return board;
      }();

      //
      // Helper methods.
      //

      this.getPieceAt = function(r, c) {
        for (var player = 0; player < this.numberOfPlayers; player++) {
          for (var piece in this.pieces[player]) {
            if ((piece.r === r) && (piece.c === c)) {
              return piece;
            }
          }
        }
      };

      this.isPlacementPhase = function() {
        for (var piece in this.pieces[this.currentPlayer]) {
          if (!('r' in piece) || !('c' in piece)) {
            return true;
          }
        }
        return false;
      };

      this.movePiece = function(piece, r, c) {
        console.assert(this.currentPlayer === piece.player);
        console.assert(this.board[r][c].tile);
        // TODO(jburnim): Check that the move is legal.
        if ((piece.r !== undefined) && (piece.c !== undefined)) {
          this.board[piece.r][piece.c].piece = undefined;
        }
        this.scores[piece.player] += this.board[r][c].tile;
        this.board[r][c].tile = undefined;
        piece.r = r;
        piece.c = c;
        this.board[r][c].piece = piece;
        this.currentPlayer = (this.currentPlayer + 1) % this.numberOfPlayers;
      };

      this.pass = function() {
        this.currentPlayer = (this.currentPlayer + 1) % this.numberOfPlayers;
      };
    };

    return new Game();
  })
