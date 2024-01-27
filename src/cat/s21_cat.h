#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <stdio.h>
#include <string.h>

#define ALLOWED_FLAGS 'b', 'e', 'n', 's', 't', 'v', 'E', 'T', 'u'
#define ALLOWED_LONG_FLAGS "-number-nonblank", " ", "-number", "-squeeze-blank"
#define FLAGS_COUNT 9

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int index;
  int nlc;
  int lnb;
  int ileb;
  int lnn;
  int ieln;
  int ffl;
  int lf;
} flags;

void looking_for_flags(int argc, char *argv[], flags *f);
void flag_validation(char *flag, flags *f);
void long_flag_validation(char *flag, flags *f);
void set_field(int i, flags *f);
void iter_file(int argc, char *argv[], flags *f);
void print_file(FILE *file, flags *f);
void handle_char(unsigned char c, flags *f);
void handle_b_flag(unsigned char c, flags *f);
void handle_n_flag(unsigned char c, flags *f);
void handle_e_flag(unsigned char c, flags *f);
void handle_s_flag(unsigned char c, flags *f);
void handle_v_flag(unsigned char c, flags *f);
#endif