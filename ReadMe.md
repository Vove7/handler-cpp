# Handler for C++

```c++
Handler h;
h.postDelayed([]{
    printf("---------delay1 500 on %lld\n", systemtime());
}, 500);
h.post([]{
    printf("---------post2 0 on %lld\n", systemtime());
});
int task3 = h.postDelayed([] {
    printf("---------delay3 2000 on %lld\n", systemtime());
}, 2000);
sleep(1);
//cancel task3
h.cancelTask(task3);

```

### Build

```bash
git clone https://github.com/Vove7/handler-cpp
cd handler-cpp
mkdir build && cd build
cmake .. && make
# run test
./handler-cpp
```
