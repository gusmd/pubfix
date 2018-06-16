rm -rf build
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=stage ..
cmake --build . --config Release --target deploy
cd stage
cmake -E tar cfv ../pubfix.zip --format=zip *
