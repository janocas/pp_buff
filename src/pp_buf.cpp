#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include "pp_buf.hh"
#include <mutex>

std::mutex myMutex;

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
void data_reader(std::vector<buf> buffers, size_t buff_sz) {
    int16_t *data = gen_data(buff_sz);
    
    //std::lock_guard<std::mutex> guard(myMutex);
    if (buffers[0].ready)
        rx(data, buff_sz, buffers[0]);
    else
        rx(data, buff_sz, buffers[1]);
}

void data_display(std::vector<buf> buffers, size_t buff_sz) {
    if (buffers[1].busy && buffers[0].ready)
        tx(buffer[0]);
    else if()
        tx(buffer[1]);
}


int main () {

    size_t buf_sz = 40;
    int num_buffers = 2;
    std::vector<buf> buffers(num_buffers);



    

    delete []data;

    
    return 0;
}

