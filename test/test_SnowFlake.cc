#include "../SnowFlake.h"
#include <pthread.h>

void *test(void *arg) {
    SnowFlake* sf;
    sf = (SnowFlake*)arg;
    for (int i = 0; i < (1<<12); i++) {
        LLOG(INFO) << sf->nextId();
    }
    return 0;
}

int main() {
    LLOG_FILE(); 
    SnowFlake sf(2, 3);
    pthread_t tid;
    pthread_create(&tid, NULL, test, &sf);
    for (int i = 0; i < (1<<12); i++) {
        LLOG(INFO) << sf.nextId();
    }
    pthread_join(tid, NULL);
    return 0;
}