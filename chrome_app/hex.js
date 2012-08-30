
function createHexagon(c, r) {
  var PI_OVER_3 = 1.04719755;
  var PI_OVER_2 = 1.57079633;
  var pts = "";
  for (var i = 0, ang = PI_OVER_2; i < 6; i++, ang += PI_OVER_3) {
    var x = c.x + r*Math.cos(ang);
    var y = c.y + r*Math.sin(ang);
    pts += x + "," + y + " ";
  }
  var hex = document.createElementNS('http://www.w3.org/2000/svg', 'polygon');
  hex.setAttribute('points', pts);
  return hex;
}

var X_RANGE = [
  [0, -1],
  [4, 11],
  [4, 10],
  [3, 10],
  [3,  9],
  [2,  9],
  [2,  8],
  [1,  8],
  [1,  7]
];

var COLORS = { 'A': 'red',
               'B': 'forestgreen',
               'C': 'blue',
               'D': 'yellow',
               '1': 'black',
               '2': 'black',
               '3': 'black'
             };

function updateBoard(boardSvgId, boardSpec) {
  for (var y = 1; y <= 8; y++) {
    for (var x = X_RANGE[y][0]; x <= X_RANGE[y][1]; x++) {
      var hex = document.getElementById('board' + x + '' + y);
      var label = document.getElementById('label' + x + '' + y);
      if (boardSpec[y][x] == '0') {
        hex.setAttribute('visibility', 'hidden');
        label.textContent = '';
      } else {
        hex.setAttribute('visibility', 'visible');
        label.setAttribute('fill', COLORS[boardSpec[y][x]]);
        label.textContent = boardSpec[y][x];
      }
    }
  }
}

function makeBoard(boardSvgId, radius, clickHandler) {
  var X_STEP = {
    x: Math.sqrt(3) * radius,
    y: 0
  };
  var Y_STEP = {
    x: Math.sqrt(3)/2 * radius,
    y: -1.5 * radius
  };
  var ORIGIN = {
    x: -4*Math.sqrt(3) * radius + 10,
    y: 13 * radius + 10
  };

  svg = document.getElementById(boardSvgId);

  for (var y = 1; y <= 8; y++) {
    for (var x = X_RANGE[y][0]; x <= X_RANGE[y][1]; x++) {
      var center = {
        x: ORIGIN.x + x*X_STEP.x + y*Y_STEP.x,
        y: ORIGIN.y + x*X_STEP.y + y*Y_STEP.y
      };

      var hex = createHexagon(center, radius);
      hex.setAttribute('id', 'board' + x + '' + y );
      hex.setAttribute('fill', 'lightsteelblue');
      hex.setAttribute('stroke-width', 3);
      hex.setAttribute('stroke', 'black');
      hex.setAttribute('onclick', clickHandler + '(' + x + ',' + y + ');');
      svg.appendChild(hex);

      var label = document.createElementNS('http://www.w3.org/2000/svg', 'text');
      label.setAttribute('id', 'label' + x + '' + y);
      label.setAttribute('x', center.x);
      label.setAttribute('y', center.y);
      label.setAttribute('text-anchor', 'middle');
      label.setAttribute('alignment-baseline', 'middle');
      label.setAttribute('font-size', radius + 'px');
      label.setAttribute('onclick', clickHandler + '(' + x + ',' + y + ');');
      svg.appendChild(label);
    }
  }
}

