@ECHO OFF
PUSHD build
cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake -DDEBUG_BREACH=ON -DBUILD_BREACH_EDITOR=OFF
POPD
cmake --build build
@ECHO ON