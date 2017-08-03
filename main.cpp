#include <iostream>
#include <cmath>

#include "sfml_visualizer.h"

int main()
{
    try {
        SFML_Visualizer vis;
        vis.run();
    }
    catch (std::runtime_error& ex) {
        std::cout << ex.what();
        return -1;
    }
    return 0;
}
