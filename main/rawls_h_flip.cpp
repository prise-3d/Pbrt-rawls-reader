#include <stdio.h>

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
    

    // extract data from image
    unsigned width, height, nbChanels;
    float* buffer;
    float* outputBuffer;

    std::tuple<unsigned, unsigned, unsigned, float*> data = rawls::getDataRAWLS(imageName);

    width = std::get<0>(data);
    height = std::get<1>(data);
    nbChanels = std::get<2>(data);
    buffer = std::get<3>(data);

    outputBuffer = new float[width * height * nbChanels];

    
    for(unsigned y = 0; y < height; y++){

        for(unsigned x = 0; x < width; x++) {

            unsigned reversedWidth = width - x;

            for(unsigned j = 0; j < nbChanels; j++){
                
                float value = buffer[nbChanels * width * y + nbChanels * reversedWidth + j];
                outputBuffer[nbChanels * width * y + nbChanels * x + j] = value;
            }
        }
    }

    // create outfile 
    if (rawls::HasExtension(outfileName, ".ppm")){
        rawls::saveAsPPM(width, height, nbChanels, outputBuffer, outfileName);
    } 
    else if (rawls::HasExtension(outfileName, ".png")){
        rawls::saveAsPNG(width, height, nbChanels, outputBuffer, outfileName);
    } 
    else if (rawls::HasExtension(outfileName, ".rawls") || rawls::HasExtension(outfileName, ".rawls_20")){
        // need to get comments from an image
        std::string comments = rawls::getCommentsRAWLS(imageName);

        // Here no gamma conversion is done, only mean of samples
        rawls::saveAsRAWLS(width, height, nbChanels, comments, outputBuffer, outfileName);
    } 
    else{
        std::cout << "Unexpected output extension image" << std::endl;
    }
}