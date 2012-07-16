// Copyright (c) 2012, Jacob Burnim <jburnim@gmail.com>
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

#include "game.h"

#include <cassert>

void test_compute_moves() {
  state_t state;
  int n_players, n_penguins;

  const char* board1[N] = { "             ",
                            "       21    ",
                            "       1A1   ",
                            "        12   ",
                            "     A       ",
                            "             ",
                            "        A    ",
                            "             ",
                            "             ",
                            "             " };
  read_state(board1, &state);

  std::vector<move_t> moves;
  compute_moves(state, 0, &moves);
  assert(moves.size() == 6);
  for (int i = 0; i < moves.size(); i++) {
    printf("%d: -> (%d,%d)\n",
           moves[i].penguin_idx, moves[i].dest.x, moves[i].dest.y);
  }
  printf("\n");
}


int main(int argc, char* argv[]) {
  const int n_players = atoi(argv[1]);
  const int n_penguins = atoi(argv[2]);

  srand(time(NULL));

  const int TRIALS = 100000;
  int total_scores[MAX_PLAYERS] = { 0, 0, 0, 0 };
  for (int i = 0; i < TRIALS; i++) {
    state_t res = random_game_result(n_players, n_penguins);
    for (int j = 0; j < n_players; j++) {
      total_scores[j] += res.player[j].score;
    }
  }

  for (int j = 0; j < n_players; j++) {
    printf("%lf ", total_scores[j] / (double)TRIALS);
  }
  printf("\n");

  return 0;
}
