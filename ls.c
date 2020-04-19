#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

int check_arg(char *argv[], const char *opt) //argv를 체크해주는 함수
{
 int count = 0;
 while (*argv != '\0')
 {
  if (!strcmp(argv[count], opt))
  {
   return 1;
  }
  argv++;
 }
 return 0;
}

void main(int argc, char *argv[])
{
 DIR *dp;
 struct dirent *entry;
 struct passwd *lpwd;
 struct group *gpwd;
 struct stat statbuf;
 char *path;
 int count = 0;
 int opt_a;
 int opt_l;
 int opt_al;
 int i;

 if (argc <3)
 {
  path = "."; // 옵션이 있는 경우이다.                   
 }

 else
 {
  path = argv[1]; // 옵션이 없는 경우에는 argv[1]의 내부 목록을 출력한다.          
 }

 if ((dp = opendir(path)) == NULL) // directory open에 실패했을 때
 {
  fprintf(stderr, "Can't open directory: %s\n", argv[1]);
  return;
 }

 // 사용자로부터 입력받은 옵션 명령어를 체크한다.
 opt_a = check_arg(argv, "-a");
 opt_l = check_arg(argv, "-l");
 opt_al = check_arg(argv, "-al");

 while ((entry = readdir(dp)) != NULL) // 디렉토리의 하위 목록을 하나씩 읽는다.
 {
  if (!opt_a && !opt_al) // -l옵션일 때이다.           
  {
   if (entry->d_name[0] == '.')
   {
    continue; // -l옵션일 때에는 '.'로 시작하는 것은 생략한다.
   }
  }

  if (!opt_l && !opt_al) // -a옵션일 때이다.
  {
   printf(" %s\t", entry->d_name); // 이름을 출력한다.
  }

  if (opt_l || opt_al) // -l혹은 -al옵션일 때이다.
  {
   if (!opt_al)
   {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
     continue;
   }

   stat(entry->d_name, &statbuf);

   switch (statbuf.st_mode & S_IFMT)
   {
   case S_IFREG:   
    putchar('-');
    break;
   case S_IFDIR: 
    putchar('d');
    break;
   case S_IFIFO:  
    putchar('p');
    break;
   case S_IFLNK:
    putchar('l');
    break;
   }

   for (i = 0; i < 3; i++)
   {
    if (statbuf.st_mode & (S_IREAD >> (i * 3)))
     putchar('r');
    else
     putchar('-');
    if (statbuf.st_mode & (S_IWRITE >> (i * 3)))
     putchar('w');
    else
     putchar('-');
    if (statbuf.st_mode & (S_IEXEC >> (i * 3)))
     putchar('x');
    else
     putchar('-');
   }

   printf(" %d  ", statbuf.st_nlink);
   lpwd = getpwuid(statbuf.st_uid); // user이름을 불러온다.
   printf("%s ", lpwd->pw_name);
   gpwd = getgrgid(statbuf.st_gid); // group이름을 불러온다.     
   printf("%s ", gpwd->gr_name);
   printf(" %4d ", statbuf.st_size);
   printf(" %s\n", entry->d_name);

   while ((entry = readdir(dp)) != NULL)
   {
    if (!opt_al)
    {
     if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    }

    stat(entry->d_name, &statbuf);

    switch (statbuf.st_mode & S_IFMT)
    {
    case S_IFREG:  
     putchar('-');
     break;
    case S_IFDIR:  
     putchar('d');
     break;
    case S_IFIFO:
     putchar('p');
     break;
    case S_IFLNK: 
     putchar('l');
     break;
    }

    for (i = 0; i < 3; i++)
    {
     if (statbuf.st_mode & (S_IREAD >> (i * 3)))
      putchar('r');
     else
      putchar('-');
     if (statbuf.st_mode & (S_IWRITE >> (i * 3)))
      putchar('w');
     else
      putchar('-');
     if (statbuf.st_mode & (S_IEXEC >> (i * 3)))
      putchar('x');
     else
      putchar('-');
    }

    printf(" %d  ", statbuf.st_nlink);
    lpwd = getpwuid(statbuf.st_uid); // user이름을 불러온다.      
    printf("%s ", lpwd->pw_name);
    gpwd = getgrgid(statbuf.st_gid); // group이름을 불러온다.     
    printf("%s ", gpwd->gr_name);
    printf(" %4d ", statbuf.st_size);
    printf(" %s\n", entry->d_name);
   }
  }
 }
 closedir(dp);
 printf("\n");
} 

