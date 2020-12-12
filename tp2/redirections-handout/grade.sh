#!/bin/bash

# grade.sh - Init Lab grader
chmod -R 777 executables/
touch executables/Question2/tmpfile

rm -f /tmp/testlab_autograde &> /dev/null
touch /tmp/testlab_autograde &> /dev/null
(make &> /dev/null || (echo 'Error while compiling!' && exit 1)) && \

./redirections && \
[ -f ./progagrade.log ] && { cat ./progagrade.log >> /tmp/testlab_autograde; rm ./progagrade.log; }
[ -f ./progbgrade.log ] && { cat ./progbgrade.log >> /tmp/testlab_autograde; rm ./progbgrade.log; }
[ -f ./commandReport1.log ] && { cat ./commandReport1.log >> /tmp/testlab_autograde; rm ./commandReport1.log; }
[ -f ./commandReport2.log ] && { cat ./commandReport2.log >> /tmp/testlab_autograde; rm ./commandReport2.log; }
[ -f ./commandReport3.log ] && { cat ./commandReport3.log >> /tmp/testlab_autograde; rm ./commandReport3.log; }

echo "" && \

python3 grader/grade.py /tmp/testlab_autograde grader/params.yml server && \
rm -f /tmp/testlab_autograde ansq1 ansq2 executables/Question2/tmpfile /tmp/pipe_a /tmp/pipe_b &> /dev/null

exit
