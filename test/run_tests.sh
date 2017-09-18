#!/bin/bash

make -C .. clean test >/dev/null
if [ $? != 0 ]
then
    exit 1
fi

pushd ../bin >/dev/null 2>/dev/null
shell=`pwd`/shell
popd >/dev/null 2>/dev/null

tests_passed=0
tests_skipped=0
tests_failed=0

function run_if_exists {
    if [ -f "$1" ]
    then
	"$1"
    fi
}

function run_test {
    echo Running test $1...
    
    pushd $1 >/dev/null 2>/dev/null
    local test_src=`pwd`
    if [ -f skip ]
    then
	tests_skipped=$(( tests_skipped + 1))
	echo -e "\e[33mTest $1 skipped.\e[0m"
	popd >/dev/null 2>/dev/null
	return
    fi
    popd >/dev/null 2>/dev/null

    # 0 for pass, 1 for fail
    local test_result=0
    local test_out=`mktemp`
    local test_err=`mktemp`
    local test_root=`mktemp -d`
    pushd "$test_root" >/dev/null 2>/dev/null

    run_if_exists "$test_src/setup.sh"
    "$shell" <"$test_src/commands.txt" >"$test_out" 2>"$test_err"

    TESTROOT="$test_root" envsubst < "$test_src/out.txt" | diff --color=always - "$test_out"
    if [ $? == 1 ]
    then
	echo -e "\e[31mOutput differs!\e[0m"
	test_result=1
    fi
       
    TESTROOT="$test_root" envsubst < "$test_src/err.txt" | diff --color=always - "$test_err"
    if [ $? == 1 ]
    then
	echo -e "\e[31mError output differs!\e[0m"
	test_result=1
    fi

    run_if_exists "$test_src/expect.sh"
    if [ $? != 0 ]
    then
	echo -e "\e[31mFailed an expectation.\e[0m"
	test_result=1
    fi

    if [ $test_result == 0 ]
    then
	echo -e "\e[32mTest $1 passed.\e[0m"
	tests_passed=$(( tests_passed + 1))
    else
	echo -e "\e[31mTest $1 failed.\e[0m"
	tests_failed=$(( tests_failed + 1))
    fi

    popd >/dev/null 2>/dev/null
    rm -rf "$test_root" "$test_out" "$test_err"
}

for test in */commands.txt; do
    run_test `dirname "$test"`
    echo
done

echo -e "\e[32m$tests_passed\e[0m tests passed. \e[33m$tests_skipped\e[0m tests skipped. \e[31m$tests_failed\e[0m tests failed."
