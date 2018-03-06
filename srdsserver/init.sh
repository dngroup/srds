mkdir build
cd build
cmake ..
cd ..
cmake --build $(pwd)/build --target srdsserver -- -j 4