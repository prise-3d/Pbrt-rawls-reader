#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

#include "lodepng.h"
#include "rawls.h"
#include "rawls_v1.h"

int main(int argc, char *argv[]){

    std::string imageName;
    std::string outfileName;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--image") || !strcmp(argv[i], "-image")) {
            imageName = argv[++i];
        } else if (!strcmp(argv[i], "--outfile") || !strcmp(argv[i], "-outfile")) {
            outfileName = argv[++i];
        }
    }
    
    if (!(rawls::HasExtension(imageName, ".rawls"))){
        std::cout << "Unexpected `rawls` image name extension" << std::endl;
        return 1;
    }

    // std::cout << "Read image `" << imageName << "` and save it into " << outfileName << " into new RAWLS format" << std::endl;

    // get dimensions and data information from image
    unsigned width, height, nbChanels;
    float* buffer;

    std::tuple<unsigned, unsigned, unsigned, float*> data = rawls_v1::getDataRAWLS(imageName);

    width = std::get<0>(data);
    height = std::get<1>(data);
    nbChanels = std::get<2>(data);
    buffer = std::get<3>(data);

    auto comments = rawls_v1::getCommentsRAWLS(imageName);

    if (!(rawls::HasExtension(outfileName, ".rawls"))){
        std::cout << "Unexpected `rawls` image name extension" << std::endl;
        return 1;
    }

    rawls::saveAsRAWLS(width, height, nbChanels, comments, buffer, outfileName);
}