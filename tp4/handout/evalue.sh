#!/bin/sh
gcc GestionMemoire.c -Llibs -leval -o answer;
./answer > grader/result.log; # so grader can interpret the output and show the result on autolab
rm answer;
cat grader/result.log;
python3 grader/grade.py;
rm grader/result.log;