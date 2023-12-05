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
        perror("Errore creazione coda messaggi");
        exit(1);
    }

    /* Coda "REQUEST TO SEND" */
    /* TBD: Definire una chiave per "REQUEST TO SEND" */
    key_t req_key = ftok(".", 'b'); 
    /* TBD: Ottenere la coda per i messaggi "REQUEST TO SEND" */
    int req_id = msgget(req_key, 0644);

    if (req_id < 0)
    {
        perror("Errore creazione coda messaggi");
        exit(1);
    }

    /* Creazione di una coda messaggi privata del processo server */
    /* TBD: Definire una chiave per la coda */
    key_t msg_key = IPC_PRIVATE;
    /* TBD: Ottenere la coda privata del server */
    int msg_id = msgget(msg_key, IPC_CREAT | 0644);

    if (msg_id < 0)
    {
        perror("Errore creazione coda messaggi");
        exit(1);
    }

    for (int i = 0; i < 4; i++)
    {
        messaggio msg;

        receive_sinc(msg_id, ok_id, req_id, &msg);

        printf("[%d] Server: ricevuto val=%d\n", getpid(), msg.val);
    }

    /* Dà ai client alcuni secondi di tempo per ricevere le risposte,
     * prima di deallocare la coda privata */
    sleep(3);
    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
}