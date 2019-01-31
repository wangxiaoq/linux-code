#include <vector>
#include <string>
#include <pthread.h>
#include <iostream>
#include <unistd.h>

static std::vector<std::string> data;
static pthread_cond_t cond;
static pthread_mutex_t lock;

void *producer(void *arg)
{
    pthread_detach(pthread_self());
    std::string str;
    while (1) {
        std::cin >> str;
        pthread_mutex_lock(&lock);
        data.push_back(str);
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&cond);
    }
    return NULL;
}

void *consumer(void *arg)
{
    pthread_detach(pthread_self());
    std::string str;
    while (1) {
        pthread_mutex_lock(&lock);
        if (data.empty()) {
            pthread_cond_wait(&cond, &lock);
        }
        str = data.back();
        data.pop_back();
        pthread_mutex_unlock(&lock);
        /* handle data */
        std::cout << str << std::endl;
    }
    return NULL;
}

void init(void)
{
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t producer_tid;
    pthread_t consumer_tid;
    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_tid, NULL, consumer, NULL);
}

void cleanup(void)
{
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock);
}

int main(int argc, char *argv[])
{
    init();
    while (1) {
        sleep(1);
    }
    cleanup();
    return 0;
}
