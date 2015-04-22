#!/bin/bash

NR_CPUS=`grep processor /proc/cpuinfo | wc -l`

pushd `dirname $0` > /dev/null
source autotest.config
cd ucore

if [ $NR_CPUS -ge 4 ]; then
    ./uCore_test -d $BUILD_DIR_I386 -t 20 -r -s ucore > /dev/null 2>&1 &
    PIDS[0]=$!
    ./uCore_test -t 45 -w 15 -d $BUILD_DIR_ARM -r -s all > /dev/null 2>&1 &
    PIDS[1]=$!
    ./uCore_test -d $BUILD_DIR_AMD64 -t 20 -r -s ucore > /dev/null 2>&1 &
    PIDS[2]=$!

    for i in {0..2}; do
	wait ${PIDS[i]}
    done
else
    # Resource limited. Run the tests one by one
    ./uCore_test -d $BUILD_DIR_I386 -t 20 -r -s ucore > /dev/null 2>&1
    ./uCore_test -t 45 -w 15 -d $BUILD_DIR_ARM -r -s all > /dev/null 2>&1
    ./uCore_test -d $BUILD_DIR_AMD64 -t 20 -r -s ucore > /dev/null 2>&1
fi

# Dump results to stdout
echo "========================= SUMMARY =========================="
printf "%-20s%-10s%-10s%-10s%-10s\n" "" passed failed broken total
tail -n 1 $BUILD_DIR_I386/test-result.latest/summary
tail -n 1 $BUILD_DIR_ARM/test-result.latest/summary
tail -n 1 $BUILD_DIR_AMD64/test-result.latest/summary
echo

echo "=========================== i386 ==========================="
head -n $[ `cat $BUILD_DIR_I386/test-result.latest/summary | wc -l` - 3 ] $BUILD_DIR_I386/test-result.latest/summary
echo
echo "=========================== ARM ============================"
head -n $[ `cat $BUILD_DIR_ARM/test-result.latest/summary | wc -l` - 3 ] $BUILD_DIR_ARM/test-result.latest/summary
echo
echo "========================== amd64 ==========================="
head -n $[ `cat $BUILD_DIR_AMD64/test-result.latest/summary | wc -l` - 3 ] $BUILD_DIR_AMD64/test-result.latest/summary

popd > /dev/null
