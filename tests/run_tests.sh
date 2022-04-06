#!/usr/bin/env bash

set -e

# in shell scripting, you don't leave white space after the assignment
# PROJECT_DIR_NAME= blabla.X --> INCORRECT!
# PROJECT_DIR_NAME=blabla.X --> CORRECT! 


PROJECT_DIR_NAME=the1.X

# without the "e" prepended!
SEVEN_DIGIT_STDT_NUMBER=2310142

rm -rf *.render

echo "---build debug---"
cd ..
make TYPE_IMAGE=DEBUG_RUN
cd tests
echo "---build debug complete---"

echo "---populate debug scripts with label addresses etc.---"
# the assembly file should be ../the_1_1234567.s relative to here!
python3 populate_tests.py "${PROJECT_DIR_NAME}" "${SEVEN_DIGIT_STDT_NUMBER}"
echo "---populate debug scripts with label addresses etc. complete---"

echo "---msec1000_passed_test START---"
mdb msec1000_passed_test.dbg.render 2> /dev/null
echo "---msec1000_passed_test END---"
echo "---msec500_passed_test START---"
mdb msec500_passed_test.dbg.render 2> /dev/null
echo "---msec500_passed_test END---"
echo "---msec500_passed_test_ra4_hold START---"
mdb msec500_passed_test_ra4_hold.dbg.render 2> /dev/null
echo "---msec500_passed_test_ra4_hold END---"
echo "---msec500_passed_test_re4_hold START---"
mdb msec500_passed_test_re4_hold.dbg.render 2> /dev/null
echo "---msec500_passed_test_re4_hold END---"

