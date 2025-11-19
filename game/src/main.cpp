#include "app.h"

int main(int, char*[]) {

    App app;
    if(!app.Init())
        return 1;

    app.Run();
    return 0;
}