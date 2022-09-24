// Copyright (c) 2022 Xiaomi Corp.        (author: Fangjun Kuang)
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of the
// License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

#include "pass_ncnn.h"

namespace pnnx {

namespace ncnn {

class icefall_RelativePositionalEncoding : public GraphRewriterPass {
 public:
  const char *match_pattern_graph() const {
    return R"PNNXIR(7767517
3 2
pnnx.Input                    input       0 1 input
icefall.RelPositionalEncoding op_0        1 1 input pos_emb
pnnx.Output                   output      1 0 pos_emb
)PNNXIR";
  }

  const char *type_str() const { return "RelPositionalEncoding"; }

  const char *name_str() const { return "relpositionalencoding"; }

  void write(Operator *op,
             const std::map<std::string, Parameter> &captured_params,
             const std::map<std::string, Attribute> &captured_attrs) const {
    int w = (int)(captured_attrs.at("op_0.pe").shape[2]);
    int h = (int)(captured_attrs.at("op_0.pe").shape[1]);
    op->params["0"] = w;
    op->params["1"] = h;

    op->attrs["0"] = captured_attrs.at("op_0.pe");
  }
};

REGISTER_GLOBAL_PNNX_NCNN_GRAPH_REWRITER_PASS(
    icefall_RelativePositionalEncoding, 20)

}  // namespace ncnn

}  // namespace pnnx
