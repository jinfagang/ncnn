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

#ifndef LAYER_MAKEPADMASK_H
#define LAYER_MAKEPADMASK_H

#include "layer.h"

namespace ncnn {

class MakePadMask : public Layer {
public:
    MakePadMask();

    virtual int forward(const Mat &bottom_blob, Mat &top_blob,
                        const Option &opt) const;
};

}  // namespace ncnn

#endif  // LAYER_MAKEPADMASK_H
