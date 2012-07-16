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

#include <vector>

const int M = 13;
const int N = 10;

typedef char board_t[M][N];

struct pos_t {
  char x;
  char y;
};

const int MAX_PLAYERS = 4;
const int MAX_PENGUINS = 4;

struct player_t {
  pos_t penguin[MAX_PENGUINS];
  char n_played_penguins;
  unsigned char score;
};

struct state_t {
  board_t board;
  player_t player[MAX_PLAYERS];
  char n_players;
  char n_penguins;
  char cur_player_idx;
};

const int NUM_DIRS = 6;
const pos_t dirs[NUM_DIRS] = { {+1,0}, {0,+1}, {-1,+1}, {-1,0}, {0,-1}, {+1,-1} };

struct move_t {
  char player_idx;   // Index of player who moved.
  char penguin_idx;  // Index of penguin to be moved.
  pos_t dest;        // Destination of the penguin.
};

int compute_utility(const state_t& state, const int player_idx);

void make_move(move_t move, state_t* state);

void compute_moves(const state_t& state,
                   const int player_idx,
                   std::vector<move_t>* moves);

state_t random_simulation(const state_t& st, const int player_idx);

void random_init_board(board_t board);

state_t random_game_result(const int n_players, const int n_penguins);

void read_state(const char* desc[N], state_t* state);
