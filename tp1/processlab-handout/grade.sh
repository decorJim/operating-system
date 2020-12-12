#!/bin/bash

# grade.sh - Init Lab grader
rm -f /tmp/processlab_autograde /tmp/processlab_autograde_ltrace_q1 /tmp/processlab_autograde_strace_part2 /tmp/processlab_autograde_ltrace_part2 &> /dev/null
touch /tmp/processlab_autograde &> /dev/null
touch /tmp/processlab_autograde_ltrace_q1 &> /dev/null
touch /tmp/processlab_autograde_strace_part2 &> /dev/null
touch /tmp/processlab_autograde_ltrace_part2 &> /dev/null
(make &> /dev/null || (echo 'Error while compiling!' && exit 1)) && \

echo "" && \
echo "Question 1 output:" && \
echo "------------------" && \
ltrace -s 200 -o /tmp/processlab_autograde_ltrace_q1 -f ./processlab 1 && \

./processlab 0 &> /dev/null && \

echo "" && \
echo "Part 2 output:" && \
echo "--------------" && \

strace -s 200 -o /tmp/processlab_autograde_strace_part2 -v -f ./processlab 2 && \
NOREPORT=1 ltrace -s 200 -o /tmp/processlab_autograde_ltrace_part2 -f ./processlab 2 2>&1 > /dev/null && \

echo "" && \

./processlab 3 &> /dev/null && \
python3 grader/grade.py /tmp/processlab_autograde grader/params.yml /tmp/processlab_autograde_ltrace_q1 /tmp/processlab_autograde_strace_part2 /tmp/processlab_autograde_ltrace_part2 handout && \
rm -f part2Output.txt /tmp/processlab_autograde /tmp/processlab_autograde_ltrace_q1 /tmp/processlab_autograde_strace_part2 /tmp/processlab_autograde_ltrace_part2 &> /dev/null

exit
