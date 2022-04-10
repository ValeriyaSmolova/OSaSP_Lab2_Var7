#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

void output_data_dir(char *dir)
{

  DIR *StreamDir;

  if((StreamDir = opendir(dir)) == NULL)
  {
    perror("opendir");
    return;
  }

  errno = 0;
  struct dirent *Data;

  while((Data = readdir(StreamDir)) != NULL)
  {
    printf("%s\n", Data->d_name);
  }

  if((Data = readdir(StreamDir)) == NULL && errno != 0)
  {
    perror("readdir");
    return;
  }

  if(closedir(StreamDir) != 0)
  {
    perror("closedir");
    return;
  }

}

int main(int argc, char *argv[])
{
  char current_dir[256];

  if(getcwd(current_dir, 256) == NULL)
  {
    perror("getcwd");
    return 1;
  }

  printf("Current dir:\n");
  output_data_dir(current_dir);
  printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
  printf("Root dir:\n");
  output_data_dir("/");

  return 0;
}
