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

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

namespace htmf_module {

class HtmfInstance : public pp::Instance {
public:
  explicit HtmfInstance(PP_Instance instance) : pp::Instance(instance) { }
  virtual ~HtmfInstance() {}

  virtual void HandleMessage(const pp::Var& var_message);
};

void HtmfInstance::HandleMessage(const pp::Var& var_message) {
  PostMessage(pp::Var("{\n"
                      "  board: [\n"
                      "    '0000000000000',\n"
                      "    '0000223D11130',\n"
                      "    '0000223121200',\n"
                      "    '0001C23B22200',\n"
                      "    '0001A2A121000',\n"
                      "    '0012221111000',\n"
                      "    '0013311310000',\n"
                      "    '02321C2230000',\n"
                      "    '012131DB00000',\n"
                      "    '0000000000000',\n"
                      "  ]\n"
                      "}"));
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
