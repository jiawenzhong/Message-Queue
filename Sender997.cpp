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
		long mtype = 1; // required
		char greeting[50]; // mesg content
		int number; // mesg content
		int sender = 997;// identify the sender
		int receiver;//name of the receiver
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	int randval;
	int done;
	int count;
	srand(time(NULL));
	

	do{
		//get random number
		randval = rand();
		//cout << "Outside Random: " << msg.number << endl;
		int numCheck = randval % msg.sender;
		//cout << "outside number" << msg.number << endl;
		//check if the number is a multiple of mtype and if the greeting = "done"
		if (numCheck == 0 && randval > 100)
		{		
			//send to receiver 997257
			if(done!= 0)
			{	
				msg.number = randval;//get a random number
				//send and receive from receiver 997257
				cout << getpid() << ": sends messeage to 997257: " << 
						msg.number << endl;
				
				//set mtype and sender
				msg.mtype = 1;
				msg.sender = 997;

				cout<<//check for error
				msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending
				msgrcv(qid, (struct msgbuf *)&msg, size, 1997, 0); // reading

				cout << getpid() << ": gets reply from: " << msg.receiver << endl;
				cout << "reply message: " << msg.number << endl;

				//check for done messege from receiver
				done = strcmp(msg.greeting, "Done");

				//check if receiver is finished
				if(done == 0)
					cout << "Receiver: " << msg.receiver << " done." << endl;
				cout << endl;
			}

			//send and receive from receiver997251(3997)
			msg.mtype = 2; 	// set message type mtype = 997
			msg.sender = 997; //reset sender after receiver
			msg.receiver = 0;//reset the receiver name after last receive
			msg.number = randval;//get a random number
			strcpy(msg.greeting, "Sending");//set greeting message

			cout << getpid() << ": sends messeage to 997251 " << msg.number << endl;
			cout<<//check for error
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending
			msgrcv(qid, (struct msgbuf *)&msg, size, 3997, 0); // reading

			//print out received message
			cout << getpid() << ": gets reply " << msg.receiver << endl;
			cout << "reply message: " << msg.number << endl;
			cout << endl;

		}
		//count the number of message sent
		count += 1;
		if(randval < 100) cout << "Exit 997: " << randval << " count: " << count << endl;

	} while(randval > 100);//continue when number is greater than 100

	//send ending message to receiver
	cout << "send ending message to receiver" << endl;
	msg.mtype = 2;
	msg.sender = 997; //reset sender after receiver
	msg.number = 0000;
	strcpy(msg.greeting, "Done");
	msgsnd (qid, (struct msgbuf *)&msg, size, 0);
	cout << "sender 997 -" << getpid() << ": now exits" << endl;

	exit(0);
}


