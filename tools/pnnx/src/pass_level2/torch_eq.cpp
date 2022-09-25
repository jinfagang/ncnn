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

#include "pass_level2.h"

namespace pnnx {

class torch_eq : public GraphRewriterPass {
public:
    const char *match_pattern_graph() const {
        return R"PNNXIR(7767517
4 3
pnnx.Input              input_0     0 1 input
pnnx.Input              input_1     0 1 other
aten::eq                op_0        2 1 input other out
pnnx.Output             output      1 0 out
)PNNXIR";
  }

  const char *type_str() const { return "torch.eq"; }
};

REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_eq, 20)

class torch_eq_1 : public GraphRewriterPass {
 public:
  const char *match_pattern_graph() const {
    return R"PNNXIR(7767517
4 3
pnnx.Input              input_0     0 1 input
pnnx.Input              input_1     0 1 other
aten::__is__            op_0        2 1 input other out
pnnx.Output             output      1 0 out
)PNNXIR";
  }

  const char *type_str() const { return "torch.eq"; }
};

REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_eq_1, 20)

}  // namespace pnnx
