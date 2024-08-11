#include "application.h"
#include "gameconfig/editorconfig.h"
#include "levels/level_showcase.h"

int main(int argc, char* argv[])
{
    Application app;
    
    ApplicationSetupParams params;
    params.SetGameWindowSize(WINDOW_SIZE.x, WINDOW_SIZE.y);
    params.SetStartLevel<Level_Showcase>();

    app.Run(params);

    return 0;
}
