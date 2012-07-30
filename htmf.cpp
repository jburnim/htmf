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
#include "uct.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>

const int NUM_TRIALS = 1;
const int NUM_UCT_TRIALS = 10000;

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


state_t random_game_result(const int n_players, const int n_penguins) {
  state_t state;
  state.n_players = n_players;
  state.n_penguins = n_penguins;
  state.cur_player_idx = 0;

  // Random board.
  random_init_board(state.board);

  // Initialize players.
  for (int i = 0; i < n_players; i++) {
    player_t& player = state.player[i];
    player.n_played_penguins = 0;
    player.score = 0;
  }

  // Play a random game.
  random_simulation(&state);

  return state;
}


state_t uct_game_result(const int n_players, const int n_penguins) {
  state_t state;
  state.n_players = n_players;
  state.n_penguins = n_penguins;
  state.cur_player_idx = 0;

  // Random board.
  random_init_board(state.board);

  // Initialize players.
  for (int i = 0; i < n_players; i++) {
    player_t& player = state.player[i];
    player.n_played_penguins = 0;
    player.score = 0;

    for (int j = 0; j < n_penguins; j++) {
      player.penguin[j].x = -1;
      player.penguin[j].y = -1;
    }
  }

  print_state(state);

  move_t move;
  while (state.cur_player_idx != NO_PLAYER) {
    uct_move(NUM_UCT_TRIALS, state, &move);

    const player_t& player = state.player[move.player_idx];
    printf("\n%d: (%d,%d) -> (%d,%d)\n\n",
           move.player_idx,
           player.penguin[move.penguin_idx].x,
           player.penguin[move.penguin_idx].y,
           move.dest.x, move.dest.y);

    make_move(move, &state);

    print_state(state);
  }

  return state;
}


int main(int argc, char* argv[]) {
  const int n_players = atoi(argv[1]);
  const int n_penguins = atoi(argv[2]);

  srand(time(NULL));

  int total_scores[MAX_PLAYERS] = { 0, 0, 0, 0 };
  for (int i = 0; i < NUM_TRIALS; i++) {
    state_t res = uct_game_result(n_players, n_penguins);
    for (int j = 0; j < n_players; j++) {
      total_scores[j] += res.player[j].score;
    }
  }

  for (int j = 0; j < n_players; j++) {
    printf("%lf ", total_scores[j] / (double)NUM_TRIALS);
  }
  printf("\n");

  return 0;
}
