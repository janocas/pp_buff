

//#include <semaphore.h>
#include <thread>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <condition_variable>
#include <mutex>

#define DATA_SZ 5
#define NUM_IT 5


std::mutex g0_mutex;
//std::mutex g1_mutex;
std::condition_variable g0_cv;
//std::condition_variable g1_cv;
bool g0_ready = false;
//bool g1_ready = false;

std::vector<int> data_1;
std::vector<int> data_2;

void receive_data() {
    for (int i = 0; i < data_1.size(); i++) {
        sleep(1);
        data_1[i] = i;
        printf("Generating New data in data_1[%d]...\n",data_1[i]);
    }
    std::unique_lock<std::mutex> ul(g0_mutex);
    printf("wating for processor to swap data....\n");
    g0_cv.wait(ul,[](){ return g0_ready == false;});
    data_1.swap(data_2);
    g0_ready = true;
    ul.unlock();
    g0_cv.notify_one();
}

void send_data() {
    std::unique_lock<std::mutex> ul(g0_mutex);
    printf("wating for generator to start to process data....\n");
    g0_cv.wait(ul,[](){return g0_ready; });
    for (int i = 0; i < data_2.size(); i++) {
        //sleep(1);
        std::cout <<"the data is: " << data_2[i] << std::endl;
    }
    g0_ready = false;
    ul.unlock();
    g0_cv.notify_one();

}


void data_generator() {
    for (int i = 0; i < NUM_IT; i++ ) {
        printf("**** data generation idx: %d\n",i);
        receive_data();
    }

}

void data_processor() {
    
    for (int i = 0; i < NUM_IT; i++ ) {
        printf("**** data processing idx: %d\n",i);
        send_data();
    }

}

int main () {
    int sz = DATA_SZ;

    data_1.resize(sz);
    data_2.resize(sz);


    std::thread generator(data_generator);
    std::thread processor(data_processor);

    generator.join();
    processor.join();
    
    return 0;
}