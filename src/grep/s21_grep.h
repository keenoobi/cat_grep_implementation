#ifndef S21_GREP_H_
#define S21_GREP_H_

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOWED_FLAGS "e:ivclnhsf:o"
#define FLAGS_COUNT 6
#define BUFFER 10000

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int fail;
  char pattern[BUFFER];
} flags;

void parser(int argc, char *argv[], flags *f);
void iter_files(int argc, char *argv[], flags *f);
void print_file(FILE *file, flags *f, char *argv, int argc);
void flag_o(regmatch_t *match, regex_t *reg, char *line);
void flag_f(char *filename, flags *f);
#endif