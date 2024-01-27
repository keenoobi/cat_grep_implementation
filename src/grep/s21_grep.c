#include "s21_grep.h"

int main(int argc, char *argv[]) {
  flags f = {0};
  parser(argc, argv, &f);
  iter_files(argc, argv, &f);
  return 0;
}

void parser(int argc, char *argv[], flags *f) {
  int c;
  char *allowed_flags = ALLOWED_FLAGS;
  while ((c = (getopt_long(argc, argv, allowed_flags, 0, 0))) != -1) {
    switch (c) {
      case 'e':
        f->e = 1;
        strcat(f->pattern, optarg);
        strcat(f->pattern, "|");
        break;
      case 'i':
        f->i = 1;
        break;
      case 'v':
        f->v = 1;
        f->o = 0;
        break;
      case 'c':
        f->c = 1;
        f->n = 0;
        f->o = 0;
        break;
      case 'l':
        f->l = 1;
        f->o = 0;
        f->n = 0;
        break;
      case 'n':
        if (!f->c && !f->l) f->n = 1;
        break;
      case 'h':
        f->h = 1;
        break;
      case 's':
        f->s = 1;
        break;
      case 'f':
        f->f = 1;
        flag_f(optarg, f);
        break;
      case 'o':
        if (!f->v && !f->c && !f->l) f->o = 1;
        break;
      default:
        f->fail = 1;
        fprintf(
            stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n\t[-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n\t[--context[=num]] [--directories=action] "
            "[--label] [--line-buffered]\n\t[--null] [pattern] [file ...]\n");
    }
  }
  if (!f->e && !f->f) {
    if ((argc > optind)) {
      strcat(f->pattern, argv[optind]);
    }
    optind++;
  }
  if (f->e || f->f) f->pattern[strlen(f->pattern) - 1] = '\0';
}

void iter_files(int argc, char *argv[], flags *f) {  // iter_files
  for (int i = optind; i < argc && f->fail != 1; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file) {
      print_file(file, f, argv[i], argc);
      fclose(file);
    } else if (!f->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);  // s
  }
}

void print_file(FILE *file, flags *f, char *argv, int argc) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  regex_t reg;
  regmatch_t start = {0};
  int counter = 0;
  int ncounter = 0;
  int match;
  int lastoutputline = -1;
  int regext = REG_EXTENDED;
  if (f->i) regext |= REG_ICASE;  // i
  regcomp(&reg, f->pattern, regext);
  while ((read = getline(&line, &len, file)) != -1) {
    ncounter++;
    match = regexec(&reg, line, 1, &start, 0);
    if (f->v) match = !match;   // v
    if (match == 0) counter++;  // c
    if (((argc - optind) > 1) && match == 0 && !f->c && !f->h && !f->l)
      printf("%s:", argv);                            // l
    if (f->n && match == 0) printf("%d:", ncounter);  // n
    if (f->o) flag_o(&start, &reg, line);
    if (!match && !f->c && !f->l && !f->o) {
      printf("%s", line);
      lastoutputline = ncounter;
    }
  }
  regfree(&reg);
  if (f->c && !f->l) {  // флаг c
    if (((argc - optind) > 1) && !f->h) printf("%s:", argv);
    printf("%d\n", counter);
  }
  if (counter && f->l && !f->c) printf("%s\n", argv);  // l
  if (f->c && f->l) {  // сочетания флагов lc
    if ((argc - optind) > 1) printf("%s:", argv);
    printf("%d\n", counter ? 1 : 0);
    if (counter) printf("%s\n", argv);
  }
  if (lastoutputline == ncounter) printf("\n");
  if (line) free(line);
}

void flag_o(regmatch_t *match, regex_t *reg, char *line) {
  char *remainingLine = line;
  while (!regexec(reg, remainingLine, 1, match, 0)) {
    printf("%.*s\n", (int)(match->rm_eo - match->rm_so),
           remainingLine + match->rm_so);
    remainingLine += match->rm_eo;
  }
}

void flag_f(char *filename, flags *f) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE *file = fopen(filename, "r");
  if (file) {
    while ((read = getline(&line, &len, file)) != -1) {
      if (strchr(line, '\n')) *(strchr(line, '\n')) = '\0';
      strcat(f->pattern, line);
      strcat(f->pattern, "|");
    }
    fclose(file);
  } else if (!f->s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
    f->fail = 1;
  }
  if (line) free(line);
}