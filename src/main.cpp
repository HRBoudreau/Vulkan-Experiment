
#include <iostream>
#include <App.h>

int main() {
    try {
    App();
    }
    catch ( std::runtime_error e ) {
        std::cout << e.what();
    }
    return 0;
}