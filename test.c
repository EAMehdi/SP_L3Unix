#include<iostream>

#include<sys/types.h>

#include<unistd.h>

#include<sys/wait.h>

using namespace std;



//control the number of travel agents - easy to change for test runs

#define NUMBER_OF_CHILDREN = 6;



//the program uses seats_left to know when the flight is totally

//booked and time to shut down.  The algorithm for this requires

//seats_left = ( 2 - number of children) to know it has completed

#define PARENT_CONTROL = (2 - NUMBER_OF_CHILDREN);



//required as a paramater for sending ints down a pipe
#define SIZE_OF_INT = sizeof(int);

//easy to change number of seats for different test runs

const int INITIAL_NUMBER_OF_SEATS = 100;



//travel agent function

void travel_agent (int* child_to_parent, int* parent_to_child, int id);



int main(){



	//create 2 pipes to talk from children to parent and parent to children

	//create an int array of size 2, then declared a pipe with the pipe() function

	//using the array as a parameter as so:

	int child_to_parent[2];

	int parent_to_child[2];

	pipe(child_to_parent);

	pipe(parent_to_child);



	//array of 6 pidIDs.  used for joining threads at end of program

	//pid_t is the variable type needed

	pid_t shut_down[NUMBER_OF_CHILDREN];



	//number of seats left

	int seats_left = INITIAL_NUMBER_OF_SEATS;



	//for keeping track if we are in the parent of child for each fork

	int pid;



	//create the travel agents

	for (int i = 0; i < NUMBER_OF_CHILDREN; i++){



		//the fork

		pid = fork();



		//error catching.  a pid < 0 is returned when there is a failure

		if (pid < 0){

			cout << "OH SNAP! Child " << i <<  " failed";

			return -1;

		}



		//if the pid is 0, we are in the child process.  output some useful

		//information, and then start running the travel agent function.

		//It is important to break the children from the for loop.  Otherwise they

		//will continue the for loop from where they were created and thus create

		//more children themselves!

		if (pid == 0 ) {



			//output to show when children are created

			cout << "CHILD: " << i << " CREATED " <<  getpid() << endl;



			//start a travel agent function

			travel_agent(child_to_parent, parent_to_child, i);

			break;

		}



		//if the pid is greater than 0, the parent is running.  we want to build

		//an array of each pid, as this information will be useful later when we

		//are ending the program.

		else{

			//build array of pids to wait for them to die

			shut_down[i] = pid;

		}

	} //end child creation for loop





	if (pid>0){ //parent and control function

		bool loop = true;



		//in a loop

		while (loop){



			//this is the IPC part of the main function.  we want to make sure that only one travel

			//agent can read the number of seats left at a time.  We also need to make sure that only

			//one agent can book a seat at a time.  If we don't protect the seats_left variable, we will

			//be able to over book the flight, which we want to avoid.



			//we accomplish the protection of the critical section with pipes.  when a process

			//reads from a pipe, if there is nothing in it to read, it will wait for something

			//to be written into it before continuing.  When something is written into a pipe, that

			//data can only be read once and only by one process.  If there are 5 processes all waiting

			//to read the same pipe and then there is one write to the pipe, only ONE of the 5

			//waiting processes will get to read what was written and continue, while the other 4 will

			//keep waiting.



			//we will start by writing the current seats_left variable into the parent_to_child pipe.

			//and in the travel agent function, they will start by reading from the parent_to_child pipe.



			//the travel agent will, after reading from the child_to_parent pipe, book a seat, and then

			//write the value of the remaining seats back to the parent by the child_to_parent pipe.



			//this is basically forcing all of the travel agents to read the seats_left variable one at a time

			//and doesn't allow the the main function to write the seats_left while a travel agent is booking

			//a seat.



			//write to the children seats_left and then wait for a response from the children

			write(parent_to_child[1], &seats_left, SIZE_OF_INT);

			read(child_to_parent[0], &seats_left, SIZE_OF_INT);



			//if there are seats left, output how many are left

			if (seats_left > 0)

				cout << "the main just read seats_left: " << seats_left << endl;



			//when seats_left < PARENT_CONTROL, all children have finished their loops

			if (seats_left < PARENT_CONTROL){

				loop = false;

				cout << "Parent exit loop. PID: " << getpid() << endl;

			}

		}



		//here is a solution to an interesting problem, known as Zombies.  A zombie is a child process that

		//continues running after the parent has terminated.  Without this next for loop, it is possible to get

		//zombies.  This is what waitpid is for.  waitpid will force the current process to wait for another process

		//to terminate before continuing.  in the for loop, we force main to wait for each of its children to terminate

		//before continuing.

		for (int i = 0; i < NUMBER_OF_CHILDREN; i++){

			cout << "Waiting for PID: " << shut_down[i] << " to finish" << endl;

			waitpid(shut_down[i], NULL, 0);

			cout << "PID: " << shut_down[i] << " has shut down" << endl;

		}

	}



	//this is the joined section of the program.  All processes, parent and children, will run this section

	//with the waitpid code, the last line shown will always be "There are no ZOMBIES".  If we remove the waitpid

	//section it will be possible to see output ater "There are no ZOMBIES", and it will even be possible to see

	//output after a new prompt line.



	//check to see all children closed

	cout << "Did we all Join?  There will be 7 of us if we did.  PID: "  << getpid() << endl;

	if (pid>0)

		cout << "There are no ZOMBIES!" << endl;



	return 0;

}



void travel_agent (int* child_to_parent, int* parent_to_child, int id){

	bool loop = true;



	//do a loop

	while (loop){



		//declare seats_left as a local variable

		int seats_left = 0;

		//wait for parent to write, then process seats left

		read(parent_to_child[0], &seats_left, SIZE_OF_INT);



		//if there are seats left, write out how many and then 'book' a seat

		//declare thread number, and pid

		if (seats_left > 0){

			cout << "I am Child: " << id << " There are: " << seats_left << " seats, booking one!";

			cout << " My PID is : " << getpid() << endl;

			seats_left--;



			//if we booked the last seat, stop looping

			if (seats_left == 0)

				loop=false;



			//tell the parent how many seats are left and get off the CPU (don't be a hog)

			write(child_to_parent[1], &seats_left, SIZE_OF_INT);

			usleep(100);

		}



		//if there are no seats left, stop looping and write back to the parent

		else{

			loop = false;

			seats_left--;

			write(child_to_parent[1], &seats_left, SIZE_OF_INT);

		}

	}

}
