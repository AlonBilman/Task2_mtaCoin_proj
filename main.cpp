
#include <iostream>
#include "server.h"
#include "global.h"
#include <pthread.h>
#include "miner.h"

#define NUMBERS_OF_MINERS 4

bool flag=false;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void* server_thread_start(void* arg)
{
    Server* server= static_cast<Server*>(arg);
    server->start();
    return nullptr;
}

void* miner_thread_start(void* arg)
{
    Miner* miner= static_cast<Miner*>(arg);
    miner->start_mining();
    return nullptr;
}

int main(int argc,char* argv[])
{
    pthread_t server_thread, real_miner[4]; // the fake miner will send wrong answers sometimes.
    pthread_mutex_init(&mutex,nullptr);
    pthread_cond_init(&cond,nullptr);

    Server* server = new Server(20) ; //1 server
    Miner* miners[NUMBERS_OF_MINERS]; //5 miners -> the 5th is the fake_miner.

    for(int i=0;i<NUMBERS_OF_MINERS-1;++i)
        miners[i]= new Miner(i);

    pthread_create(&server_thread,nullptr,&server_thread_start,server);

    for(int i=0;i<NUMBERS_OF_MINERS - 1;++i)
        pthread_create(&real_miner[i],nullptr,&miner_thread_start,miners[i]);


    for(int i=0;i<NUMBERS_OF_MINERS-1;++i)
        pthread_join(real_miner[i],nullptr);


    //free/delete all the threads

    return 0;


}
