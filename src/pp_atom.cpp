

//#include <semaphore.h>
#include <thread>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <condition_variable>
#include <mutex>

#define DATA_SZ 5
#define NUM_IT 5

std::atomic<bool> atomic_do_swap;


std::vector<int> data_1;
std::vector<int> data_2;

void receive_data() {
    for (int i = 0; i < data_1.size(); i++) {
//        sleep(1);
        data_1[i] = i;
        printf("Generating New data in data_1[%d]...\n",data_1[i]);
    }
    
    printf("wating for processor to swap data....\n");
    atomic_do_swap.load(std::memory_order_acquire);
    data_1.swap(data_2);
    atomic_do_swap .store(false,std::memory_order_release);

}

void send_data() {
    
    printf("wating for generator for data....\n");
    for (int i = 0; i < data_2.size(); i++) {
//        sleep(1);
        std::cout <<"the data is: " << data_2[i] << std::endl;
    }


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
    
    atomic_do_swap.store(true, std::memory_order_release);


    std::thread generator(data_generator);
    std::thread processor(data_processor);

    generator.join();
    processor.join();
    
    return 0;
}