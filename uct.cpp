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

#include "uct.h"

#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

#include "game.h"

using std::vector;

void delete_tree(node_t* n) {
  node_t *p = n->child;
  while (p != NULL) {
    node_t* q = p;
    p = p->next;
    delete q;
  }
}


// Returns true if children were expanded.  Returns false if this is a
// terminal node.
void expand(const state_t& state, node_t* const n) {
  assert(n->child == NULL);

  if (state.cur_player_idx == NO_PLAYER) {
    // Nothing to do -- game is over.
    return;
  }

  // Compute legal moves.
  vector<move_t> moves;
  compute_moves(state, state.cur_player_idx, &moves);
  assert(moves.size() > 0);

  // Expand children.
  node_t** prev = &n->child;
  for (int j = 0; j < moves.size(); j++) {
    // Allocate new child node.
    node_t* p = new node_t;
    p->n_visits = p->total_score = 0;
    p->child = p->next = NULL;
    p->move = moves[j];

    // Update previous child/next link.
    *prev = p;
    prev = &p->next;
  }
}


bool is_terminal(const node_t* const n) {
  return (n->child == NULL);
}


double rand_double() {
  return (rand() / (double)RAND_MAX);
}


node_t* select(node_t* n) {
  assert(n->child);

  node_t* ret = NULL;
  double best = -std::numeric_limits<double>::max();

  // printf("Selecting a child [%lf]:", best);
  for (node_t* p = n->child; p != NULL; p = p->next) {
    double expected_val = p->n_visits ? (p->total_score / (double)p->n_visits) : 0.0;
    double uct = sqrt(log(n->n_visits + 1) / (p->n_visits + EPSILON));
    double val = expected_val + UCT_FACTOR*uct + EPSILON*rand_double();
    // printf(" %lf [%d/%d]", val, p->total_score, p->n_visits);
    if (val > best) {
      ret = p;
      best = val;
    }
  }
  // printf("\n\n");

  return ret;
}


void uct_once(node_t* n, state_t* state) {
  if (n->n_visits == 0) {
    // First visit to this node, randomly simulate to compute an
    // initial score.
    assert(n->child == NULL);
    random_simulation(state);

  } else {
    // Going to use UCT to select a child.

    if (n->n_visits == 1) {
      // Second visit to this node, time to expand the children
      assert(n->child == NULL);
      expand(*state, n);
    }

    if (!is_terminal(n)) {
      // Recursively continue exploring.
      node_t* child = select(n);
      make_move(child->move, state);
      uct_once(child, state);
    }
  }

  // Update our stats, given the simulated game result.
  n->n_visits++;
  n->total_score += compute_utility(*state, n->move.player_idx);
}


void uct_move(const int n_moves, const state_t& state, move_t* move) {
  assert(state.cur_player_idx >= 0);
  assert(state.cur_player_idx < state.n_players);

  bool ret = false;
  node_t* root = new node_t;
  root->n_visits = root->total_score = -1;
  root->child = root->next = NULL;
  root->move.player_idx = -1;

  // Explore the game tree.
  for (int i = 0; i < n_moves; i++) {
    state_t s = state;
    uct_once(root, &s);
  }

  // Select the best move we've found.
  double best = -std::numeric_limits<double>::max();
  for (node_t* n = root->child; n != NULL; n = n->next) {
    double expected_val = n->n_visits ? (n->total_score / (double)n->n_visits) : 0.0;
    if (expected_val > best) {
      best = expected_val;
      *move = n->move;
    }
  }

  printf("\nexpected value: %lf\n", best);

  // Clean up the tree.
  delete_tree(root);
}
