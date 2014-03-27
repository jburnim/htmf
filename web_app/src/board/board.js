angular.module('myApp', [])
  .directive('board', function() {
    return {
      restrict: 'E',
      controller: function($scope) {

        // TODO(jburnim): Extract this code.  (But to where?)
        var Game = function(N) {
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
            // return undefined
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
            console.assert(this.currentPlayer == piece.player);
            console.assert(this.board[r][c].tile);
            // TODO(jburnim): Check that the move is legal.
            this.scores[piece.player] += this.board[r][c].tile;
            this.board[r][c].tile = undefined;
            piece.r = r;
            piece.c = c;
            this.currentPlayer = (this.currentPlayer + 1) % this.numberOfPlayers;
          };

          this.pass = function() {
            this.currentPlayer = (this.currentPlayer + 1) % this.numberOfPlayers;
          };
        };

        $scope.game = new Game(8);
        $scope.RADIUS = Math.min(100 / ($scope.game.board.length * 1.5 + 1),
                                 100 / ($scope.game.board[0].length * Math.sqrt(3)));
        $scope.R_STEP = 1.5 * $scope.RADIUS;
        $scope.C_STEP = Math.sqrt(3) * $scope.RADIUS;
        $scope.center = function(r, c) {
          return {
            x: $scope.C_STEP * (c + 0.5 + (r % 2) / 2),
            y: $scope.R_STEP * r + $scope.RADIUS
          };
        };
        $scope.points = function(r, c) {
          var PI_OVER_3 = 1.04719755;
          var PI_OVER_2 = 1.57079633;
          var center = $scope.center(r, c);
          var ret = "";
          for (var i = 0, ang = PI_OVER_2; i < 6; i++, ang += PI_OVER_3) {
            ret += (center.x + $scope.RADIUS * Math.cos(ang)) + ",";
            ret += (center.y + $scope.RADIUS * Math.sin(ang)) + " ";
          }
          return ret;
        };

        $scope.click = function(r, c) {
          $scope.game.board[r][c].tile = undefined;
        };
      },
      templateUrl: 'board/board.tpl.html'
    }
  })
