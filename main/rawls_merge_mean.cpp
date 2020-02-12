#include <iostream>
#include <string.h>
#include <memory>

#include "lodepng.h"
#include "rawls.h"

#include <algorithm>
#include <filesystem>

void writeProgress(float progress){
    int barWidth = 200;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

int main(int argc, char *argv[]){

    std::string folderName;
    std::string outfileName;
    unsigned nbSamples = 10;
    unsigned startIndex = 0;
    bool random;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--folder") || !strcmp(argv[i], "-folder")) {
            folderName = argv[++i];
        } else if (!strcmp(argv[i], "--samples") || !strcmp(argv[i], "-samples")) {
            nbSamples = atoi(argv[++i]);
        }else if (!strcmp(argv[i], "--random") || !strcmp(argv[i], "-random")) {
            random = bool(atoi(argv[++i]));
        }else if (!strcmp(argv[i], "--outfile") || !strcmp(argv[i], "-ouftile")) {
            outfileName = argv[++i];
        }else if (!strcmp(argv[i], "--startindex") || !strcmp(argv[i], "-startindex")) {
            startIndex = atoi(argv[++i]);
        }
    }

    std::vector<std::string> imagesPath;

    for (const auto & entry : std::filesystem::directory_iterator(folderName)){
        std::string imageName = entry.path().string();
        if (rawls::HasExtension(imageName, ".rawls") || rawls::HasExtension(imageName, ".rawls_20")){
            imagesPath.push_back(imageName);
        }
    }

    // sort or shuffle the images path
    if (!random){
        std::sort(imagesPath.begin(), imagesPath.end(), std::less<std::string>());
    }else{
        std::random_shuffle(imagesPath.begin(), imagesPath.end());
    }

    // check nSamples
    if (nbSamples > imagesPath.size()){
        nbSamples = imagesPath.size();
    }


    unsigned width, height, nbChanels;
    float* outputBuffer;

    if (imagesPath.size() > 0){

        std::tuple<unsigned, unsigned, unsigned> dimensions = rawls::getDimensionsRAWLS(imagesPath.at(0));

        width = std::get<0>(dimensions);
        height = std::get<1>(dimensions);
        nbChanels = std::get<2>(dimensions);
        outputBuffer = new float[width * height * nbChanels];

        // init values of buffer
        for (int i = 0; i < height * width * nbChanels; i++){
            outputBuffer[i] = 0;
        }
    }
    else
    {
        std::cout << "Folder is empty..." << std::endl;
        return 1;
    }

    // just for indication
    float progress = 0.0;
    unsigned bufferSize = width * height * nbChanels;

    for (unsigned i = startIndex; i < (nbSamples + startIndex); i++){

        // read into folder all `.rawls` file and merge pixels values
        float* buffer = rawls::getPixelsRAWLS(imagesPath.at(i));

        for(unsigned y = 0; y < height; y++){

            for(unsigned x = 0; x < width; x++) {

                for(unsigned j = 0; j < nbChanels; j++){
                    
                    float value = buffer[nbChanels * width * y + nbChanels * x + j];
                    outputBuffer[nbChanels * width * y + nbChanels * x + j] = outputBuffer[nbChanels * width * y + nbChanels * x + j] + value;
                }
            }
        }

        // update and write progress information
        progress += (1 / (float)nbSamples);
        writeProgress(progress);

        delete buffer;
    }
    
    writeProgress(1.);
    std::cout << std::endl;

    // mean all samples values by number of samples used
    for (int i = 0; i < height * width * nbChanels; i++){
        outputBuffer[i] = outputBuffer[i] / nbSamples;
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
        std::string comments = rawls::getCommentsRAWLS(imagesPath.at(0));

        // Here no gamma conversion is done, only mean of samples
        rawls::saveAsRAWLS(width, height, nbChanels, comments, outputBuffer, outfileName);
    } 
    else{
        std::cout << "Unexpected output extension image" << std::endl;
    }

    // delete the outputbuffer used
    delete outputBuffer;
}