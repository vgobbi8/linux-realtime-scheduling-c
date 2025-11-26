#!/bin/bash
# 1. Setup Shared Memory
./setup

echo "Starting Rate Monotonic (RM)..."

# 2. Launch Tasks
# Syntax: chrt -f <PRIO> ./task<ID> <ID> <BURST> <PERIOD>
# Note: Burst values (2,3,7,8,9) come from the PDF "Computacao" column

chrt -f 50 ./task1 1 2 50 &
chrt -f 70 ./task2 2 3 40 &
chrt -f 80 ./task3 3 7 30 &
chrt -f 60 ./task4 4 8 45 &
chrt -f 90 ./task5 5 9 20 &

# Wait for them to finish
wait
echo "RM Simulation Finished."

# SAVE THE RESULT
OUTPUT_FILE=${1:-result_rm.bmp}
./dump "$OUTPUT_FILE"