@ECHO OFF
PUSHD build
cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake -DDEBUG_BREACH=True
POPD
cmake --build build
@ECHO ON