mkdir -p bin

mkdir -p rkasm/build
cd rkasm/build
cmake ..
make
cd ../..
ln -s ./rkasm/build/rkasm ./bin/rkasm

mkdir -p rkemu/build
cd rkemu/build
cmake ..
make
cd ../..
ln -s ./rkemu/build/rkemu ./bin/rkemu

mkdir -p rkcc/build
cd rkcc/build
cmake ..
make
cd ../..
ln -s ./rkcc/build/rkcc ./bin/rkcc
