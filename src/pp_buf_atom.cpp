#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include "pp_buf.hh"
#include <mutex>
#include <unistd.h>

std::mutex theMutex;

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

// Will generate data and swap buffers if they are free
void data_reader(std::vector<buf> &buffers, size_t buff_sz) {
    std::cout << "Starting reader thread...." << std::endl;
    int16_t *data = gen_data(buff_sz);

    if (buffers[0].empty)
        rx(data, buff_sz, buffers[0]);
    // else if (buffers[1].empty)
    //     rx(data, buff_sz, buffers[1]);
    else
        printf("both buffers are full, wait until any is empty...\n");
    
    //std::lock_guard<std::mutex> lock(theMutex);
    
    delete []data;
}

void data_display(std::vector<buf> &buffers, size_t buff_sz) {
    std::cout << "Starting display thread...." << std::endl;
    //
    for (int i = 0; i < 10; i++) {
    std::cout << "from display buffers empty: " << i <<" "<<buffers[0].empty   << std::endl;
        if (!buffers[0].empty) {
            //std::lock_guard<std::mutex> lock(theMutex);
            tx(buffers[0]);
        }

        // if (!buffers[1].empty) {
        //     std::lock_guard<std::mutex> lock(theMutex);
        //     tx(buffers[1]);
        // }
    }
}


int main () {

    size_t buf_sz = 10;
    int num_buffers = 2;
    std::vector<buf> buffers(num_buffers);
    
  
    

    //std::thread display(&data_display, std::ref(buffers), buf_sz);
    std::thread processing(&data_reader, std::ref(buffers),buf_sz);
std::thread display(&data_display, std::ref(buffers), buf_sz);
    processing.join();
    display.join();




    
    return 0;
}

