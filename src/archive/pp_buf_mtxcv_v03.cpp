#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include "pp_buf.hh"
#include <mutex>
#include <unistd.h>
#include <condition_variable>

#define NUM_IT 10

std::mutex g0_mutex, g1_mutex;
std::condition_variable g0_cv, g1_cv;
bool gb0_ready = false;
bool gb1_ready = false;

void rx(int16_t data[], size_t sz, buf &buffer ) {
    std::vector<int16_t> v(data, data + sz);
    buffer.set_sz(sz);
    buffer.write_bf(v);

}


void tx(buf &buffer) {
    buffer.read_bf();
}

int16_t *gen_data(size_t sz) {
    int16_t * ptr = new int16_t[sz];
    for (int i = 0; i < sz; i++) {
        ptr[i] = (int16_t)i;
    }

    return ptr;
}

// * Will generate data and swap buffers if they are free
// * This version always get the both buffers full or empty.
// * It seems that does not let thread 2 to read in between.
// * Also if-else does not look good. 
// * Maybe a while loop to ping pong buffers.
// * This approach can read buffer 1 consecutive
// * Check which data needs to read first = oldest one.
// * does not read and write in parallel. which is pointless
void data_reader(std::vector<buf> &buffers, size_t buff_sz) {
    std::cout << "Starting reader thread...." << std::endl;
    int16_t *data = gen_data(buff_sz);
    
    for (int i = 0; i < NUM_IT; i++) {
        printf("loop in reader it: %d\n",i);
        std::unique_lock<std::mutex> ul0(g0_mutex);
        if (buffers[0].empty) { 
            rx(data, buff_sz, buffers[0]);
            gb0_ready = true;
            ul0.unlock();
            g0_cv.notify_one();
            ul0.lock();
            g_cv.wait(ul0,[](){return gb0_ready == false; });
        } 

        std::unique_lock<std::mutex> ul1(g1_mutex);
        if (buffers[1].empty) {
            rx(data, buff_sz, buffers[1]);
            gb1_ready = true;
            ul1.unlock();
            g1_cv.notify_one();
            ul1.lock();
            g_cv.wait(ul0,[](){return gb0_ready == false; });
        } 
        if (!buffers[0].empty && !buffers[1].empty)
            printf("both buffers are full, wait until any is empty...\n");
        //
    }
    //std::lock_guard<std::mutex> lock(theMutex);
    
    delete []data;
}

void data_display(std::vector<buf> &buffers, size_t buff_sz) {
    std::cout << "Starting display thread...." << std::endl;
    //
    for (int i = 0; i < NUM_IT; i++) {
        printf("loop in display it: %d\n",i);
        //std::cout << "from display buffers empty: " << i <<" "<<buffers[0].empty   << std::endl;
        std::unique_lock<std::mutex> ul0(g0_mutex);
        g0_cv.wait(ul0,[](){return gb0_ready; });
        tx(buffers[0]);
        gb0_ready = false;
        ul0.unlock();
        g0_cv.notify_one();

        std::unique_lock<std::mutex> ul1(g1_mutex);
        g1_cv.wait(ul1,[](){return gb1_ready; });
        tx(buffers[1]);
        gb1_ready = false;
        ul1.unlock();
        g1_cv.notify_one();
  
    }
}


int main () {

    size_t buf_sz = 5;
    int num_buffers = 2;
    std::vector<buf> buffers(num_buffers);
    
  
    

    std::thread display(&data_display, std::ref(buffers), buf_sz);
    std::thread processing(&data_reader, std::ref(buffers),buf_sz);

    processing.join();
    display.join();

    printf("goodbye\n");


    
    return 0;
}

