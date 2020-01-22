#include <iostream>
#include <string.h>
#include <memory>

#include "lodepng.h"
#include "rawls.h"

#include <algorithm>
#include <filesystem>
#include <regex>

void writeProgress(float progress, bool moveUp = false){
    int barWidth = 200;

    if (moveUp){
        // move up line
        std::cout << "\e[A";
        std::cout.flush();
    }

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

void insertSorted(unsigned size, float* values, float value){

    // avoid use of:
    //std::sort(values, values + size, std::greater<float>());
    unsigned position = 0;

    // find expected position value
    for (int i = 0; i < size; i++){

        if(value <= values[i]){
            break; // position found go out of the loop
        }
        position = i + 1;
    }

    // shift all values to right by one
    for (int i = size ; i > position; i--){        
        values[i] = values[i - 1];
    }

    // insert new value into found position (now free)
    values[position] = value;

}

float getMedianValue(unsigned size, float* values){

    if (size % 2 == 0)
    {
      return (values[size / 2 - 1] + values[size / 2]) / 2;
    }
    else 
    {
      return values[size / 2];
    }
}

/*
 * Save current step images from current buffer
 */
bool saveCurrentImage(int width, int height, int nbChanels, float* buffer, std::string outfileName, std::string comments){
    
    // create outfile 
    if (rawls::HasExtension(outfileName, ".ppm")){
        rawls::saveAsPPM(width, height, nbChanels, buffer, outfileName);
    } 
    else if (rawls::HasExtension(outfileName, ".png")){
        rawls::saveAsPNG(width, height, nbChanels, buffer, outfileName);
    } 
    else if (rawls::HasExtension(outfileName, ".rawls") || rawls::HasExtension(outfileName, ".rawls_20")){
    
        // Here no gamma conversion is done, only mean of samples
        rawls::saveAsRAWLS(width, height, nbChanels, comments, buffer, outfileName);
    }
    else{
        std::cout << "Unexpected output extension image" << std::endl;
        return false;
    }

    return true;
}

/*
 * Incremental merge of `rawls` images using `median-of-means`
 */
int main(int argc, char *argv[]){

    std::string folderName;
    std::string outputFolder;
    std::string prefixImageName;
    std::string imageExtension;

    unsigned step = 10;
    unsigned maxSamples = 0;
    bool random;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--folder") || !strcmp(argv[i], "-folder")) {
            folderName = argv[++i];
        } else if (!strcmp(argv[i], "--step") || !strcmp(argv[i], "-step")) {
            step = atoi(argv[++i]);
        }else if (!strcmp(argv[i], "--random") || !strcmp(argv[i], "-random")) {
            random = bool(atoi(argv[++i]));
        }else if (!strcmp(argv[i], "--output") || !strcmp(argv[i], "-output")) {
            outputFolder = argv[++i];
        }else if (!strcmp(argv[i], "--prefix") || !strcmp(argv[i], "-prefix")) {
            prefixImageName = argv[++i];
        }else if (!strcmp(argv[i], "--max") || !strcmp(argv[i], "-max")) {
            maxSamples = atoi(argv[++i]);
        }else if (!strcmp(argv[i], "--extension") || !strcmp(argv[i], "-extension")) {
            imageExtension = argv[++i];
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

    unsigned width, height, nbChanels;

    float** outputMeanBuffer; // stores means array for each sample
    float* outputStepBuffer; // buffer which stores kept median for each generated image (median is found using `outputMeanBuffer`)
    float* outputSumBuffer; // buffer which stores sum of new computed mean for each sample (then new mean is added to `outputMeanBuffer`)

    if (imagesPath.size() > 0){

        std::tuple<unsigned, unsigned, unsigned> dimensions = rawls::getDimensionsRAWLS(imagesPath.at(0));

        width = std::get<0>(dimensions);
        height = std::get<1>(dimensions);
        nbChanels = std::get<2>(dimensions);

        // init all pointers size
        outputMeanBuffer = new float*[width * height * nbChanels];
        outputSumBuffer = new float[width * height * nbChanels];
        outputStepBuffer = new float[width * height * nbChanels];

        // init values of buffer
        for (int i = 0; i < height * width * nbChanels; i++){
            // set as default size `maxSamples` by `step`
            outputMeanBuffer[i] = new float[maxSamples / step];
            outputSumBuffer[i] = 0;
            outputStepBuffer[i] = 0;
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
    std::string comments;

    // get comments if output is also `rawls` file
    if (rawls::HasExtension(imageExtension, "rawls")){
        comments = rawls::getCommentsRAWLS(imagesPath.at(0));
    }

    for (unsigned i = 0; i < maxSamples; i++){

        unsigned currentSample = i + 1;

        // read into folder all `.rawls` file and merge pixels values
        float* buffer = rawls::getPixelsRAWLS(imagesPath.at(i));

        for(unsigned y = 0; y < height; y++){

            for(unsigned x = 0; x < width; x++) {

                for(unsigned j = 0; j < nbChanels; j++){
                    
                    float value = buffer[nbChanels * width * y + nbChanels * x + j];

                    // add new `luminance` of chanel[j] found
                    outputSumBuffer[nbChanels * width * y + nbChanels * x + j] += value;
                }
            }
        }

        // save a new 
        if (currentSample % step == 0){

            unsigned nbElementsMean = currentSample / step;
            float currentMean;

            // get median all samples values by number of samples used (using MON method)
            for (int j = 0; j < height * width * nbChanels; j++){

                // get current mean for new samples luminances found
                currentMean = outputSumBuffer[j] / step;

                // insert this new mean into buffer (using `nbElementsMean - 1` because the new mean element is now inserted)
                insertSorted(nbElementsMean - 1, outputMeanBuffer[j], currentMean);

                // get meadian of these means as expected output luminance
                outputStepBuffer[j] = getMedianValue(nbElementsMean, outputMeanBuffer[j]);
            }


            // add suffix with `5` digits
            std::string suffix = std::to_string(currentSample);
            
            while(suffix.length() < 5){
                suffix = "0" + suffix;
            }

            // build output path of image
            std::string outfileName = outputFolder + "/" + prefixImageName + "_" + suffix + "." + imageExtension;
            outfileName = std::regex_replace(outfileName, std::regex("\\//"), "/"); // fix path 

            // save the expected `step` image using built outpath
            saveCurrentImage(width, height, nbChanels, outputStepBuffer, outfileName, comments);
            
            // reinit `sum` buffer in order to get next mean
            for (int j = 0; j < height * width * nbChanels; j++){
                outputSumBuffer[j] = 0;
            }

            // just for progress information with erasing previous info
            writeProgress(progress, true);
        }

        // update and write progress information
        progress += (1 / (float)maxSamples);
        writeProgress(progress);

        delete buffer;
    }
    
    writeProgress(1.);
    std::cout << std::endl;

    // clear all pointers memory
    for (int j = 0; j < height * width * nbChanels; j++){
        delete[] outputMeanBuffer[j];
    }

    delete outputMeanBuffer;
    delete outputSumBuffer;
    delete outputStepBuffer;
}