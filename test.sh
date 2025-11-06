#!/bin/bash

# Simple test of the command interpreter

in="abbabaq"
out="0,3,5;"

[[ $(./cmd_int <<< "$in") == "$out"* ]] && echo "PASSED TEST 1" || echo "FAILED TEST 1"

# Other examples from assignment
in=""
out=";"

[[ $(./cmd_int <<< "$in") == "$out"* ]] && echo "PASSED TEST 2" || echo "FAILED TEST 2"

in="abccbaabcq"
out="5;"

[[ $(./cmd_int <<< "$in") == "$out"* ]] && echo "PASSED TEST 3" || echo "FAILED TEST 3"

# Edge cases

in="thisShouldEndImmediately" # check if ending with something other than q works correctly 
out=";" 

[[ $(./cmd_int <<< "$in") == "$out"* ]] && echo "PASSED TEST 4" || echo "FAILED TEST 4"

in="bbbathisShouldAlsoEndImmediately" # check if ending with something other than q works correctly when items have been added
out="3;"

[[ $(./cmd_int <<< "$in") == "$out"* ]] && echo "PASSED TEST 5" || echo "FAILED TEST 5"

in="bbbacc" # check if removing empty collection breaks program
out=";"

[[ $(./cmd_int <<< "$in") == "$out"* ]] && echo "PASSED TEST 6" || echo "FAILED TEST 6"

in="ababababababababababababababababababababa" # check if the collection can re-size properly for big inputs
out="0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40;"

[[ $(./cmd_int <<< "$in") == "$out"* ]] && echo "PASSED TEST 7" || echo "FAILED TEST 7"
