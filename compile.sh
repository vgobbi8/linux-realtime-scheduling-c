gcc -O2 -o setup setup.c -lrt -lpthread
gcc -O2 -o worker worker.c -lrt -lpthread
gcc -O2 -o dump dump.c -lrt

cp worker task1
cp worker task2
cp worker task3
cp worker task4
cp worker task5

#reset memory
./setup



