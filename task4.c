#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char *argv[]) {

  if(argc != 3)
  {
    fprintf(stderr, "Invalid number of parameters!\nThe 1st parameter - the name of file to read\nThe 2nd parameter - amount of rows in group.\nIf you enter 0, the text will be output in one block.\n");
    fprintf(stderr, "The format of the command should be: %s The_1st_parameter The_2nd_parameter\n", argv[0]);
    return 1;
  }

  errno = 0;
  char *endptr;

  long row_count = strtol(argv[2], &endptr, 10);

  if((errno = ERANGE && (row_count == LONG_MAX || row_count == LONG_MIN)) || (errno != 0 && row_count == 0))
  {
    perror("strtol");
    return 1;
  }
  else if(row_count < 0)
  {
    fprintf(stderr, "You entered a number less than 0.\n");
    return 1;
  }
  else if(endptr == argv[2])
  {
    fprintf(stderr, "There are no numbers in the line or you entered an incorrect parameter.\n");
    return 1;
  }
  else if(endptr[0] != '\0')
  {
    fprintf(stderr, "You entered an incorrect number.\n");
    return 1;
  }

  FILE *file = fopen(argv[1], "r");

  if(file == NULL)
  {
    fprintf(stderr, "There were problems, when opening, maybe the file doesn't exist.\n");
    return 1;
  }

  char symb;
  long rows = 0;

  while((symb = fgetc(file)) != EOF)
  {
    if(rows == 0 || rows != row_count)
    {
      if(symb == '\n')
      {
        putc('\n', stdout);
        rows++;
      }
      else
        putc(symb, stdout);
    }
    else
    {
      getc(stdin);
      putc(symb, stdout);
      rows = 0;
    }

  }

  if(fclose(file) != 0)
  {
    perror("fclose");
    return 1;
  }

  return 0;
}
