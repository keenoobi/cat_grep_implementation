#include "s21_cat.h"

int main(int argc, char *argv[]) {
  flags f = {0};
  looking_for_flags(argc, argv, &f);
  iter_file(argc, argv, &f);
  return 0;
}

void looking_for_flags(int argc, char *argv[], flags *f) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && f->index == 0) {
      long_flag_validation(&argv[i][1], f);
      flag_validation(&argv[i][1], f);
    } else if (f->index == 0 && f->ffl == 0) {
      f->index = i;
    }
  }
  if (argc == 1) {
    fprintf(stderr, "Command line has no additional arguments\n");
  }
}

void flag_validation(char *flag, flags *f) {
  if (*flag != '-') {
    char allowed_flags[FLAGS_COUNT] = {ALLOWED_FLAGS};
    for (; *flag && f->ffl != 2; flag++) {
      for (int i = 0; i < FLAGS_COUNT; i++) {
        if (*flag == allowed_flags[i]) {
          set_field(i, f);
          i = FLAGS_COUNT;
          f->ffl = 0;
        } else
          f->ffl = 2;
      }
      if (f->ffl == 2)
        fprintf(stderr,
                "cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]",
                *flag);
    }
  }
}

void long_flag_validation(char *flag, flags *f) {
  char *allowed_flags[4] = {ALLOWED_LONG_FLAGS};
  for (int i = 0; i < 4; i++) {
    if (strcmp(flag, allowed_flags[i]) == 0) {
      set_field(i, f);
      f->ffl = 0;
      i = 5;
    } else
      f->ffl = 3;
  }
  if (f->ffl == 3 && *flag == '-')
    fprintf(stderr,
            "cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]",
            *flag);
}

void set_field(int i, flags *f) {
  switch (i) {
    case 0:
      f->b = 1;
      f->n = 0;
      break;
    case 1:
      f->e = 1;
      f->v = 1;
      break;
    case 2:
      if (f->b == 0) f->n = 1;
      break;
    case 3:
      f->s = 1;
      break;
    case 4:
      f->t = 1;
      f->v = 1;
      break;
    case 5:
      f->v = 1;
      break;
    case 6:
      f->e = 1;
      break;
    case 7:
      f->t = 1;
      break;
  }
}

void iter_file(int argc, char *argv[], flags *f) {
  for (int i = f->index; i < argc && i != 0; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file) {
      print_file(file, f);
      fclose(file);
    } else
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
  }
}

void print_file(FILE *file, flags *f) {
  f->lnb = 1;   // Счетчик номера строки для -b
  f->ileb = 1;  // Флаг пустой строки для -b
  f->lnn = 1;   //Счетчик номера строки для -n
  f->ieln = 1;  //Флаг пустой строки для -n
  unsigned char c = fgetc(file);
  while (!feof(file)) {
    handle_char((unsigned char)c, f);
    if (f->t && c == '\t') {
      printf("^I");
    } else if (!f->v && f->nlc < 2) {
      putchar(c);
    }
    c = fgetc(file);
  }
}

void handle_char(unsigned char c, flags *f) {
  handle_s_flag(c, f);
  handle_b_flag(c, f);
  handle_n_flag(c, f);
  handle_e_flag(c, f);
  handle_v_flag(c, f);
}

void handle_b_flag(unsigned char c, flags *f) {
  if (f->b && f->nlc < 2) {
    if (c == '\n' && f->ileb == 0) {
      f->lnb++;
      f->ileb = 1;
    } else if (f->ileb == 1 && c != '\n') {
      printf("%6d\t", f->lnb);
      f->ileb = 0;
    }
  }
}

void handle_n_flag(unsigned char c, flags *f) {
  if (f->n && f->nlc < 2) {
    if (c == '\n' && f->ieln == 1) {
      printf("%6d\t", f->lnn);
      f->lnn++;
      f->ieln = 1;
    } else {
      if (f->ieln != 0) {
        printf("%6d\t", f->lnn);
        f->ieln = 0;
      } else if (c == '\n' && f->ieln == 0) {
        f->lnn++;
        f->ieln = 1;
      }
    }
  }
}

void handle_e_flag(unsigned char c, flags *f) {
  if (f->e && c == '\n' && f->nlc < 2) {
    printf("$");
  }
}

void handle_s_flag(unsigned char c, flags *f) {
  static int is_empty_line = 1;
  if (f->s) {
    if (c == '\n' && f->nlc < 3 && is_empty_line == 1) {
      f->nlc++;
    } else if (c == '\n' && is_empty_line == 0) {
      f->nlc = 0;
      is_empty_line = 1;
    } else {
      is_empty_line = 0;
      f->nlc = 0;
    }
  }
}

void handle_v_flag(unsigned char c, flags *f) {
  if (f->v && f->nlc < 2) {
    if (c > 127 && c < 160) printf("M-^");
    if ((c < 32 && c != '\n' && c != '\t') || c == 127) printf("^");
    if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t')
      c = c > 126 ? c - 128 + 64 : c + 64;
    if (!f->t || c != '\t') putchar(c);
  }
}