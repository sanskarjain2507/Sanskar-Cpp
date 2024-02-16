//  Task ventilator
//  Binds PUSH socket to tcp://localhost:5557
//  Sends batch of tasks to workers via that socket

#include "zhelpers.h"

int main (void) 
{
    void *context = zmq_ctx_new ();

    //  Socket to send messages on
    void *sender = zmq_socket (context, ZMQ_PUSH);
    zmq_connect (sender, "tcp://localhost:5557");
    // zmq_bind (sender, "tcp://*:5557");

    //  Socket to send start of batch message on
    void *sink = zmq_socket (context, ZMQ_PUSH);
   int res= zmq_connect (sink, "tcp://localhost:5558");
    if (res == 0) {
    // Connection successful
    printf("Connected successfully\n");
} else {
    // Connection failed
    perror("zmq_connect");
}
    printf ("Press Enter when the workers are ready: ");
    getchar ();
    printf ("Sending tasks to workers...\n");

    //  The first message is "0" and signals start of batch
    
    printf ("Total expected cost: %d msec\n",s_send (sink, "0"));
    //  Initialize random number generator
    srandom ((unsigned) time (NULL));

    //  Send 100 tasks
    int task_nbr;
    int total_msec = 0;     //  Total expected cost in msecs
    for (task_nbr = 0; task_nbr < 100; task_nbr++) {
        int workload;
        //  Random workload from 1 to 100msecs
        workload = randof (100) + 1;
        total_msec += workload;
        char string [10];
        sprintf (string, "%d", workload);
        s_send (sender, string);
         printf ("sent %d \n", task_nbr);
    }
    printf ("Total expected cost: %d msec\n", total_msec);

    zmq_close (sink);
    printf ("Total expected cost: %d msec\n", total_msec);
    zmq_close (sender);
    printf ("Total expected cost: %d msec\n", total_msec);
    zmq_ctx_destroy (context);
    printf ("Total expected cost: %d msec\n", total_msec);
    return 0;
}