#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "serversync.h"

int main()
{

    /* Coda "OK TO SEND" */
    /* TBD: Definire una chiave per "OK TO SEND" */
    key_t ok_key = ftok(".", 'a');
    /* TBD: Ottenere la coda per i messaggi "OK TO SEND" */
    int ok_id = msgget(ok_key, 0644);

    if (ok_id < 0)
    {
        perror("Errore creazione coda ok-to-send");
        exit(1);
    }

    /* Coda "REQUEST TO SEND" */
    /* TBD: Definire una chiave per "REQUEST TO SEND" */
    key_t req_key = ftok(".", 'b');
    /* TBD: Ottenere la coda per i messaggi "REQUEST TO SEND" */
    int req_id = msgget(req_key, 0644);

    if (req_id < 0)
    {
        perror("Errore creazione coda request-to-send");
        exit(1);
    }




    srand(getpid());

    for (int i = 0; i < 2; i++)
    {
        int val = rand() % 100;

        printf("[%d] Client: invio val=%d\n", getpid(), val);


        /* TBD: Inviare un messaggio al server, usando la struct "messaggio"
                definita in "serversync.h" 
         */
        messaggio msg;
        msg.type = getpid();
        msg.val = val;

        /* NOTA: sarà la funzione send_sinc() a scambiare i messaggi 
                 "REQUEST TO SEND" e "OK TO SEND" con uno dei server.
        */

        send_sinc(ok_id, req_id, &msg);

        sleep(2);
    }

    return 0;
}