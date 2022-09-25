// Tencent is pleased to support the open source community by making ncnn
// available.
//
// Copyright (C) 2021 THL A29 Limited, a Tencent company. All rights reserved.
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

#include "../utils.h"
#include "pass_level1.h"

namespace pnnx {

class ReflectionPad2d : public FuseModulePass {
public:
    const char *match_type_str() const {
        return "__torch__.torch.nn.modules.padding.ReflectionPad2d";
    }

    const char *type_str() const {
        return "nn.ReflectionPad2d";
    }

    void write(Operator *op,
               const std::shared_ptr<torch::jit::Graph> &graph) const {
        const torch::jit::Node *pad = find_node_by_kind(graph, "aten::pad");
        const torch::jit::Node *reflection_pad2d =
            find_node_by_kind(graph, "aten::reflection_pad2d");

        if (pad) {
            op->params["padding"] = pad->namedInput("pad");
        } else {
            op->params["padding"] = reflection_pad2d->namedInput("padding");
        }
    }
};

REGISTER_GLOBAL_PNNX_FUSE_MODULE_PASS(ReflectionPad2d)

}  // namespace pnnx
