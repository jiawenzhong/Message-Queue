/* 

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program A will use a message queue created by Program B. 
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by anyone.

Both child processes use message type mtype = 113 and 114.

*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {

	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
		int number; // mesg content
		int sender; //sender name
		int receiver; //receiver name
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	srand(time(NULL));//seed of the random number generator
	int done = 1;//set to 1 means receiver is not done
	
	// prepare my message to send
	strcpy(msg.greeting, "Sending");

	while(done!= 0){//continue receiver is not done
		msg.number = rand();//get a random number
		msg.sender = 257; 	// set message type mtype = 257

		//check if the number is a multiple of the sender's mtype
		int check = msg.number % msg.sender;

		//check if the number is a multiple of mtype and if the greeting = "done"
		if (check == 0 && done!= 0){
			cout << getpid() << ": sends greeting " << msg.number << endl;
			msg.mtype = 1;//set sender's mtype

			cout<<//check for error
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending
			msgrcv(qid, (struct msgbuf *)&msg, size, 2997, 0); // reading

			done = strcmp(msg.greeting, "Done");
			cout << "done " << msg.greeting << endl;
			cout << endl;
		}
	} 

	cout << "Sender 257 - " << getpid() << ": now exits" << endl;

	exit(0);
}


