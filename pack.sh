# Create build dir
rm -rf build
mkdir build
cd build
# Run CMake out-of-source and build deploy target
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=stage ..
cmake --build . --config Release --target deploy

echo Creating distribution README...
cd ..
./build_readme.sh
mv README.tmp build/stage/README.md

echo Creating package...
cd build/stage
cmake -E tar cfv ../pubfix.zip --format=zip *
