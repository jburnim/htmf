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

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <cstring>
#include <ctime>

using std::vector;


int compute_utility(const state_t& state, const int player_idx) {
  int max_other_score = 0;
  for (int i = 0; i < state.n_players; i++) {
    if (i != player_idx) {
      if (state.player[i].score > max_other_score) {
        max_other_score = state.player[i].score;
      }
    }
  }

  return (state.player[player_idx].score - max_other_score);
}


void make_move(move_t move, state_t* state) {
  player_t& player = state->player[move.player_idx];

  assert(move.player_idx >= 0);
  assert(move.player_idx < state->n_players);
  assert(move.player_idx == state->cur_player_idx);

  assert(move.penguin_idx >= 0);
  assert(move.penguin_idx < state->n_penguins);
  assert(move.penguin_idx <= state->player[move.penguin_idx].n_played_penguins);

  assert(move.dest.x >= 0);
  assert(move.dest.y >= 0);
  assert(move.dest.x < M);
  assert(move.dest.y < N);

  // Update penguin position.
  player.penguin[move.penguin_idx] = move.dest;

  // Update board and score.
  assert(state->board[move.dest.x][move.dest.y]);
  player.score += state->board[move.dest.x][move.dest.y];
  state->board[move.dest.x][move.dest.y] = 0;

  // If necessary, update n_played_penguins.
  if (move.penguin_idx >= player.n_played_penguins) {
    assert(move.penguin_idx == player.n_played_penguins);
    player.n_played_penguins++;
  }

  // Update the current player.
  for (int i = 0; i < state->n_players; i++) {
    state->cur_player_idx = (state->cur_player_idx + 1) % state->n_players;

    // Does this player have legal moves?
    //
    // TODO: Could be more efficient here, by not allocating a new
    // vector and by stopping after finding the first legal move.
    vector<move_t> moves;
    compute_moves(*state, state->cur_player_idx, &moves);
    if (moves.size() > 0) {
      // This player has moves, so he will move next.
      return;
    }
  }

  // No player has legal moves, so game is over.
  state->cur_player_idx = NO_PLAYER;
}


void print_state(const state_t& state) {
  for (int y = N-1; y >= 0; y--) {
    for (int x = 0; x < M; x++) {
      bool found = false;
      for (int i = 0; i < state.n_players; i++) {
        const player_t& player = state.player[i];
        for (int j = 0; j < player.n_played_penguins; j++) {
          if ((player.penguin[j].x == x) && (player.penguin[j].y == y)) {
            printf("%c ", (char)('A' + i));
            found = true;
          }
        }
      }
      if (!found) {
        printf("%d ", (int)state.board[x][y]);
      }
    }
    printf("\n");
  }
}

void compute_moves(const state_t& state,
                   const int player_idx,
                   vector<move_t>* moves)
{
  const player_t& player = state.player[player_idx];

  if (player.n_played_penguins == state.n_penguins) {
    // All penguins have been placed -- try moving them.
    move_t move;
    move.player_idx = player_idx;

    for (int i = 0; i < state.n_penguins; i++) {
      move.penguin_idx = i;
      pos_t p = player.penguin[i];
      for (int j = 0; j < NUM_DIRS; j++) {
        move.dest = p;
        while (true) {
          move.dest.x += dirs[j].x;
          move.dest.y += dirs[j].y;
          if (state.board[move.dest.x][move.dest.y]) {
            moves->push_back(move);
          } else {
            break;
          }
        }
      }
    }

  } else {
    // Not all penguins have been placed -- place the next one.
    move_t move;
    move.player_idx = player_idx;
    move.penguin_idx = player.n_played_penguins;

    for (move.dest.x = 0; move.dest.x < M; move.dest.x++) {
      for (move.dest.y = 0; move.dest.y < N; move.dest.y++) {
        if (state.board[move.dest.x][move.dest.y] == 1) {
          moves->push_back(move);
        }
      }
    }
  }
}


void read_state(const char* desc[N], state_t* state) {
  board_t& board = state->board;
  for (int i = 0; i < MAX_PLAYERS; i++) {
    state->player[i].n_played_penguins = 0;
  }
  state->n_players = 0;

  for (int x = 0; x < M; x++) {
    for (int y = 0; y < N; y++) {
      if ((desc[N-y-1][x] >= '1') && (desc[N-y-1][x] <= '3')) {
        board[x][y] = desc[N-y-1][x] - '0';
      } else {
        board[x][y] = 0;
      }
      if ((desc[N-y-1][x] >= 'A') && (desc[N-y-1][x] <= 'D')) {
        char i = desc[N-y-1][x] - 'A';
        player_t& player = state->player[i];
        player.penguin[player.n_played_penguins].x = x;
        player.penguin[player.n_played_penguins].y = y;
        player.n_played_penguins++;
        state->n_players = std::max(state->n_players, (char)(i+1));
      }
    }
  }

  assert(state->n_players > 0);
  state->n_penguins = state->player[0].n_played_penguins;
  for (int i = 1;  i < state->n_players; i++) {
    state->n_penguins = std::max(state->n_penguins,
                                 state->player[i].n_played_penguins);
  }
}


void random_simulation(state_t* state) {
  const int n_players = state->n_players;
  const int n_penguins = state->n_penguins;

  // Play randomly until no player can move.
  while (state->cur_player_idx != NO_PLAYER) {
    player_t& player = state->player[state->cur_player_idx];

    // Generate legal moves.
    vector<move_t> moves;
    compute_moves(*state, state->cur_player_idx, &moves);
    assert(moves.size() > 0);

    // Randomly move one penguin.
    move_t move = moves[int(rand()/(double)RAND_MAX * moves.size())];
    make_move(move, state);
  }
}


void random_init_board(board_t board) {
  memset(board, 0, M*N);

  char tiles[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                   2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                   2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };


  // shuffle tiles
  std::random_shuffle(tiles, &tiles[sizeof(tiles)]);

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

  int n = 0;
  for (int y = 1; y <= 8; y++) {
    for (int x = x_range[y][0]; x <= x_range[y][1]; x++) {
      board[x][y] = tiles[n++];
    }
  }
  assert(n == sizeof(tiles));
}
