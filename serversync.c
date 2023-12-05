#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "serversync.h"



void send_sinc(int ok_id, int req_id, messaggio * msg)
{

    int ret;

    /* TBD: Inviare il messaggio REQUEST TO SEND */
    pid_t pid = getpid();
    
    messaggio_RTS msg_req;
    msg_req.type = pid;

    messaggio_OTS msg_ok;

    printf("[%d] Client: invio request-to-send, type=%ld\n", getpid(), msg_req.type);

    ret = msgsnd(req_id, &msg_req, sizeof(messaggio_RTS) - sizeof(long), 0);

    if (ret < 0)
    {
        perror("Errore msgsnd (request-to-send)");
        exit(1);
    }


    /* TBD: Ricevere il messaggio OK TO SEND */

    printf("[%d] Client: in attesa di ok-to-send...\n", getpid());

    ret = msgrcv(ok_id, &msg_ok, sizeof(messaggio_OTS) - sizeof(long), getpid(), 0);

    if (ret < 0)
    {
        perror("Errore msgrcv (ok-to-send)");
        exit(1);
    }

    printf("[%d] Client: ricevuto ok-to-send... type=%ld, id_coda=%d\n", getpid(), msg_ok.type, msg_ok.id_coda);




    /* TBD: Inviare il messaggio al server */

    long type = msg_ok.type;
    int id_coda = msg_ok.id_coda;

    printf("[%d] Client: invio messaggio, coda=%d, type=%ld, valore=%d\n", getpid(), id_coda, msg->type, msg->val);

    ret = msgsnd(id_coda, msg, sizeof(messaggio) - sizeof(long), 0);

    if (ret < 0)
    {
        perror("Errore msgsnd (coda messaggi)");
        exit(1);
    }

}

void receive_sinc(int msg_id, int ok_id, int req_id, messaggio * msg)
{

    int ret;


    /* TBD: Ricevere il messaggio REQUEST TO SEND */
    pid_t pid = getpid();

    messaggio_RTS msg_REQ;

    messaggio_OTS msg_OK;

    printf("[%d] Server: in attesa di request-to-send...\n", getpid());

    ret = msgrcv(req_id, &msg_REQ, sizeof(messaggio_RTS) - sizeof(long), 0, 0);


    printf("[%d] Server: ricevuto request-to-send, type=%ld\n", getpid(), msg_REQ.type);

    if (ret < 0)
    {
        perror("Errore msgrcv (request-to-send)");
        exit(1);
    }



    /* TBD: Inviare il messaggio OK TO SEND */

    msg_OK.type = msg_REQ.type;
    msg_OK.id_coda = msg_id;

    printf("[%d] Server: invio ok-to-send, type=%ld, id_coda=%d\n", getpid(), msg_OK.type, msg_OK.id_coda);

    ret = msgsnd(ok_id, &msg_OK, sizeof(messaggio_OTS) - sizeof(long), 0);

    if (ret < 0)
    {
        perror("Errore msgsnd (ok-to-send)");
        exit(1);
    }



    /* TBD: Ricevere il messaggio destinato al server, usando la struct indicata
            dal puntatore "msg" (passato dal chiamante della funzione) 
     */

    printf("[%d] Server: in attesa del messaggio...\n", getpid());

    ret = msgrcv(msg_id, msg, sizeof(messaggio) - sizeof(long), 0, 0);

    if (ret < 0)
    {
        perror("Errore msgrcv (coda messaggi)");
        exit(1);
    }

    printf("[%d] Server: ricevuto messaggio, type=%ld, valore=%d\n", getpid(), msg->type, msg->val);

}
