#include <unistd.h>
#include <stdio.h>
#include <string.h>

extern char** environ;

int main(){

   char **ep = environ;
   for(; *ep!=NULL; ep++){
        printf("%s\n", *ep);
   } 

   return 0;
}