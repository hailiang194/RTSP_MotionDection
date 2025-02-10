#include <iostream>
#include <string>

#include "GStreamerWrapper/GStreamerWrapper.h"


int main(int argc, char** argv) 
{
    if (argc != 2) 
    {
        std::cout << "Usage:\n" << argv[0] << " <GStreamer pipelines before default appsink plugin> " << std::endl;
        return 0;
    }
    GStreamerWrapper wrapper(argc, argv);
    std::string pipeStr = argv[1];
    wrapper.Setup(pipeStr);
    wrapper.Run();
    return 0;
}