angular.module('myApp')
  .directive('board', function() {
    return {
      restrict: 'E',
      controller: function($scope, Game) {
        $scope.Game = Game;
        $scope.RADIUS = Math.min(100 / ($scope.Game.board.length * 1.5 + 1),
                                 100 / ($scope.Game.board[0].length * Math.sqrt(3)));
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

        $scope.getColorForPlayer = function(player) {
          return ['red', 'forestgreen', 'blue', 'yellow'][player];
        };

        $scope.getSymbolForPlayer = function(player) {
          return String.fromCharCode(65 + player);
        };

        $scope.isSelected = function(r, c) {
          return $scope.selectedPiece
            && ($scope.selectedPiece === $scope.Game.board[r][c].piece);
        };

        $scope.click = function(r, c) {
          if ($scope.Game.board[r][c].tile !== undefined) {
            // Clicked on a tile.

            if ($scope.selectedPiece) {
              // Attempt to move the selected piece to this tile.
              $scope.Game.movePiece($scope.selectedPiece, r, c);
              $scope.selectedPiece = undefined;

            } else {
              // Initial placement of a piece on the tile.
              var pieces = $scope.Game.pieces[$scope.Game.currentPlayer];
              for (var i = 0; i < pieces.length; i++) {
                if ((pieces[i].r === undefined) && (pieces[i].c === undefined)) {
                  $scope.Game.movePiece(pieces[i], r, c);
                  break;
                }
              }
            }
          } else if ($scope.Game.board[r][c].piece) {
            // Selecting a piece.
            $scope.selectedPiece = $scope.Game.board[r][c].piece;
          }
        };
      },
      templateUrl: 'board/board.tpl.html'
    }
  })
