#include <unistd.h>
#include "Handler.h"
#include<sys/timeb.h>

using namespace std;

long long systemtime() {
    timeb t{};
    ftime(&t);
    return t.time * 1000 + t.millitm;
}


void testHandler() {
    Handler h;
    printf("send delay1 on %lld\n", systemtime());
    int w1 = h.postDelayed([] {
        printf("---------delay1 500 on %lld\n", systemtime());
    }, 500);
    printf("send delay1 ok id: %d on %lld\n", w1, systemtime());

    h.postDelayed([] {
        printf("---------delay3 0 on %lld\n", systemtime());
    }, 0);


    int w2 = h.postDelayed([] {
        printf("---------delay2 2000 on %lld\n", systemtime());
    }, 2000);
    printf("send 2 ok id:%d on %lld\n", w2, systemtime());

    std::this_thread::sleep_for(120ms);

    // h.cancelAllTask();
    sleep(1);
    h.cancelTask(w2);
}

int main() {
    testHandler();

    sleep(1);
}
