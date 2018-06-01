#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
void fork_example(){

  int signal;
  int on_off = 0;
  int pid = 0;
  int get_pid = 0;
  while(1){
    printf("singal??? : ");
    scanf("%d",&signal);

    if(signal == 1){
      switch(on_off){
        case 0 :
          pid = fork();
          //get_pid = getpid();
          on_off = 1;
          break;

        case 1 :
          printf("child(%d) is exited\n",pid);
          char str1[20] = "kill -9 ";
          char str2[10];
          sprintf(str2,"%d",pid);
          strcat(str1,str2);
          printf("%s",str1);
          system(str1);
          //system("ps -ef | grep defunct | awk '{print $3}' | xargs kill -9");
          on_off = 0;
          break;
      }
    }

    if(pid == 0){//child
      printf("child(%d), break!\n",getpid());
      break;
    }
  }
}




void main(){
  fork_example();
  int a =0;
  while(1){
    a++;
  }
}
