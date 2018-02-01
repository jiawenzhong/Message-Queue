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
#include "header.h"
using namespace std;

int main() {

	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype = 2; // required
		char greeting[50]; // mesg content
		int number; // mesg content
		int sender = 251; // sender's name
		int receiver = 0;
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	srand(time(NULL));//seed of the random number generator
	int done = 1;//set to 1 means receiver is not done

	//set done msg
	strcpy(msg.greeting,"Done");
	msg.mtype = 2;
	msg.number = 0000;
	get_info(qid, (struct msgbuf *)&msg, size, msg.mtype);

	strcpy(msg.greeting, "Sending");

	while(done!= 0){//continue receiver is not done
		msg.number = rand();//get a random number
		//cout << "Outside Random: " << msg.number << endl;

		msg.mtype = 2;

		//check if the random number is a multiple of sender's mtype
		int check = msg.number % msg.sender;

		//cout << "outside number" << msg.number << endl;
		//check if the number is a multiple of mtype and if the greeting = "done"
		if (check == 0){
			cout << getpid() << ": sends greeting " << msg.number << endl;
			msg.sender = 251; 	// set message type mtype = 257

			cout<<//check for error
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending
			msgrcv(qid, (struct msgbuf *)&msg, size, 4997, 0); // reading
			//cout << getpid() << ": gets reply" << endl;
			//cout << "Received Reply from: " << msg.receiver << endl;
			cout << endl;
		}

	} 


	cout << "Sender 257 - " << getpid() << ": now exits" << endl;

	exit(0);
}


