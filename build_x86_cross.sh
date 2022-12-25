mkdir build
cd build
cmake .. -DNCNN_INT8=OFF -DNCNN_BUILD_EXAMPLES=OFF
make -j32