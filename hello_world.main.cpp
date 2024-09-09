#include <thread>

#include "include/hello_world.gui.hpp"
#include "include/hello_world.audio.hpp"
#include "include/SharedProps.hpp"

int main(int , char *[])
{
    SharedProps sharedProps;


    std::thread Audio(AUDIO, std::ref(sharedProps));
    std::thread Gui  (GUI,   std::ref(sharedProps));

    Audio.join();
    Gui.join();

    return 0;
}
