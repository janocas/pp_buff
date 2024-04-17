#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include "pp_buf.hh"
#include <mutex>
#include <unistd.h>
#include <condition_variable>

#define NUM_IT 10

std::mutex g_mutex;
std::condition_variable g_cv;
bool g_ready = false;

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
// * Multithread that acts like on thread. Silly test.
void data_reader(std::vector<buf> &buffers, size_t buff_sz) {
    std::cout << "Starting reader thread...." << std::endl;
    int16_t *data = gen_data(buff_sz);
    
    for (int i = 0; i < NUM_IT; i++) {
        std::unique_lock<std::mutex> ul(g_mutex);
        rx(data, buff_sz, buffers[0]);
        g_ready = true;
        ul.unlock();
        g_cv.notify_one();
        ul.lock();
        g_cv.wait(ul,[](){return g_ready == false; });
 
        rx(data, buff_sz, buffers[1]);
        g_ready = true;
        ul.unlock();
        g_cv.notify_one();
        ul.lock();
        g_cv.wait(ul,[](){return g_ready == false; });
    }
    
    
    delete []data;
}

void data_display(std::vector<buf> &buffers, size_t buff_sz) {
    std::cout << "Starting display thread...." << std::endl;
    //
    for (int i = 0; i < NUM_IT; i++) {
        //std::cout << "from display buffers empty: " << i <<" "<<buffers[0].empty   << std::endl;
        std::unique_lock<std::mutex> ul(g_mutex);
        g_cv.wait(ul,[](){return g_ready; });
        tx(buffers[0]);
        g_ready = false;
        ul.unlock();
        g_cv.notify_one();
        ul.lock();

        g_cv.wait(ul,[](){return g_ready; });
        tx(buffers[1]);
        g_ready = false;
        ul.unlock();
        g_cv.notify_one();
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

