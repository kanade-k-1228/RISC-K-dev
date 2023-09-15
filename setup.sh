mkdir -p rkasm/build
cd rkasm/build
cmake ..
make
cd ../..

mkdir -p rkemu/build
cd rkemu/build
cmake ..
make
cd ../..

mkdir -p rkcc/build
cd rkcc/build
cmake ..
make
