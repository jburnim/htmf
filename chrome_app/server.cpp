// Copyright (c) 2014, Jacob Burnim <jburnim@gmail.com>
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  - Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  - Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#include "server.h"

#include "game.h"

// game board goes from:
//   (1,8) ... ( 7,8)
//   (1,7) ... ( 8,7)
//   (2,6) ... ( 8,6)
//   (2,5) ... ( 9,5)
//   (3,4) ... ( 9,4)
//   (3,3) ... (10,3)
//   (4,2) ... (10,2)
//   (4,1) ... (11,1)
const int x_range[][2] =
  { {0, 0}, {4, 11}, {4, 10}, {3, 10}, {3, 9}, {2, 9}, {2, 8}, {1, 8}, {1, 7} };

std::string toJSON(const state_t& state) {
  std::ostringstream out;
  toJSON(state, out);
  return out.str();
}

void toJSON(const state_t& state, std::ostream& out) {
  out << "{ ";

  out << "\"numberOfPlayers\": " << (int) state.n_players << ", ";
  out << "\"currentPlayer\": " << (int) state.cur_player_idx << ", ";

  out << "\"scores\": [ ";
  for (int i = 0; i < state.n_players; i++) {
    if (i != 0) {
      out << ", ";
    }
    out << (int) state.player[i].score;
  }
  out << " ], ";

  out << "\"board\": [ ";
  for (int i = 1; i <= 8; i++) {
    if (i != 1) {
      out << ", ";
    }
    out << "[ ";
    for (int j = x_range[i][0]; j <= x_range[i][1]; j++) {
      if (j != x_range[i][0]) {
        out << ", ";
      }
      if (state.board[j][i] != 0) {
        // There is a tile at this board location.
        out << "{\"tile\": " << (int) state.board[j][i] << "}";
      } else {
        // Check if there is a piece at this board location.
        if (false) {
          int piece = 0;
          out << "{\"piece\": " << (int) piece << "}";
        } else {
          out << "{}";
        }
      }
    }
    out << " ]";
  }
  out << " ]";

  out << " }";
}
