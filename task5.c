#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

  if(argc != 3)
  {
    fprintf(stderr, "Invalid number of parameters!\nThe 1st parameter - file to copy.\nThe 2nd parameter - file to which another file is being copied.\n");
    fprintf(stderr, "The format of the command should be: %s The_1st_parameter The_2nd_parameter\n", argv[0]);
    return 1;
  }

  FILE *file1 = fopen(argv[1], "r");
  if(file1 == NULL)
  {
    perror("fopen");
    return 1;
  }

  FILE *file2 = fopen(argv[2], "w");
  if(file2 == NULL)
  {
    perror("fopen");
    return 1;
  }

  struct stat buff;

  if(stat(argv[1], &buff) == 0)
  {
    if(chmod(argv[2], buff.st_mode) != 0)
    {
      perror("chmod");
      return 1;
    }
  }
  else {
    perror("stat");
    return 1;
  }

  char copy;
  while((copy = fgetc(file1)) != EOF)
  {
    if(fputc(copy, file2) == EOF)
    {
      perror("fputc");
      return 1;
    }
  }

  if((fclose(file1) != 0) || (fclose(file2) != 0))
  {
    perror("fclose");
    return 1;
  }

  return 0;
}
