
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

int main() {
	//for parent to child
	int parentToChild[2];
	//for child to parent
	int childToParent[2];

	//string data
	char message1[] = "Message1 C->P\n";
	char message2[] = "Message2 P->C\n";
	char message3[] = "Message3 C->P\n";
	char message4[] = "Message4 P->C\n";
	char readBuffer[120];
	pipe(parentToChild);
	pipe(childToParent);
	int pid;

	pid = fork();
	if(pid == -1){
		printf("Error Fork");
		perror ("fork( )");
		exit (-1);
	}

	if(pid == 0){
		//child process here
		//send message from child to parent
		close(childToParent[0]);
		write(childToParent[1] , message1 , strlen(message1)+1);
		

		//recive message from parent
		close(parentToChild[1]);
		read(parentToChild[0] , readBuffer , sizeof(readBuffer));
		printf("%s",readBuffer);


		write(childToParent[1] , message3 , strlen(message3)+1);


		read(parentToChild[0] , readBuffer , sizeof(readBuffer));
		printf("%s",readBuffer);


		// exit(1);
	}

	if(pid !=0 ){
		//parent process here
		//send message from parent to child
		close(parentToChild[0]);
		write(parentToChild[1] , message2 , strlen(message2)+1);


		//recieve message from child
		close(childToParent[1]);
		read(childToParent[0] , readBuffer , sizeof(readBuffer));
		printf("%s", readBuffer);

		// exit(1);

		write(parentToChild[1] , message4 , strlen(message4)+1);

		read(childToParent[0] , readBuffer , sizeof(readBuffer));
		printf("%s", readBuffer);
	}
	return 0;
}
