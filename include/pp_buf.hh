#include <vector>
#include <cstdint>

class buf {
private:
    static int total;
    int id;
    size_t buf_sz;
    
    
    std::vector<int16_t> dat;
public:
    bool empty;
    bool read_first;

    buf(int sz);
    buf();
    ~buf();
    void read_bf();
    void write_bf(std::vector<int16_t> &data);
    void set_sz(int sz);

};

int buf::total = 0;

// write will take the argument and write data into buf
// read will print data out to stdio.

buf::buf(int sz) {
    total++;
    id = total;
    buf_sz = sz;
    empty = true;
    read_first = true;
    dat.reserve(sz);
}

buf::buf(){
    std::cout << "constructor called" << std::endl;
    total++;
    id = total;
    empty = true;
    if (id == 1) read_first = true;
    else read_first = false;
}
buf::~buf(){
    total--;
}



void buf::set_sz(int sz) {
    buf_sz = sz;
    dat.reserve(sz);
}

void buf::write_bf(std::vector<int16_t> &data) {
    printf("writing buffers into mem in buf %d...\n",id);
    for (int i = 0; i < buf_sz; i ++) {
        dat[i] = data[i]; 
    }
    empty = false;
}

void buf::read_bf() {
    printf("reading buffers into stdout for buf %d...\n",id);
    for (int i = 0; i < buf_sz; i ++) {
        std::cout <<"B" << id <<": "<<dat[i] << std::endl; 
    }
    empty = true; 
}