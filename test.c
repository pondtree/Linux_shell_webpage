#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <signal.h>                //필요한 헤더파일 정의

#define size 1024                  // size라는 것을 1024로 쓰겠다.

void mkdir_(int argc, char* argv)//mkdir p mode
{
 char* p_path;
 struct stat sd;

 p_path = strtok(argv, "/");// /로 구분
 while (p_path != NULL)//while문으로 /로 구분된 디렉토리 생성
 {
  if (stat(p_path, &sd) == -1)//디렉토리 존재 여부 확인
  {
   mkdir(p_path, 0755); //makedirectory
   chdir(p_path);
  }
  else
   chdir(p_path);

  p_path = strtok(NULL, "/");
 }
}

int rm(int argc, char *argv[]) //argc는 인자들의 갯수, argv는 인자들의 인덱스
{
 char y[5];
 if (argv[1] == NULL)
 {
  printf("NOT EXIST FILE NAME \n");
 }
 else if (argv[3] == NULL)
 {
  if (strchr(argv[2], '.') != NULL)
  {   //strchr 문자열 처리 ->string 라이브러리
   if (unlink(argv[2]))  //unlink() 함수는 파일을 삭제,성공시 True, fail 반환
    printf("Not exist file!\n");
   else
    printf("%s: file is deleted.\n", argv[1]);
  }
  else
   printf("rm: %s is incorrect file format, please use '-r' to remove directory.\n", argv[2]);
 } // argv[2]에서 옵션이 r이라면 하위폴더나 디렉토리 제거.
 else
 {
  if (strchr(argv[2], 'f') != NULL)  // -f 옵션: argv[2]에서 옵션이 f라면 모든 폴더나 디렉토리 제거.
  {
   if (strchr(argv[2], 'r') == NULL)
   {
    if (unlink(argv[3]))  // unlink는 매개변수를 받아서 파일을 삭제하는 함수.
     printf("Not exist file!\n");
    else
     printf("%s: File is deleted.\n", argv[3]);
   }
   else
   {
    if (rmdir(argv[3]))
    {
     printf("%s\n", argv[2]);
     printf("%s\n", argv[3]);
     printf("Not exist directory!\n");
    }
    else
     printf("%s: directory is deleted.\n", argv[2]);
   }
  }
  else if (strchr(argv[2], 'r') != NULL) //
  {
   if (rmdir(argv[3]))
    printf("Not exist directory!\n");
   else
    printf("%s: directory is deleted.\n", argv[2]);
  }
 }
}

void cd(int argc, char *argv[]) //cd
{
 char *path;

 if (argc > 2) path = argv[2]; //argv[2] 이 있을 경우 path로 지정
 else path = getenv("HOME"); // 없을 경우 getenv로 HOME 환경변수를 읽어서 path로 지정
 if (chdir(path) != 0); // path 로 이동
 system("pwd"); //printf("%s", path);
}
void cp(int argc, char *argv[])
{
 FILE *src;
 FILE *dst;
 char ch;
 char buf[200];
 struct stat info;     // 필요한 변수들을 정의해준다.
 if (argc < 4)      // 인자수가 충분하지 않다면
 {
  fprintf(stderr, "Not enough arguments.\n");
  return;
 }
 lstat(argv[3], &info);           //디렉토리 존재여부를 확인한다.
 if (S_ISDIR(info.st_mode)) // 파일을 복사해주는 작업을 한다.
 {
  getcwd(buf, sizeof(buf));  // 현재 작업 경로를 얻는다.
  src = fopen(argv[2], "r"); // src를 읽기 전용 파일로 해준다.
  chdir(argv[3]);
  dst = fopen(argv[2], "w"); // dst를 쓰기 전용 파일로 해준다.

  while (!feof(src))  // 파일을 다 읽을 때까지
  {
   ch = (char)fgetc(src);
   if (ch != EOF)
   {
    fputc((int)ch, dst);            //파일을 복사해준다.
   }
  }
  fclose(src);
  chdir(buf);
  fclose(dst);
 }
 else                       //같은 작업이다.
 {
  src = fopen(argv[2], "r");
  dst = fopen(argv[3], "w");
  while (!feof(src))
  {
   ch = (char)fgetc(src);
   if (ch != EOF)
   {
    fputc((int)ch, dst);
   }
  }
  fclose(src);
  fclose(dst);
 }
}

int get_pid_by_process_name(char *process_name)
{
 int pid = -1;
 char cmd_string[512];
 FILE *fp;

 sprintf(cmd_string, "ps | grep -w %s | grep -v grep", process_name);
 //grep으로 입력한 프로세스 이름을 걸러내어 찾아옴.
 //grep -v grep 은 grep 명령에 의한 결과 제거 하기 위해 붙여 넣음

 fp = popen(cmd_string, "r");  // popen으로 커맨드를 실행 , 결과 저장
 fseek(fp, 0, SEEK_SET);
 fscanf(fp, "%d", &pid);  // 지정된 결과에서 pid를 읽어들인다.

 fclose(fp);

 if (pid < 1)  // pid가 없는 경우이고, 그 경우 -1을 return
  pid = -1;

 return pid;  // pid값을 리턴
}

int kill_(int argc, char* argv[])
{
 if (argc == 3) // 입력받은 값의 개수가 3개 일때
 {
  kill(atoi(argv[2]), SIGKILL);  // SIGKILL신호를 보내서 argv[2]에 해당하는 pid를 죽인다.
 }
 else if (argc == 4)   // 입력 받은 값의 개수가 4개 일때
 {
  while (1)   // 무한 루프문을 돌려서 다 지워질 때 까지 돌린다.
  {
   int kk = get_pid_by_process_name(argv[3]);  // get_pid_by_process_name함수에서 값을 받아와 저장한다.
   if (kk == -1)
   {
    break;  // 프로세스 아이디가 없는 것이기 때문에 종료한다.
   }
   else
   {
    kill(kk, SIGKILL);  // 해당 프로세스 아이디를 kill한다.
   }
  }
 }
 else
 {
  printf("Usage :  ./kill pid or ./kill all process_name\n");   // 다른 형식으로 받은 경우 출력
 }
 return 0;
}


void mv(int argc, char *argv[])       //cp와 함수가 거의 비슷하다.
{
 FILE *src;
 FILE *dst;

 char ch;
 char buf[200];
 struct stat info;   // 필요한 변수들을 정의해준다.

 if (argc < 4) // 인자수가 충분하지 않다면
 {
  fprintf(stderr, "Not enough arguments.\n");
  return;
 }
 lstat(argv[3], &info); // 디렉토리 존재 여부를 확인한다.
 if (S_ISDIR(info.st_mode))// 파일을 복사해주는 작업을 한다.
 {
  getcwd(buf, sizeof(buf)); // 현재 작업 경로를 얻는다.
  src = fopen(argv[2], "r");     // src를 읽기 전용 파일로 해준다.
  chdir(argv[3]);
  dst = fopen(argv[2], "w");    // dst를 쓰기 전용 파일로 해준다.
  while (!feof(src))  // 파일을 다 읽을 때까지
  {
   ch = (char)fgetc(src);
   if (ch != EOF)
   {
    fputc((int)ch, dst);   //파일을 복사해준다.
   }
  }
  fclose(src);
  chdir(buf);
  fclose(dst);
 }
 else                    //같은 작업이다.
 {
  src = fopen(argv[2], "r");
  dst = fopen(argv[3], "w");
  while (!feof(src))
  {
   ch = (char)fgetc(src);
   if (ch != EOF)
   {
    fputc((int)ch, dst);
   }
  }
  fclose(src);
  fclose(dst);
 }
 unlink(argv[2]);
}

void tar(int argc, char *argv[])        //파일이나 폴더를 압축해주는 작업
{
 char buf[100];
 if (argc < 4)                 //인자수가 충분하지 않다면
 {
  printf("Not enough arguments\n");
  return;
 }
 if (argc == 4)                //압축해주는 명령어를 환경변수 buf에 저장
 {
  sprintf(buf, "tar %s %s", argv[2], argv[3]);
 }
 if (argc == 5)               //압축해제해주는 명령어를 환경변수 buf에 저장
 {
  sprintf(buf, "tar %s %s %s", argv[2], argv[3], argv[4]);
 }
 system(buf);                 //명령어 실행
}

int main(int argc, char* argv[])
{
 int i;
 int mode;
 int childpid;
 char current[size];
 char* path;
 struct stat sb;

 pid_t pid;
 pid = fork(); //자식 프로세스 생

 if (pid == 0) //자식 프로세스 이면
  childpid = getpid(); //childpid에 id 저장

 if (childpid == getpid())//자식 프로세스 일때 실행
 {
  if (strcmp(argv[1], "mkdir") == 0)//mkdir
  {
   if (strcmp(argv[2], "-m") == 0)//m mode
   {
    int access = strtol(argv[3], NULL, 8); //권한을 받고 8진수로 변환
    mkdir(argv[4], access);//mkdirectory
    chmod(argv[4], access);//권한 변경
   }
   else if (strcmp(argv[2], "-p") == 0)//p mode
    mkdir_(argc, argv[3]);//go to mkdir_ function
   else//no option
   {
    if (stat(argv[2], &sb) == -1)
    {
     mkdir(argv[2], 0755);//mkdirectory
     chmod(argv[2], 0755); //권한 변경         
    }
    else//파일이 이미 있을 경우
     printf("directory is already exist");
   }
  }
  else if (strcmp(argv[1], "rm") == 0) //rm
   rm(argc, argv);
  else if (strcmp(argv[1], "pwd") == 0) //pwd
  {
   char buff[1024];
   if (getcwd(buff, size) == NULL) // 현재 경로 길이가 NULL로 반환되면 실행 
   {
    printf("getcwd Failed!\n");
    exit(1);              // if문을 빠져나간다.
   }
   printf("%s\n", buff); // 현재 경로를 표시한다.
   return 0;
  }
  
  else if (strcmp(argv[1], "cd") == 0)//cd
   cd(argc, argv);
  else if (strcmp(argv[1], "cp") == 0)//cp
   cp(argc, argv);
  else if (strcmp(argv[1], "kill") == 0)//kill
   kill_(argc, argv);
  else if (strcmp(argv[1], "mv") == 0)//mv
   mv(argc, argv);
  else if (strcmp(argv[1], "tar") == 0)//tar
   tar(argc, argv);
  else //wrong command
   printf("wrong command\n");
 }
 else//부모 프로세스 일때
 {
  wait(NULL);//자식 종료 기다림
  return 0;
 }

 return 0;
}

