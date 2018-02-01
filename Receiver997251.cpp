/* 

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program B creates a message queue to be shared with Program A.
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by Program B.

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

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype = 1997; // required
		char greeting[50]; // mesg content
		int number; // mesg content
		int sender;
		int receiver;
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	//int done = 1;
	int done997 = 1;
	int done251 = 1;
	int receiveMsg;
	srand(time(NULL));


	//msgrcv(qid, (struct msgbuf *)&msg, size, 257, 0); // read mesg
	while(done997 + done251 != 0){
	
		//receive msg from sender 997
		msgrcv(qid, (struct msgbuf *)&msg, size, 2, 0); // read mesg
		//if msg.sender == 0, it is the msg send by the interrupt from kill command
		if (msg.sender == 0) msg.sender = 251;
		//cout << "sender: " << msg.sender << ": " << msg.greeting<< endl;
		if(msg.sender == 997 | msg.sender == 251){
			//print out sender's information and the message
			cout << getpid() << ": gets message from " << msg.sender << endl;
			receiveMsg = msg.number;
			cout << "message: " << receiveMsg << endl;

			msg.receiver = 997251;//set receiver name

			//set mtype and print out information according to the sender 
			if (msg.sender == 997){	
				msg.mtype = 3997; //set receiver mtype
				//check done
				done997 = strcmp(msg.greeting, "Done");
				cout << "done997: " << done997 << endl;
				//send msg back to 997
				cout << getpid() << ": sends reply to 997" << endl;
				cout << "send msg: " << receiveMsg << endl;
				msg.number = receiveMsg;
			} else if (msg.sender == 251){
				msg.mtype = 4997;
				cout << "reply from 251: " << msg.greeting << endl;
				done251 = strcmp(msg.greeting, "Done");
				cout << "done251: " << done251 << endl;
			}


			cout<<
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		
			cout << endl;
			//cout << "done check: " << done997 + done251 << endl;
		}

		
	}

	cout << getpid() << ": now exits" << endl;
	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}


