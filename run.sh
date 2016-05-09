#!/bin/bash
./findminmax_seq 1 400000000
./findminmax_par1 1 400000000 1
./findminmax_par1 1 400000000 2
./findminmax_par1 1 400000000 4
# ./findminmax_par2 1 400000000 1
# ./findminmax_par2 1 400000000 2
# ./findminmax_par2 1 400000000 4
# ./findminmax_par1 1 400000000 3
