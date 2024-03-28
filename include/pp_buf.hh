#include <vector>
#include <cstdint>

class buf {
private:
    static int total;
    int id;
    size_t buf_sz;
    bool ready;
    bool busy;
    std::vector<int16_t> dat;
public:
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
    dat.reserve(sz);
}

buf::buf(){
    std::cout << "constructor called" << std::endl;
    total++;
    id = total;
}
buf::~buf(){
    total--;
}





void buf::set_sz(int sz) {
    buf_sz = sz;
    dat.reserve(sz);
}

void buf::write_bf(std::vector<int16_t> &data) {
    printf("writing buffers into mem...\n");
    for (int i = 0; i < buf_sz; i ++) {
        dat[i] = data[i]; 
    }
}

void buf::read_bf() {
    printf("reading buffers into stdout...\n");
    for (int i = 0; i < buf_sz; i ++) {
        std::cout <<"B" << id <<": "<<dat[i] << std::endl; 
    }
}