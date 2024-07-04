set -e
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_MAKE_PROGRAM=/usr/bin/make -B build -S . -DCMAKE_TOOLCHAIN_FILE=OFF
cmake --build ./build
exec ./build/server "$@"