#include "app/Lab4App.h"
#include <cstdio>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    std::printf("AI Lab 4 - Fuzzy Weather (Krystallic Engine)\n");

    CLab4App app;
    if (!app.Initialize())
        return 1;

    return app.Run();
}
