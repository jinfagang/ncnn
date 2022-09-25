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

static void deconvolution_pack8to4_avx(
    const Mat &bottom_blob, Mat &top_blob, const Mat &weight_data_packed,
    const Mat &bias_data, int kernel_w, int kernel_h, int dilation_w,
    int dilation_h, int stride_w, int stride_h, int activation_type,
    const Mat &activation_params, const Option &opt) {
    int outch = top_blob.c;

    const int kernel_extent_w = dilation_w * (kernel_w - 1) + 1;
    const int kernel_extent_h = dilation_h * (kernel_h - 1) + 1;

    const float *bias_data_ptr = bias_data;

    // num_output
    #pragma omp parallel for num_threads(opt.num_threads)
    for (int p = 0; p < outch; p++) {
        float *outptr = top_blob.channel(p);

        const int maxk = kernel_w * kernel_h;

        // shadowed variable for less openmp task args
        const int w = bottom_blob.w;
        const int h = bottom_blob.h;
        const int channels = bottom_blob.c;
        const int outw = top_blob.w;
        const int outh = top_blob.h;

        for (int i = 0; i < outh; i++) {
            for (int j = 0; j < outw; j++) {
                __m128 _sum = _mm_setzero_ps();

                if (bias_data_ptr) {
                    _sum = _mm_loadu_ps(bias_data_ptr + p * 4);
                }

                const float *kptr = weight_data_packed.channel(p);

                // channels
                for (int q = 0; q < channels; q++) {
                    const Mat m = bottom_blob.channel(q);

                    for (int y = 0; y < kernel_h; y++) {
                        int sys = (i + y * dilation_h - (kernel_extent_h - 1));
                        if (sys < 0 || sys % stride_h != 0) continue;

                        int sy = sys / stride_h;
                        if (sy >= h) continue;

                        for (int x = 0; x < kernel_w; x++) {
                            int sxs = (j + x * dilation_w - (kernel_extent_w - 1));
                            if (sxs < 0 || sxs % stride_w != 0) continue;

                            int sx = sxs / stride_w;
                            if (sx >= w) continue;

                            const float *sptr = m.row(sy) + sx * 8;

                            int k = (y * kernel_w + x) * 32;

                            __m128 _val0 = _mm_broadcast_ss(sptr);
                            __m128 _val1 = _mm_broadcast_ss(sptr + 1);
                            __m128 _val2 = _mm_broadcast_ss(sptr + 2);
                            __m128 _val3 = _mm_broadcast_ss(sptr + 3);
                            __m128 _val4 = _mm_broadcast_ss(sptr + 4);
                            __m128 _val5 = _mm_broadcast_ss(sptr + 5);
                            __m128 _val6 = _mm_broadcast_ss(sptr + 6);
                            __m128 _val7 = _mm_broadcast_ss(sptr + 7);
                            __m128 _w0 = _mm_load_ps(kptr + k);
                            __m128 _w1 = _mm_load_ps(kptr + k + 4);
                            __m128 _w2 = _mm_load_ps(kptr + k + 8);
                            __m128 _w3 = _mm_load_ps(kptr + k + 12);
                            __m128 _w4 = _mm_load_ps(kptr + k + 16);
                            __m128 _w5 = _mm_load_ps(kptr + k + 20);
                            __m128 _w6 = _mm_load_ps(kptr + k + 24);
                            __m128 _w7 = _mm_load_ps(kptr + k + 28);
                            _sum = _mm_comp_fmadd_ps(_val0, _w0, _sum);
                            _sum = _mm_comp_fmadd_ps(_val1, _w1, _sum);
                            _sum = _mm_comp_fmadd_ps(_val2, _w2, _sum);
                            _sum = _mm_comp_fmadd_ps(_val3, _w3, _sum);
                            _sum = _mm_comp_fmadd_ps(_val4, _w4, _sum);
                            _sum = _mm_comp_fmadd_ps(_val5, _w5, _sum);
                            _sum = _mm_comp_fmadd_ps(_val6, _w6, _sum);
                            _sum = _mm_comp_fmadd_ps(_val7, _w7, _sum);
                        }
                    }

                    kptr += maxk * 32;
                }

                _sum = activation_sse(_sum, activation_type, activation_params);

                _mm_storeu_ps(outptr, _sum);
                outptr += 4;
            }
        }
    }
}
