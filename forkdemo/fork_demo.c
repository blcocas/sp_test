#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
void fork_example(){

  int a = 1;
  int b = 0;
  int on_off = 0;

  printf("input : ");
  scanf("%d",&a);
  while(1){
    int pid;
    if(a == 1){
      if(!on_off) pid = fork();
      on_off = 1;
     // printf("1 이입력되었습니다. 그리고 fork되었습니다. b is (%d)\n",b);
      b++;
     // sleep(2);
    }

    if(pid == 0) {
        printf("자식프로세스 입니다. while문 탈출!\n");
	printf("pid :  %d\n",getpid());
        break; //자식프로세스
    }
    else if(a == 0){
      //int ppid = getppid();
      //strcat("kill ",ppid);
      //system("kill    //자식프로세스 종료
      printf("자식프로세스를 종료합니다.");
    }
  }
}



void main(){
  fork_example();
  int a = 0;
  int ppid = getppid();
  while(1){
   scanf("%d",&a);
   if(a == 1) 
  	 strcat("kill ", ppid);
   printf("자식프로세스 작동중... a is %d\n",a);
   // a++;
  }
}
