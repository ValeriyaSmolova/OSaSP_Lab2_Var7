#include <stdio.h>

const int Exit = 27;

int main(int argc, char *argv[]) {

  if(argc != 2)
  {
    fprintf(stderr, "Invalid parameters!\nThe_1st_parameter - the name of the file to which the result will be output.\n");
    fprintf(stderr, "The format of the command should be: %s The_1st_parameter\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "w");

  if(file == NULL)
  {
    perror("fopen");
    return 1;
  }

  char input;
  while((input = getc(stdin)) != Exit)
  {
    if(fputc(input, file) == EOF)
    {
      perror("fputc");
      return 1;
    }
  }

  if(fclose(file) != 0)
  {
    perror("fclose");
    return 1;
  }

  return 0;
}
