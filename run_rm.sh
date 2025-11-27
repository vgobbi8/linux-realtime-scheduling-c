./setup

echo "Starting Rate Monotonic (RM)..."


chrt -f 50 ./task1 1 2 50 &
chrt -f 70 ./task2 2 3 40 &
chrt -f 80 ./task3 3 7 30 &
chrt -f 60 ./task4 4 8 45 &
chrt -f 90 ./task5 5 9 20 &

wait
echo "RM Simulation Finished."

OUTPUT_FILE=${1:-result_rm.bmp}
./dump "$OUTPUT_FILE"