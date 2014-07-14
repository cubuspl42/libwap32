#include <wap32.h>
#include "../src/wwd.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <unistd.h>

using namespace std;

typedef unique_ptr<Wap32Wwd, void(*)(Wap32Wwd*)> unique_wwd_pointer;

static unsigned WapMapChecksum(std::istream * psStream, int piOffset)
{
    int pos = psStream->tellg();
    unsigned int length;
    psStream->seekg (0, std::ios::end);
    length = psStream->tellg();
    psStream->seekg(piOffset, std::ios::beg);
    unsigned int newCheckSum = 4294967295 - 159;
    for(unsigned int n=piOffset; n<length; n++){
        unsigned char buffer;
        psStream->read((char*)&buffer, 1);
        newCheckSum = newCheckSum - (n-(piOffset-1)) + (unsigned int)buffer;
    }
    psStream->seekg(pos);
    return newCheckSum;
}

int main(int argc, const char **argv)
{
    for(int i = 1; i < argc; ++i) {
        const char *filename = argv[i];
        cerr << filename << endl;
        
        Wap32Wwd *wwd = nullptr;
        int error = wap32_wwd_open(&wwd, filename);
        unique_wwd_pointer wwd_ptr(wwd, wap32_wwd_free);        
        
        if(error < 0) {
            cerr << "Error opening WWD file" << endl;
            return 2;
        }
    }
}