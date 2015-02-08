#include "main.hpp"
int main()
{
    //The main rendering window that we will open to
    //view scenes in.
    Render mainWindow("My Test Window", 800, 600);

    //We will setup cameras at different positions that
    //we can move around or add new ones.
    std::vector<Camera> myCameras;

    return 0;
}
