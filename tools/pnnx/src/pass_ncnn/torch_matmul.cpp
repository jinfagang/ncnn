// Tencent is pleased to support the open source community by making ncnn
// available.
//
// Copyright (C) 2022 THL A29 Limited, a Tencent company. All rights reserved.
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

class torch_matmul : public GraphRewriterPass {
public:
    const char *match_pattern_graph() const {
        return R"PNNXIR(7767517
4 3
pnnx.Input              input       0 1 input
pnnx.Input              other       0 1 other
torch.matmul            op_0        2 1 input other out
pnnx.Output             output      1 0 out
)PNNXIR";
  }

  const char *type_str() const { return "MatMul"; }

  const char *name_str() const { return "matmul"; }

  void write(
      Operator * /*op*/,
      const std::map<std::string, Parameter> & /*captured_params*/) const {}
};

REGISTER_GLOBAL_PNNX_NCNN_GRAPH_REWRITER_PASS(torch_matmul, 20)

}  // namespace ncnn

}  // namespace pnnx
