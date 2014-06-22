angular.module('myApp')
  .controller('ScoreController', function($scope, Game) {
    $scope.Game = Game;

    $scope.range = function(n) {
      var ret = new Array(n);
      for (var i = 0; i < n; i++) {
        ret[i] = i;
      }
      return ret;
    };
  })
