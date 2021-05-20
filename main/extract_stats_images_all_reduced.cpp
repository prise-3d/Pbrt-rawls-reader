#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>
#include <numeric>  
#include <map>
#include <algorithm>
#include <filesystem>
#include <unistd.h> 

#include <bits/stdc++.h> 
#include <iostream> 
#include <sys/stat.h> 
#include <sys/types.h> 

#include "rawls.h"

struct Point {
    unsigned x;
    unsigned y;
};

struct Tile {
    Point p1;
    Point p2;
};

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void writeProgress(float progress, bool moveUp = false){
    int barWidth = 150;

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

float getMedian(std::vector<float> &values) {
    std::sort(values.begin(), values.end());

    unsigned size = values.size();

    if (size % 2 == 0)
    {
        return (values[size / 2 - 1] + values[size / 2]) / 2;
    }
    else 
    {
        return values[size / 2];
    }
}

float getVariance(float &mean, std::vector<float> &values){   

    // Now calculate the variance
    auto variance_func = [&mean](float accumulator, const float& val) {
        return accumulator + pow(val - mean, 2);
    };

    return std::accumulate(values.begin(), values.end(), 0.0, variance_func) / values.size();
}

float getSkewness(float &mean, float &std, std::vector<float> &values) {
    unsigned size = values.size();

    // Now calculate the sum of pow 3
    auto order3_func = [&mean, &std](float accumulator, const float& val) {
        return accumulator + pow((val - mean) / std, 3);
    };

    float order3 = std::accumulate(values.begin(), values.end(), 0.0, order3_func);

    return order3 / size;
}

float getKurtosis(float &mean, float &std, std::vector<float> &values){
    unsigned size = values.size();

    // Now calculate the sum of pow 4
    auto order4_func = [&mean, &std](float accumulator, const float& val) {
        return accumulator + pow((val - mean) / std, 4);
    };

    float order4 = std::accumulate(values.begin(), values.end(), 0.0, order4_func);

    return order4 / size;
}

float getMode(std::vector<float> &values) {

     std::vector<float> pvalues;

    for (unsigned i = 0; i < values.size(); i++){
        pvalues.push_back(roundf(values.at(i) * 100) / 100.0);
    }

    typedef std::map<float,unsigned int> CounterMap;
    CounterMap counts;
    for (int i = 0; i < pvalues.size(); ++i)
    {
        CounterMap::iterator it(counts.find(pvalues[i]));
        if (it != counts.end()){
            it->second++;   
        } else {
            counts[pvalues[i]] = 1;
        }
    }

    // Create a map iterator and point to beginning of map
    std::map<float, unsigned int>::iterator it = counts.begin();
    unsigned noccurences = 0;
    float modeValue = 0.;
    // Iterate over the map using Iterator till end.
    while (it != counts.end())
    {
        // Accessing KEY from element pointed by it.
        float potentialMode = it->first;
        // Accessing VALUE from element pointed by it.
        unsigned count = it->second;

        if (count > noccurences) {
            noccurences = count;
            modeValue = potentialMode;
        }

        // Increment the Iterator to point to next entry
        it++;
    }

    return modeValue;
}

// float getEstimator(std::string estimator, std::vector<float> values) {

//     // another version of scripts in order to quick compute data
//     if (estimator == "median") {
        
//         std::sort(values.begin(), values.end());

//         unsigned size = values.size();

//         if (size % 2 == 0)
//         {
//             return (values[size / 2 - 1] + values[size / 2]) / 2;
//         }
//         else 
//         {
//             return values[size / 2];
//         }
//     } else if (estimator == "mean") {

//         return std::accumulate(values.begin(), values.end(), 0.0) / values.size(); 

//     } else if (estimator == "var") {
//         // Calculate the mean
//         const float mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();

//         // Now calculate the variance
//         auto variance_func = [&mean](float accumulator, const float& val) {
//             return accumulator + pow(val - mean, 2);
//         };

//         return std::accumulate(values.begin(), values.end(), 0.0, variance_func) / values.size();

//     } else if (estimator == "std") {

//         return sqrt(getEstimator("var", values));

//     } else if (estimator == "skewness") {

//         unsigned size = values.size();

//         float mean = getEstimator("mean", values);
//         float std = getEstimator("std", values);

//         // Now calculate the sum of pow 3
//         auto order3_func = [&mean, &std](float accumulator, const float& val) {
//             return accumulator + pow((val - mean) / std, 3);
//         };

//         float order3 = std::accumulate(values.begin(), values.end(), 0.0, order3_func);

//         return order3 / size;

//     } else if (estimator == "kurtosis") {
        
//         unsigned size = values.size();

//         float mean = getEstimator("mean", values);
//         float std = getEstimator("std", values);

//         // Now calculate the sum of pow 4
//         auto order4_func = [&mean, &std](float accumulator, const float& val) {
//             return accumulator + pow((val - mean) / std, 4);
//         };

//         float order4 = std::accumulate(values.begin(), values.end(), 0.0, order4_func);

//         return order4 / size;

//     } else if (estimator == "mode") {

//         std::vector<float> pvalues;

//         for (unsigned i = 0; i < values.size(); i++){
//             pvalues.push_back(roundf(values.at(i) * 100) / 100.0);
//         }

//         typedef std::map<float,unsigned int> CounterMap;
//         CounterMap counts;
//         for (int i = 0; i < pvalues.size(); ++i)
//         {
//             CounterMap::iterator it(counts.find(pvalues[i]));
//             if (it != counts.end()){
//                 it->second++;   
//             } else {
//                 counts[pvalues[i]] = 1;
//             }
//         }

//         // Create a map iterator and point to beginning of map
//         std::map<float, unsigned int>::iterator it = counts.begin();
//         unsigned noccurences = 0;
//         float modeValue = 0.;
//         // Iterate over the map using Iterator till end.
//         while (it != counts.end())
//         {
//             // Accessing KEY from element pointed by it.
//             float potentialMode = it->first;
//             // Accessing VALUE from element pointed by it.
//             unsigned count = it->second;

//             if (count > noccurences) {
//                 noccurences = count;
//                 modeValue = potentialMode;
//             }

//             // Increment the Iterator to point to next entry
//             it++;
//         }

//         return modeValue;
//     }

//     // by default
//     return 0.;
// }

int main(int argc, char *argv[]){

    std::string folderName;
    std::vector<std::string> estimators = {"mean", "median", "var", "std", "skewness", "kurtosis", "mode"};
    unsigned blockHeight;
    unsigned blockWidth;
    unsigned nfiles = 10000;
    std::string outputFolder;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--folder") || !strcmp(argv[i], "-folder")) {
            folderName = argv[++i];
        } else if (!strcmp(argv[i], "--bwidth") || !strcmp(argv[i], "-bwidth")) {
            blockHeight = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--bheight") || !strcmp(argv[i], "-bheight")) {
            blockWidth = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--output") || !strcmp(argv[i], "-output")) {
            outputFolder = argv[++i];
        } else if (!strcmp(argv[i], "--nfiles") || !strcmp(argv[i], "-nfiles")) {
            nfiles = atoi(argv[++i]);
        }
    }

    // create outputs directory
    mkdir(outputFolder.c_str(), 0755);

    auto elements = split(folderName, '/');
    std::string sceneName = elements.at(elements.size() - 1);

    for (int i = 0; i < estimators.size(); i++) {

        mkdir((outputFolder + "/" + estimators[i]).c_str(), 0755);
        mkdir((outputFolder + "/" + estimators[i] + "/" + sceneName).c_str(), 0755);
    }


    // get all files path
    std::vector<std::string> imagesPath;

    for (const auto & entry : std::filesystem::directory_iterator(folderName)){
        std::string imageName = entry.path().string();
        if (rawls::HasExtension(imageName, ".rawls") || rawls::HasExtension(imageName, ".rawls_20")){
            imagesPath.push_back(imageName);
        }
    }

    if (imagesPath.size() != nfiles) {
        return 0;
    }

    std::sort(imagesPath.begin(), imagesPath.end());

    std::tuple<unsigned, unsigned, unsigned> data = rawls::getDimensionsRAWLS(imagesPath.at(0));

    unsigned outputWidth = std::get<0>(data);
    unsigned outputHeight = std::get<1>(data);
    unsigned nbChanels = std::get<2>(data);

    std::vector<float*> outputBuffers;
    std::vector<std::string> outputFiles;
    std::vector<std::string> selectedEstimators;
    // new buffer size as new output buffer image (default 3 channels)

    for (int i = 0; i < estimators.size(); i++) {

        std::string outputFile = outputFolder + "/" + estimators[i] + "/" + sceneName + "/" + sceneName + ".rawls";

        std::ifstream ifile;
        ifile.open(outputFile);
        if(!ifile) {
        
            // create new buffer entry
            selectedEstimators.push_back(estimators[i]);
            outputFiles.push_back(outputFile);
            outputBuffers.push_back(new float[outputHeight * outputWidth * nbChanels]);
        
        } else {
            ifile.close();
        }
    }

    // get all tiles to apply
    unsigned nWidth = ceil(outputWidth / (float)blockWidth);
    unsigned nHeight = ceil(outputHeight / (float)blockHeight);

    std::vector<Tile> tiles;

    for (unsigned i = 0; i < nWidth; i++) {
        for (unsigned j = 0; j < nHeight; j++) {

            unsigned x1 = i * blockWidth;
            unsigned y1 = j * blockHeight;

            unsigned x2 = i * blockWidth + blockWidth;
            unsigned y2 = j * blockHeight + blockHeight;

            x2 = x2 > outputWidth ? outputWidth: x2;
            y2 = y2 > outputHeight ? outputHeight: y2;
            
            Point p1 = {x1, y1};
            Point p2 = {x2, y2};

            Tile tile = {p1, p2};
            tiles.push_back(tile);
        }
    }

    unsigned nsamples = imagesPath.size();
    unsigned nloop = tiles.size() * nsamples;
    unsigned nloopCounter = 0;

    for (unsigned t_index = 0; t_index < tiles.size(); t_index++){

        Tile tile = tiles.at(t_index);

        //std::cout << "Tile: (" << tile.p1.x << ", " << tile.p1.y << ")" << " => " << "(" << tile.p2.x << ", " << tile.p2.y << ")" << std::endl;

        unsigned nvalues = (tile.p2.x - tile.p1.x) * (tile.p2.y - tile.p1.y) * 3;

        std::vector<std::vector<float>> rgbValues(nvalues);

        for (unsigned i = 0; i < nsamples; i++) {
            
            try {
                
                float* RGBpixels = rawls::getPixelsRAWLS(imagesPath.at(i));
                std::cout << "Read image n°" << i << " / " << nsamples << " for tile n°" << t_index << " / " << tiles.size() << std::endl;

                unsigned index = 0;
                for (int y = tile.p1.y; y < tile.p2.y; ++y) {
                    for (int x = tile.p1.x; x < tile.p2.x; ++x) {
            
                        rgbValues.at(index).push_back(RGBpixels[3 * (y * outputWidth + x) + 0]);
                        rgbValues.at(index + 1).push_back(RGBpixels[3 * (y * outputWidth + x) + 1]);
                        rgbValues.at(index + 2).push_back(RGBpixels[3 * (y * outputWidth + x) + 2]);

                        index += 3;
                    
                    }
                }

                delete RGBpixels;

            } catch(std::exception& e){
                std::cout << "Error occurs when reading file" << std::endl;
            }

            // display progress
            nloopCounter += 1;
            writeProgress(nloopCounter / (float)nloop);
        }


        // for (int i = 0; i < outputFiles.size(); i++) {
        
        // extract stat and add predicted value into output buffer
        unsigned index = 0;

        for (int y = tile.p1.y; y < tile.p2.y; ++y) {
            for (int x = tile.p1.x; x < tile.p2.x; ++x) {
                
                // Here we will compute each estimator in specific order
                // => {"median", "var", "std", "skewness", "kurtosis", "mode"}
                auto rvalues = rgbValues.at(index + 0);
                auto gvalues = rgbValues.at(index + 1);
                auto bvalues = rgbValues.at(index + 2);
                
                // Index [0] : MEAN
                float rmean = std::accumulate(rvalues.begin(), rvalues.end(), 0.0) / rvalues.size();
                float gmean = std::accumulate(gvalues.begin(), gvalues.end(), 0.0) / gvalues.size();
                float bmean = std::accumulate(bvalues.begin(), bvalues.end(), 0.0) / bvalues.size();

                outputBuffers.at(0)[3 * (y * outputWidth + x) + 0] = rmean;
                outputBuffers.at(0)[3 * (y * outputWidth + x) + 1] = gmean;
                outputBuffers.at(0)[3 * (y * outputWidth + x) + 2] = bmean;

                // Index [1] : MEDIAN
                float rmedian = getMedian(rvalues);
                float gmedian = getMedian(gvalues);
                float bmedian = getMedian(bvalues);

                outputBuffers.at(1)[3 * (y * outputWidth + x) + 0] = rmedian;
                outputBuffers.at(1)[3 * (y * outputWidth + x) + 1] = gmedian;
                outputBuffers.at(1)[3 * (y * outputWidth + x) + 2] = bmedian;

                // Index [2] : VARIANCE
                float rvariance = getVariance(rmean, rvalues);
                float gvariance = getVariance(gmean, gvalues);
                float bvariance = getVariance(bmean, bvalues);

                outputBuffers.at(2)[3 * (y * outputWidth + x) + 0] = rvariance;
                outputBuffers.at(2)[3 * (y * outputWidth + x) + 1] = gvariance;
                outputBuffers.at(2)[3 * (y * outputWidth + x) + 2] = bvariance;

                // Index [3] : STD
                float rstd = sqrt(rvariance);
                float gstd = sqrt(gvariance);
                float bstd = sqrt(bvariance);

                outputBuffers.at(3)[3 * (y * outputWidth + x) + 0] = rstd;
                outputBuffers.at(3)[3 * (y * outputWidth + x) + 1] = gstd;
                outputBuffers.at(3)[3 * (y * outputWidth + x) + 2] = bstd;

                // Index [4] : SKEWNESS
                float rskew = getSkewness(rmean, rstd, rvalues);
                float gskew = getSkewness(gmean, gstd, gvalues);
                float bskew = getSkewness(bmean, bstd, bvalues);

                outputBuffers.at(4)[3 * (y * outputWidth + x) + 0] = rskew;
                outputBuffers.at(4)[3 * (y * outputWidth + x) + 1] = gskew;
                outputBuffers.at(4)[3 * (y * outputWidth + x) + 2] = bskew;

                // Index [5] : KURTOSIS
                float rkurtosis = getKurtosis(rmean, rstd, rvalues);
                float gkurtosis = getKurtosis(gmean, gstd, gvalues);
                float bkurtosis = getKurtosis(bmean, bstd, bvalues);

                outputBuffers.at(5)[3 * (y * outputWidth + x) + 0] = rkurtosis;
                outputBuffers.at(5)[3 * (y * outputWidth + x) + 1] = gkurtosis;
                outputBuffers.at(5)[3 * (y * outputWidth + x) + 2] = bkurtosis;

                // Index [6] : MODE (TODO: check computation time and if very necessary)
                float rmode = getMode(rvalues);
                float gmode = getMode(gvalues);
                float bmode = getMode(bvalues);

                outputBuffers.at(6)[3 * (y * outputWidth + x) + 0] = rmode;
                outputBuffers.at(6)[3 * (y * outputWidth + x) + 1] = gmode;
                outputBuffers.at(6)[3 * (y * outputWidth + x) + 2] = bmode;                

                index += 3;
            }
        }
        // }
    }

    // Save here new rawls image
    std::string comments = rawls::getCommentsRAWLS(imagesPath.at(0));

    for (int i = 0; i < outputFiles.size(); i++) {

        // construct specific outfile name
        bool success = rawls::saveAsRAWLS(outputWidth, outputHeight, nbChanels, comments, outputBuffers[i], outputFiles[i]);

        if (success) {
            std::cout << "New image saved into " << outputFiles[i] << std::endl;
        }
        else
        {
            std::cout << "Error while saving current image " << outputFiles[i] << std::endl;
        }

        delete outputBuffers[i];
    }

}