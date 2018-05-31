#include <unistd.h>
#include <stdio.h>
#include <time.h>

void fork_example(){

  int a = 1;
  int b = 0;
  printf("input : ");
  scanf("%d",&a);
  while(1){
    int pid;
    int b = 0;
    if(a == 1){
      pid = fork();
      printf("1 이입력되었습니다. 그리고 fork되었습니다. b is (%d)\n",b);
      b++;
     // sleep(2);
    }

    if(pid == 0) {
        printf("자식프로세스 입니다. while문 탈출!\n");
        break; //자식프로세스
    }
    else if(a == 0){
          //자식프로세스 종료
      printf("자식프로세스를 종료합니다.");
    }
  }
}



void main(){
  fork_example();
  int a;
  while(1){
    printf("자식프로세스 작동중...\n");
    printf("type your number:");
    scanf("%d",&a);
    printf("%d",a);
   // sleep(2);
  }
}
