#!/bin/bash
# 1. Setup Shared Memory
./setup

echo "Starting Deadline Monotonic (DM)..."

# Note the priorities are inverted compared to RM!

chrt -f 90 ./task1 1 2 50 &
chrt -f 80 ./task2 2 3 40 &
chrt -f 70 ./task3 3 7 30 &
chrt -f 60 ./task4 4 8 45 &
chrt -f 50 ./task5 5 9 20 &

wait
echo "DM Simulation Finished."

# SAVE THE RESULT
./dump