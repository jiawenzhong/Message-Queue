#include<iostream>
#include<signal.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring> // memcpy
#include <cstdlib> // malloc
#include "header.h"
using namespace std;

int gqid;
msgbuf *gmbuf;
int gsize;
long gmtype;

void sig_handler(int signo)
{
	if (signo == SIGUSR1) {
		gmbuf->mtype = gmtype;
		msgsnd(gqid,gmbuf, gsize, 0);
		free (gmbuf);
		_exit(0);
	}
}
void get_info(int qid, struct msgbuf *mbuf, int size, long mtype) {
	signal(SIGUSR1, sig_handler);
	gqid = qid;
	gsize = size;
	gmtype = mtype;
	gmbuf = (struct msgbuf *)malloc(size);
	memcpy (gmbuf,mbuf,size);

}

