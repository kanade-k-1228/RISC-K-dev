cd build
make
cd ..

for i in 1 2 3
do
cat test/test$i.tc
rkcc test/test$i.tc
echo
done
