#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include "pp_buf.hh"



void rx(int16_t data[], size_t sz, buf &buffer ) {
    std::vector<int16_t> v(data, data + sz);
    buffer.set_sz(sz);
    buffer.write_bf(v);

}


void tx(buf &buffer) {
    buffer.read_bf();
}


int main () {

    size_t buf_sz = 40;
    int num_buffers = 2;
    std::vector<buf> buffers(2);
    //buffers.reserve(2);

    
    int16_t data[buf_sz] = {};
    for (int i = 0; i < buf_sz; i++) {
        data[i] = (int16_t)i;
    }

    rx(data, buf_sz, buffers[0]);
    //tx(buffers[0]);
    

    

    
    return 0;
}

