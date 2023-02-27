//will hopkirk threads lab

#include <stdio.h>
#include <stdlib.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "sockettome.h"
#include <pthread.h>

/*
 * public thread holds data that all thread functions need to be able to access from any thread
 * port = port
 * messages = JRB keyed on chatrooms used to store message queue
 * userd = JRB also keyed on chatrooms but store users in that room
 * fouts = JRB keyed on users that store the file buffers for messages and server messages
 * lock = mutex
 * chatting = condition var
*/
typedef struct{

	int port;
  	JRB messages;
	JRB users;
  	JRB fouts;

	pthread_mutex_t *lock;
  	pthread_cond_t *chatting;
}public_thread;

/*
 * private thread holds individual thread data
 * sockfd = socket file descriptor
 * id = thread's name for users and rooms
 * also holds a public thread so all individual threads can access the public data
*/
typedef struct{

  int sockfd;
  char *id;

  public_thread *pub;
}private_thread;

//thread function for clients
void *client_threads(void *c){

  /*
   * VARLIST
   * priv = thread to add when a client joins
   * userlist = jrb to get dllist of users from public thread info
   * messagelist = jrb to get dllist of messages from public thread info 
   * temp = dllist iterator;
   * users =  dllist of the users in a room
   * chat = dllist of the messages in a room
   * ptr = used to get rid of newline char when using fgets
   * s = string to get from fgets
   * user = user's name
   * room = room name
   * message = a message to be stored on the chat list and then fputs out
   * joining = same as message but used for the has joined server message
   * leaving = same as joining but for the has left server message
   * roomprint = for printing the room with all the users who are in it
   * fin = for fgets
   * fout = for fputs
  */

  private_thread *priv;
  JRB userlist, messagelist;
  Dllist temp, users, chat;
  char *ptr;
  char s[500], user[500], room[500], message[5000], joining[5000], leaving[5000], roomprint[5000];

  priv = (private_thread *) c;
  FILE *fin = fdopen(priv->sockfd, "r");
  FILE *fout = fdopen(priv->sockfd, "w");

  /*
   * Print out server info
   * first print out chat rooms:
   * then go through the users JRB to print out all the rooms
   * while printing the rooms we build the string: roomname: user user user ...
   * print out all room names and any users in the rooms
   * then print out the enter name statement
   * next we get the username and sadly fgets gives us a newline character so we have to do some pointer arithmatic and use strchr to get rid of it
   * now we get the chat room the user want to enter again fgets makes my life hard
   * finally we have printed out the preliminary stuff and got the users name and the room they want to chat in
  */

  fputs("Chat Rooms:\n\n", fout);
  fflush(fout);

  jrb_traverse(userlist, priv->pub->users){
   
	sprintf(roomprint, "%s:", userlist->key.s);
    if(userlist->val.v != NULL){

      users = userlist->val.v;
      dll_traverse(temp, users){

		strcat(roomprint, " ");
		strcat(roomprint, temp->val.s);
      }
    }

	strcat(roomprint, "\n");
    fputs(roomprint, fout);
    fflush(fout);
  }

  fputs("\nEnter your chat name (no spaces):\n", fout);
  fflush(fout);

  fgets(s, 500, fin);
  sprintf(user, "%s", s);
  ptr = strchr(user, '\n');
  user[(ptr - user)] = 0;	
  
  fputs("Enter chat room:\n", fout);
  fflush(fout);

  fgets(s, 500, fin);
  sprintf(room, "%s", s);
  ptr = strchr(room, '\n');
  room[(ptr - room)] = 0;

  /*
   * now we need to handle users entering the chatroom
   * start with the mutex lock call
   * grab the user list for the room
   * if the room has users add this user to the dll
   * if there are no users in the room then call newdll and add this user to it also need to store the new dll as the val of the userlist JRB
   * then add the output buffer to the fouts JRB in public thread info
   * next grab the messagelist and give it to the chat dll
   * use sprintf to construct the user has left string
   * send cond signal that chatting has occurred and unlock mutex
  */

  pthread_mutex_lock(priv->pub->lock);

  userlist = jrb_find_str(priv->pub->users, room);

  if(userlist->val.v != NULL){

  	users = userlist->val.v;
  	dll_append(users, new_jval_s(strdup(user)));
  }else{
		  
  	users = new_dllist();
  	dll_append(users, new_jval_s(strdup(user)));
  	userlist->val = new_jval_v(users);
  }
			
  jrb_insert_str(priv->pub->fouts, user, new_jval_v(fout));
  messagelist = jrb_find_str(priv->pub->messages, room);
  chat = messagelist->val.v;
			
  sprintf(joining, "%s has joined\n", user);
  dll_append(chat, new_jval_s(strdup(joining)));

  pthread_cond_signal(priv->pub->chatting);
  pthread_mutex_unlock(priv->pub->lock);
  

	/*
	 * loop while input is still happening
	 * now we are checking for messages from users
	 * if we get one we build it with sprintf and throw it in the chat dll
	 * then we throw the cond signal that we chatted and unlock the mutex like before
	*/

  while(!feof(fin)){
	  
    if(fgets(s, 500, fin) != NULL){
    
      sprintf(message, "%s: %s", user, s);

      pthread_mutex_lock(priv->pub->lock);
	  userlist = jrb_find_str(priv->pub->messages, room);
      chat = userlist->val.v;
      dll_append(chat, new_jval_s(strdup(message)));
      pthread_cond_signal(priv->pub->chatting);
      pthread_mutex_unlock(priv->pub->lock);
    }
  }

	/*
	 * in this section we do the same thing we did for joining but now for leaving
	*/

	pthread_mutex_lock(priv->pub->lock);

	messagelist = jrb_find_str(priv->pub->messages, room);
	chat = messagelist->val.v;

	sprintf(leaving, "%s has left\n", user);
	dll_append(chat, new_jval_s(strdup(leaving)));

	pthread_cond_signal(priv->pub->chatting);
	pthread_mutex_unlock(priv->pub->lock);

	/*
	 * when a user leaves we need to take them off the dll
	 * we grab the userlist and set the temp dll to the head node of the users dll
	 * start mutex lock
	 * while the next node is not the head node check if the nodes value is the user who left and if it is delete them otherwise continue through the list
	 * unlock mutex
	*/

	userlist = jrb_find_str(priv->pub->users, room);
	if(userlist != NULL){
		
		users = userlist->val.v;
		temp = dll_nil(users);
		pthread_mutex_lock(priv->pub->lock);
		while(temp->flink != dll_nil(users)){

			if(strcmp(temp->flink->val.s, user) == 0){

				free(temp->flink->val.s);
				dll_delete_node(temp->flink);
			}
			temp = temp->flink;
		}
		pthread_mutex_unlock(priv->pub->lock);
		
	}

	//finally close the fouts buffer and filestreams and free allocated variables
	pthread_mutex_lock(priv->pub->lock);
	userlist = jrb_find_str(priv->pub->fouts, user);
	jrb_delete_node(userlist);
  	pthread_mutex_unlock(priv->pub->lock);

	fclose(fin);
	fclose(fout);
	free(priv);

  	pthread_exit(NULL);
}

//thread function for chat rooms
void *room_threads(void *c){

  /*
   * VARLIST
   * priv = thread for the room
   * find = jrb to find the correct room
   * inchat = dll to fill with message queue
   * userthreads = users in the chatroom
   * fout = outputstream
  */

  private_thread *priv;
  JRB find;
  Dllist inchat, userthreads, temp, temp2;
  FILE *fout;

  priv = (private_thread *) c;
  inchat = new_dllist();

  /*
   * get the chatroom
   * start mutex lock
   * wait for chatting condition
   * get the users dll from the JRB
   * traverse the message queue
   * traverse the users and if the user has a fouts buffer print it out
  */

  find = jrb_find_str(priv->pub->messages, priv->id);
  find->val = new_jval_v(inchat);

  while(1){

	pthread_mutex_lock(priv->pub->lock);

    while(dll_empty(inchat)){

      pthread_cond_wait(priv->pub->chatting, priv->pub->lock);
      pthread_cond_signal(priv->pub->chatting);
    }

    find = jrb_find_str(priv->pub->users, priv->id);
    userthreads = find->val.v;

    dll_traverse(temp, inchat){

      dll_traverse(temp2, userthreads){

        find = jrb_find_str(priv->pub->fouts, temp2->val.s);
			
		if(find != NULL){

	        fout = find->val.v;
	        fputs(temp->val.s, fout);
	        fflush(fout);
      	}
	  }
    }

	/*
	 * clean up by clearing the message queue and unlock the mutex
	*/

    while(!dll_empty(inchat)){

		free(inchat->flink->val.s);
      	dll_delete_node(inchat->flink);
	}
		pthread_mutex_unlock(priv->pub->lock);
  }
}

//thread function for the server
void *server_thread(void *c){

  /*
   * VARLIST
   * cl = client thread
   * tmp = because otherwise it segfaulted
   * port = port
   * sock = socket
   * sockfd = socket file descriptor
  */

  private_thread *cl;
  private_thread *tmp;
  int port, sock, sockfd;
  pthread_t client;

  tmp = (private_thread *) c;
  port = tmp->pub->port;
  sock = serve_socket(port);

  /*
   * indefinite while
   * wait for a connection and create a client thread
   * detach
  */

  while(1){

    sockfd = accept_connection(sock);
	cl = malloc(sizeof(private_thread));
	cl->sockfd = sockfd;
	cl->pub = tmp->pub;
    client = (pthread_t) malloc(sizeof(pthread_t));

    if(pthread_create(&client, NULL, client_threads, (void*) cl) != 0)
    {
      perror("error");
      exit(1);
    }
    pthread_detach(client);
  }
}

int main(int argc, char** argv){

	/*
	 * VARLIST
	 * pub = public thread info all threads can access
	 * priv = individual threads
	 * server = server thread for create
	 * roomthreads = room threads for create
	 * lock = mutex
	 * chatting = chatting condition var
	*/
	public_thread pub;
	private_thread *priv;
	pthread_t server;
	pthread_t *roomthreads;
	pthread_mutex_t lock;
  	pthread_cond_t chatting;
	JRB roomnames;
	int rooms = (argc - 2);

	//just checking
	if(argc < 3){

		perror("nah");
		return -1;
	}

	//get the port and init the public thread jrbs
	pub.port = atoi(argv[1]);
	pub.users = make_jrb();
	pub.messages = make_jrb();
	pub.fouts = make_jrb();

	/*
	 * build the JRBs that are keyed on the chatrooms
	 * init the mutex and cond var
	 * malloc the threads and the roomthreads and server thread
	 * give all the rooms a reference of the public info for threads
	 * malloc + 1 room for the main server
	*/

	for(int i = 2; i < argc; i++){

		jrb_insert_str(pub.users, strdup(argv[i]), new_jval_v(NULL));
		jrb_insert_str(pub.messages, strdup(argv[i]), new_jval_v(NULL));
	}

  	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&chatting, NULL);
	pub.lock = &lock;
	pub.chatting = &chatting;

	priv = malloc(sizeof(private_thread) * (rooms + 1));
	roomthreads = (pthread_t *)malloc(sizeof(pthread_t) * (rooms + 1));
	server = (pthread_t)malloc(sizeof(pthread_t));

	for(int i = 0; i < rooms + 1; i++){
		
		priv[i].pub = &pub;
	}

	/*
	 * start mutex lock
	 * make the first thread the main server thread
	 * create the server thread to start accepting clients
	 * detach
	*/

	pthread_mutex_lock(pub.lock);
	priv[0].id = strdup("Main_Server");

	if(pthread_create(&server, NULL, server_thread, (void*) &priv[0]) != 0){

		perror("nuh uh");
	 	exit(1);
	}
	pthread_detach(server);

	/* 
	 * set up jrb of chat rooms
	 * set roomnames to the public thread info struct
	 * for each room copy the name to the private thread
	 * call create
	 * detach
	 * move to the next chatroom
	 * finally done unlock mutex and exit
	*/

	roomnames = jrb_first(pub.messages);

	for(int i = 1; i < rooms + 1; i++){

		priv[i].id = strdup(roomnames->key.s);

		if(pthread_create(&roomthreads[i - 1], NULL, room_threads, (void*) &priv[i]) != 0){

			perror("nope");
			exit(1);
		}

		roomnames = jrb_next(roomnames);
		pthread_detach(roomthreads[i - 1]);
		
	}
	pthread_mutex_unlock(pub.lock);

	pthread_exit(NULL);
	return 0;
}
