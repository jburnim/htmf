// Copyright (c) 2012, Jacob Burnim <jburnim@gmail.com>
// All rights reserved.
//
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

#include <cstring>
#include <iostream>

#include <json/reader.h>

#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>

#include "server.h"
#include "uct.h"

namespace htmf_module {

const int x_range[][2] =
  { {0, 0}, {4, 11}, {4, 10}, {3, 10}, {3, 9}, {2, 9}, {2, 8}, {1, 8}, {1, 7} };

class HtmfInstance : public pp::Instance {
public:
  explicit HtmfInstance(PP_Instance instance) : pp::Instance(instance) {
    memset(&state, 0, sizeof(state_t));
    state.n_players = 2;
    state.n_penguins = 4;
    random_init_board(state.board);
  }

  virtual ~HtmfInstance() {}

  virtual void HandleMessage(const pp::Var& var_message);

private:
  state_t state;
};

void HtmfInstance::HandleMessage(const pp::Var& var_message) {
  Json::Value msg;
  Json::Reader reader;
  if (!reader.parse(var_message.AsString(), msg)) {
    std::cout << "Failed!";
  }

  if (!msg["player"].isNull()
      && (msg["player"].asInt() >= 0)
      && (msg["player"].asInt() < state.n_players)) {

    const player_t& player = state.player[msg["player"].asInt()];

    if (!msg["to"]) {
      // Pass -- nothing to do.
      goto missing_or_invalid_move;
    }

    move_t move;
    int to_r = msg["to"]["r"].asInt();
    int to_c = msg["to"]["c"].asInt();

    move.player_idx = msg["player"].asInt();
    move.dest.y = to_r + 1;
    move.dest.x = to_c + x_range[move.dest.y][0];

    if (!msg["from"]) {
      // Initial placement of piece.
      if (player.n_played_penguins >= state.n_penguins) {
        goto missing_or_invalid_move;
      }
      move.penguin_idx = player.n_played_penguins;
        
    } else {
      int from_r = msg["from"]["r"].asInt();
      int from_c = msg["from"]["c"].asInt();

      int from_y = from_r + 1;
      int from_x = from_c + x_range[from_y][0];

      // Find which piece is at (from_x, from_y).
      move.penguin_idx = -1;
      for (int i = 0; i < state.n_penguins; i++) {
        if ((player.penguin[i].x == from_x) && (player.penguin[i].y == from_y)) {
          move.penguin_idx = i;
        }
      }
      if (move.penguin_idx == -1) {
        goto missing_or_invalid_move;
      }
    }
    
    if (!make_move(move, &state)) {
      goto missing_or_invalid_move;
    }
  }

 missing_or_invalid_move:
  // Send the updated game state.
  PostMessage(pp::Var(toJSON(state)));

  while (state.cur_player_idx == 1) {
    // Now the AI must move.
    move_t ai_move;
    uct_move(10000, state, &ai_move);
    make_move(ai_move, &state);

    PostMessage(pp::Var(toJSON(state)));
  }
}

class HtmfModule : public pp::Module {
public:
  HtmfModule() : pp::Module() { }
  virtual ~HtmfModule() { }

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new HtmfInstance(instance);
  }
};
}  // namespace htmf_module


namespace pp {
Module* CreateModule() {
  return new htmf_module::HtmfModule();
}
}  // namespace pp
