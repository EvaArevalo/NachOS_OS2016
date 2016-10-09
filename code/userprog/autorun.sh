# go back and build
cd ../build.linux
make clean
make

# go to test and run test
cd ../test
make clean 
make 

echo 'testhalt'

../build.linux/nachos -e halt

echo 'testadd'

../build.linux/nachos -e add

echo 'test1'

../build.linux/nachos -e consoleIO_test1

echo 'test2'

../build.linux/nachos -e consoleIO_test2

make clean

# come back to userprogram
cd ../userprog
