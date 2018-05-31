#include <unistd.h>
#include <stdio.h>


void fork_example(){

  int a = 0;

  while(1){

    scanf("%d",&a);
    int pid;

    if(a == 1){
      pid = fork();
      printf("1이입력되었습니다. 그리고 fork되었습니다.");
    }

    if(pid == 0) {
        printf("자식프로세스 입니다. while문 탈출!");
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
  while(1){
    printf("자식프로세스 작동중...");
    sleep(100);
  }

}
