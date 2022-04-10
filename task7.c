#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>

int scaning(char *dir_name, int min_size, int max_size);
void dublfiles();

typedef char Path[PATH_MAX];
Path *path_file;
int max_count_files = 8;
int path_file_count = 0;

int main(int argc, char *argv[]) {

  if(argc != 4)
  {
    fprintf(stderr, "Invalid parameters!\nThe_1st_parameter - the name of the directory to start search.\nThe_2nd_parameter - minimal size of the file in bytes.\nThe_3rd_parameter - maximal size of the file in bytes.\n");
    fprintf(stderr, "The format of the command should be: %s The_1st_parameter The_2nd_parameter The_3rd_parameter\n", argv[0]);
    return 1;
  }

  errno = 0;
  char *endptr, *endptr1;

  int min = strtol(argv[2], &endptr, 10);
  int max = strtol(argv[3], &endptr1, 10);

  if((errno = ERANGE && ((min == LONG_MAX || min == LONG_MIN) || (max == LONG_MAX || max == LONG_MIN))) || (errno != 0 && ((min == 0) || (max == 0))))
  {
    perror("strtol");
    return 1;
  }
  else if(min < 0 || max < 0)
  {
    fprintf(stderr, "You entered a number less than 0.\n");
    return 1;
  }
  else if(endptr == argv[2] || endptr1 == argv[3])
  {
    fprintf(stderr, "You entered incorrect size.\n");
    return 1;
  }
  else if(endptr[0] != '\0' || endptr1[0] != '\0')
  {
    fprintf(stderr, "You entered an incorrect number.\n");
    return 1;
  }
  else if(min > max)
  {
    fprintf(stderr, "The size range should be: from small to large.\n");
    return 1;
  }

  path_file = calloc(max_count_files, sizeof(Path));
  if(path_file == NULL)
  {
    perror("calloc");
    return 1;
  }

  scaning(argv[1], min, max);
  dublfiles();

  free(path_file);

  return 0;

}

int scaning(char *dir_name, int min_size, int max_size)
{
  DIR *StartDir;

  if((StartDir = opendir(dir_name)) == NULL)
  {
    perror("opendir");
    return 1;
  }

  errno = 0;
  struct dirent *Data;

  while((Data = readdir(StartDir)) != NULL)
  {
    Path filepath;
    sprintf(filepath, "%s/%s", dir_name, Data->d_name);

    if(Data->d_type == DT_REG)
    {
      struct stat inform;
      stat(filepath, &inform);

      if(inform.st_size >= min_size && inform.st_size <= max_size)
      {
        if(max_count_files == path_file_count)
        {
          max_count_files += max_count_files;
          path_file = realloc(path_file, max_count_files * sizeof(Path));
          if(path_file == NULL)
          {
            perror("realloc");
            return 1;
          }
        }

        strcpy(path_file[path_file_count++], filepath);
      }


    }
    else if(Data->d_type == DT_DIR && strcmp(".", Data->d_name) && strcmp("..", Data->d_name))
    {
      scaning(filepath, min_size, max_size);
    }
  }

  if((Data = readdir(StartDir)) == NULL && errno != 0)
  {
    perror("readdir");
    return 1;
  }

  if(closedir(StartDir) != 0)
  {
    perror("closedir");
    return 1;
  }

  return 0;
}

void dublfiles()
{
  int dubl = 0;
  int ind[100];
  int l = 0;
  int flag = 0;

  for (int i = 0; i < path_file_count; i++)
  {

    for (int k = 0; k < l; k++)
    {
      if(ind[k] == i)
      {
        flag = 1;
        goto Start;
      }

    }

    Start:
      if(flag == 0)
      {
        int hasdubl = 0;
        FILE *file1 = fopen(path_file[i], "r");
        if(file1 == NULL)
          continue;
        for (int j = i + 1; j < path_file_count; j++)
        {
          fseek(file1, SEEK_SET, 0);

          FILE *file2 = fopen(path_file[j], "r");
          if(file2 == NULL)
            continue;

          char c1, c2;
          do
          {
            c1 = fgetc(file1);
            c2 = fgetc(file2);
          } while (c1 != EOF && c2 != EOF && c1 == c2);

          if(c1 == c2 && c1 == EOF)
          {
            if(dubl == 0)
            {
              dubl = 1;
              printf("Dublicates:\n\n");
            }
            if(hasdubl == 0)
            {
              printf("\n%s", path_file[i]);
            }
            hasdubl++;
            printf(" = %s\n", path_file[j]);
            ind[l] = j;
            l++;

          }

          fclose(file2);
        }

        fclose(file1);
      }
  }

  if(dubl == 0)
  {
    printf("There are no dublicates.\n");
  }
}
