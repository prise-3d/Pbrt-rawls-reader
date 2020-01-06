#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

#include "lodepng.h"
#include "rawls.h"

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
    
    if (!(rawls::HasExtension(imageName, ".rawls") || rawls::HasExtension(imageName, ".rawls_20") )){
        std::cout << "Unexpected `rawls` image name extension" << std::endl;
        return 1;
    }

    std::cout << "Read image `" << imageName << "` and save it into " << outfileName << std::endl;

    // create outfile 
    if (rawls::HasExtension(outfileName, ".ppm")){
        rawls::convertToPPM(imageName, outfileName);
    } 
    else if (rawls::HasExtension(outfileName, ".png")){
        rawls::convertToPNG(imageName, outfileName);
    }
    else{
        std::cout << "Unexpected output extension image" << std::endl;
    }
}