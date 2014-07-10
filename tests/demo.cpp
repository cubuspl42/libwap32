#include <wap32.h>
#include <iostream>
#include <memory>
#include <unistd.h>

using namespace std;

typedef unique_ptr<Wap32Wwd, void(*)(Wap32Wwd*)> unique_wwd_pointer;

int main(int argc, const char **argv)
{
    if(argc != 2) {
        cerr << "Usage: demo wwd_file" << endl;
        return 1;
    }
    
    Wap32Wwd *wwd = nullptr;
    int error = wap32_wwd_open(&wwd, argv[1]);
    unique_wwd_pointer wwd_ptr(wwd, wap32_wwd_free);
    
    if(error < 0) {
        cerr << "Error opening WWD file";
        const char *desc = wap32_err_description();
        if(desc)
            cerr << ": " << desc;
        cerr << endl;
        return 2;
    }
    cout << wap32_wwd_get_author(wwd) << endl;
}