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
	int qid = msgget(ftok(".",'u'), 0);

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
	int receiveCount = 0;
	int maxReceive = 50000;
	int randval;
	int receiveMsg;
	srand(time(NULL));

	//msgrcv(qid, (struct msgbuf *)&msg, size, 257, 0); // read mesg
	while(receiveCount < maxReceive){
	
		//receive msg from sender 997
		msgrcv(qid, (struct msgbuf *)&msg, size, 1, 0); // read mesg
		//cout << "sender: " << msg.sender<< endl;
		if(msg.sender == 997 | msg.sender == 257){
			cout << getpid() << ": gets message from " << msg.sender << endl;
			receiveMsg = msg.number;
			cout << "message: " << receiveMsg << endl;

			//increase msg count
			receiveCount += 1;

			//check before sending
			if((receiveCount) >= maxReceive){
				strcpy(msg.greeting, "Done");
				cout << "set done msg " << receiveCount << endl;
			}

			msg.receiver = 997257;
			//cout << endl;	
			if (msg.sender == 997){	
				//send msg back to 997
				msg.mtype = 1997; //set receiver mtype
				cout << getpid() << ": sends reply to 997" << endl;
				cout << "send msg: " << receiveMsg << endl;
				msg.number = receiveMsg;
			}else if (msg.sender == 257){
				msg.mtype = 2997; //set receiver mtype
				cout <<"send flag " << msg.greeting << endl; 	
			}


			//send message back to sender
			cout<<
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			
			//send ending message to the other sender
			if(strcmp(msg.greeting, "Done") == 0){
				cout << endl;
				cout << "last message from: " << msg.sender << endl;
				if(msg.mtype == 2997){
					msg.mtype = 1997;
					msgsnd(qid, (struct msgbuf *)&msg, size, 0);
					cout << "send done msg to " << msg.mtype << endl;
				} else {
					msg.mtype = 2997;
					msgsnd(qid, (struct msgbuf *)&msg, size, 0);
					cout << "send done msg to " << msg.mtype << endl;
				}
			}

		
			cout << endl;
		}
	
	}
	
	cout << getpid() << ": now exits" << endl;

	exit(0);
}


