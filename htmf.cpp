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

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>

using std::vector;

const int M = 13;
const int N = 10;

typedef char board_t[M][N];

struct pos_t {
  char x;
  char y;
};

const int MAX_PLAYERS = 4;
const int MAX_PENGUINS = 4;

typedef struct player_t {
  pos_t penguin[MAX_PENGUINS];
  unsigned char n_played_penguins;
  unsigned char score;
};

struct state_t {
  board_t board;
  player_t player[MAX_PLAYERS];
};

const int NUM_DIRS = 6;
const pos_t dirs[NUM_DIRS] = { {+1,0}, {0,+1}, {-1,+1}, {-1,0}, {0,-1}, {+1,-1} };

struct move_t {
  char penguin_idx;  // Index of penguin to be moved.
  pos_t dest;        // Destination of the penguin.
};

void print_state(const int n_players,
                 const int n_penguins,
                 const state_t& state)
{
  for (int y = N-1; y >= 0; y--) {
    for (int x = 0; x < M; x++) {
      bool found = false;
      for (int i = 0; i < n_players; i++) {
        const player_t& player = state.player[i];
        for (int j = 0; j < n_penguins; j++) {
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

void compute_moves(const int n_penguins,
                   const state_t& state,
                   const player_t& player,
                   vector<move_t>* moves)
{
  move_t move;
  for (int i = 0; i < n_penguins; i++) {
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
}

void read_state(const char* desc[N],
                state_t* state,
                int* const n_players,
                int* const n_penguins)
{
  board_t& board = state->board;
  for (int i = 0; i < MAX_PLAYERS; i++) {
    state->player[i].n_played_penguins = 0;
  }
  *n_players = 0;

  for (int x = 0; x < M; x++) {
    for (int y = 0; y < N; y++) {
      if ((desc[N-y-1][x] >= '1') && (desc[N-y-1][x] <= '3')) {
        board[x][y] = desc[N-y-1][x] - '0';
      } else {
        board[x][y] = 0;
      }
      if ((desc[N-y-1][x] >= 'A') && (desc[N-y-1][x] <= 'D')) {
        int i = desc[N-y-1][x] - 'A';
        player_t& player = state->player[i];
        player.penguin[player.n_played_penguins].x = x;
        player.penguin[player.n_played_penguins].y = y;
        player.n_played_penguins++;
        *n_players = std::max(*n_players, i+1);
      }
    }
  }

  assert(*n_players > 0);
  *n_penguins = state->player[0].n_played_penguins;
  for (int i = 1;  i < *n_players; i++) {
    *n_penguins = std::max(*n_penguins,
                           (int)state->player[i].n_played_penguins);
  }
}


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
  read_state(board1, &state, &n_players, &n_penguins);

  vector<move_t> moves;
  compute_moves(n_penguins, state, state.player[0], &moves);
  assert(moves.size() == 6);
  for (int i = 0; i < moves.size(); i++) {
    printf("%d: -> (%d,%d)\n",
           moves[i].penguin_idx, moves[i].dest.x, moves[i].dest.y);
  }
  printf("\n");
}

// game board goes from:
//   (1,8) ... ( 7,8)
//   (1,7) ... ( 8,7)
//   (2,6) ... ( 8,6)
//   (2,5) ... ( 9,5)
//   (3,4) ... ( 9,4)
//   (3,3) ... (10,3)
//   (4,2) ... (10,2)
//   (4,1) ... (11,1)

// might try to read (3,0)
// might try to read (12,0)
// might try to read (0,9)
// might try to read (8,9)

// so, can store the board in 128 characters -- entries B[12][8] and
// B[12][9] don't exist.

// counter-clockwise, neighbors are:
//   (+1,0),(0,+1),(-1,+1),(-1,0),(0,-1),(+1,-1)


state_t random_simulation(const int n_players,
                          const int n_penguins,
                          const state_t& st,
                          const int player_idx)
{
  state_t state = st;

  // Play randomly until no player can move.
  for (int i = player_idx, turns_since_move = 0; ; i = (i+1)%n_players) {
    player_t& player = state.player[i];

    if (player.n_played_penguins < n_penguins) {
      // Place a penguin randomly on the board.

      // Find possible initial positions.  (We could do this faster,
      // but with less generality, by using the fact that there are
      // thirty 1's.)
      vector<pos_t> init_positions;
      for (pos_t p = {0, 0}; p.x < M; p.x++) {
        for (p.y = 0; p.y < N; p.y++) {
          if (state.board[p.x][p.y] == 1) {
            init_positions.push_back(p);
          }
        }
      }

      // Randomly place penguin in one such position.
      int pos_idx = int(rand()/(double)RAND_MAX * init_positions.size());
      pos_t pos = init_positions[pos_idx];
      player.penguin[player.n_played_penguins++] = pos;
      player.score += 1;
      state.board[pos.x][pos.y] = 0;

      turns_since_move = 0;

    } else {
      // Randomly move one penguin.
      vector<move_t> moves;
      compute_moves(n_penguins, state, player, &moves);
      if (moves.size() == 0) {
        if (++turns_since_move == n_players)
          break;
      } else {
        move_t move = moves[int(rand()/(double)RAND_MAX * moves.size())];

        player.penguin[move.penguin_idx] = move.dest;
        assert(state.board[move.dest.x][move.dest.y]);
        player.score += state.board[move.dest.x][move.dest.y];
        state.board[move.dest.x][move.dest.y] = 0;

        turns_since_move = 0;
      }
    }
  }

  return state;
}


void init_board(board_t board) {
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


state_t random_game_result(const int n_players, const int n_penguins) {
  state_t state;

  // Random board.
  init_board(state.board);

  // Initialize players.
  for (int i = 0; i < n_players; i++) {
    player_t& player = state.player[i];
    player.n_played_penguins = 0;
    player.score = 0;
  }

  return random_simulation(n_players, n_penguins, state, 0);
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
