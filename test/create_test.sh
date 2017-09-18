#!/bin/bash

for test in "$@"
do
    mkdir $test
    pushd $test >/dev/null 2>/dev/null

    echo \#!/bin/bash>setup.sh
    echo \# This script runs in the test root before the shell is started.>>setup.sh
    echo \# Use it to create any files or folders your test needs.>>setup.sh
    echo \# You can delete this file if you do not need it.>>setup.sh
    chmod +x setup.sh

    # Start out with the test being skipped, since it isn't written yet.
    touch skip
    
    touch commands.txt
    echo "\$TESTROOT\$ ">out.txt
    touch err.txt

    echo \#!/bin/bash>expect.sh
    echo \# This script runs in the test root after the shell has exited.>>expect.sh
    echo \# Use it to check any expectations on the state of the filesystem, etc.>>expect.sh
    echo \# If this script returns a non-zero exit code, the test will fail.>>expect.sh
    echo \# This script should emit a helpful message to stdout in the case of a failure>>expect.sh
    echo \# You can delete this file if you do not need it.>>expect.sh
    chmod +x expect.sh

    popd >/dev/null 2>/dev/null
done
