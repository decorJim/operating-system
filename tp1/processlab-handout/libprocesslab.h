/*
 * Init Lab - libprocesslab.h
 * Not visible to students
 * 
 * Ecole polytechnique de Montreal, 2020
 */

#ifndef _LIBPROCESSLAB_H
#define _LIBPROCESSLAB_H

#include <sys/types.h>

void registerProc(int procLevel, int procNum, pid_t pid, pid_t ppid);
void initLab();
void startQuestion(void (*questionFunc)(void), int partNum, int quietMode);
void truncateStr(char* destCharArray, char* str, int strLen);
int compareStrings(char* s1, char* s2, int maxLen);
void report(char* reportStr);

#endif