#include "rawls.h"

#include <fstream>
#include <memory>
#include "lodepng.h"

/*
 * Read all pixels values from .rawls image file and save as PPM image
 */
bool rawls::convertToPPM(std::string imageName, std::string outfileName){

    // get dimensions and data information from image
    unsigned width, height, nbChanels;
    float* buffer;

    std::tuple<unsigned, unsigned, unsigned, float*> data = getDataRAWLS(imageName);

    width = std::get<0>(data);
    height = std::get<1>(data);
    nbChanels = std::get<2>(data);
    buffer = std::get<3>(data);

    return saveAsPPM(width, height, nbChanels, buffer, outfileName);
}

/*
 * Read all pixels values from .rawls image file and save as PNG image
 */
bool rawls::convertToPNG(std::string imageName, std::string outfileName){

    // get dimensions and data information from image
    unsigned width, height, nbChanels;
    float* buffer;

    std::tuple<unsigned, unsigned, unsigned, float*> data = getDataRAWLS(imageName);

    width = std::get<0>(data);
    height = std::get<1>(data);
    nbChanels = std::get<2>(data);
    buffer = std::get<3>(data);

    return saveAsPNG(width, height, nbChanels, buffer, outfileName);
}


/*
 * Convert buffer using dimensions to PPM image format
 */
bool rawls::saveAsPPM(unsigned width, unsigned height, unsigned nbChanels, float* buffer, std::string outfileName){

    // open buffer
    std::ofstream outfile(outfileName);

    outfile << "P" << nbChanels << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << "255" << std::endl;

    for(unsigned y = 0; y < height; y++){

        for(unsigned x = 0; x < width; x++) {

            // for each chanel read and keep save float value i nto new file
            for(int j = 0; j < nbChanels; j++){
                outfile << int(GammaConvert(buffer[nbChanels * width * y + nbChanels * x + j])) << " "; 
            } 
        }

        outfile << std::endl;
    }

    std::cout << "Image is now saved as .ppm into " << outfileName << std::endl;

    outfile.close();

    if(!outfile.good()) {
        std::cout << "Error occurred at writing time!" << std::endl;
        return false;
    }

    return true;
}


/*
 * Convert buffer using dimensions to PNG image format
 */
bool rawls::saveAsPNG(unsigned width, unsigned height, unsigned nbChanels, float* buffer, std::string outfileName){

    // image buffer
    unsigned nbChanelsAlpha = nbChanels + 1;
    std::unique_ptr<uint8_t[]> outBuf(new uint8_t[width * height * nbChanelsAlpha]);
    uint8_t *image = outBuf.get();

    for(unsigned y = 0; y < height; y++){

        for(unsigned x = 0; x < width; x++) {

            for(int j = 0; j < nbChanels; j++){

                float value = buffer[nbChanels * width * y + nbChanels * x + j];
                image[nbChanelsAlpha * width * y + nbChanelsAlpha * x + j] = int(GammaConvert(value)); 
            } 

            // alpha channel
            image[nbChanelsAlpha * width * y + nbChanelsAlpha * x + 3] = 255; 
        }
    }
    
    unsigned error = lodepng::encode(outfileName, image, width, height);
   
    //if there's an error, display it
    if(error) {
        std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
        return false;
    }

    std::cout << "Image is now saved as .png into " << outfileName << std::endl;
    
    return true;
}


bool rawls::saveAsRAWLS(unsigned width, unsigned height, unsigned nbChanels, std::string comments, float* buffer, std::string outfileName){
    
    std::ofstream outfile(outfileName, std::ios::out | std::ios::binary);

    outfile << "IHDR" << std::endl;
    outfile << ((sizeof(width) + sizeof(height) + sizeof(nbChanels)))  << std::endl;
    outfile.write((char *) &width, sizeof(width));
    outfile.write((char *) &height, sizeof(height));
    outfile.write((char *) &nbChanels, sizeof(nbChanels));
    outfile << std::endl;

    // Part 2
    // Comments (usefull information about scene and generation data used)
    outfile << "COMMENTS" << std::endl;
    outfile << comments;
    
    // Part 3
    // using data information write specific chunck
    outfile << "DATA" << std::endl;
    outfile << (sizeof(float) * nbChanels * width * height) << std::endl;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float pixel[3];

            pixel[0] = buffer[3 * (y * width + x) + 0];
            pixel[1] = buffer[3 * (y * width + x) + 1];
            pixel[2] = buffer[3 * (y * width + x) + 2];
            
            outfile.write((char *) &pixel[0], sizeof(pixel[0]));
            outfile.write((char *) &pixel[1], sizeof(pixel[1]));
            outfile.write((char *) &pixel[2], sizeof(pixel[2]));
        }
        outfile << std::endl;
    }

    outfile.close();
    
    if(!outfile.good()) {
        std::cout << "Error occurred at writing time!" << std::endl;
        return false;
    }

    // std::cout << "Image is now saved as .rawls into " << outfileName << std::endl;

    return true;
}


float* rawls::getPixelsRAWLS(std::string filename){

    // get dimensions information from image
    unsigned width, height, nbChanels;

    std::tuple<unsigned, unsigned, unsigned> dimensions = getDimensionsRAWLS(filename);

    width = std::get<0>(dimensions);
    height = std::get<1>(dimensions);
    nbChanels = std::get<2>(dimensions);

    std::ifstream rf(filename, std::ios::out | std::ios::binary);

    if(!rf) {
      std::cout << "Cannot open file!" << std::endl;
    }

    std::string line; 
    char c; // to get space of new line char
  
    bool dataBegin = false;

    // READ DATA info
    // case of data chunck begin
    while (!dataBegin && std::getline(rf, line)) { 

        if (line.find(std::string("DATA")) != std::string::npos){
            dataBegin = true;
        }
    }

    // resize buffer if necessary
    float* buffer = new float[height * width * nbChanels];

    std::getline(rf, line);
    unsigned size = std::stoi(line);
    float chanelValue;

    for(unsigned y = 0; y < height; y++){

        for(unsigned x = 0; x < width; x++) {

            for(int j = 0; j < nbChanels; j++){
                rf.read((char *) &chanelValue, sizeof(float));  
                
                buffer[nbChanels * width * y + nbChanels * x + j] = chanelValue; 
            } 
        }
        // go to next line
        rf.get(c);
    }

    rf.close();

    if(!rf.good()) {
        std::cout << "Error occurred at writing time!" << std::endl;
    }

    return buffer;
}


std::tuple<unsigned, unsigned, unsigned> rawls::getDimensionsRAWLS(std::string filename){

    std::ifstream rf(filename, std::ios::out | std::ios::binary);

    if(!rf) {
      std::cout << "Cannot open file!" << std::endl;
    }

    std::string line; 
    unsigned nbChanels, width, height;
    char c; // to get space of new line char

    // READ IHDR info
    bool ihdrBegin = false;

    while (!ihdrBegin && std::getline(rf, line)) { 

        if (line.find(std::string("IHDR")) != std::string::npos){
            ihdrBegin = true;
            std::getline(rf, line); // avoid data size line

            rf.read((char *) &width, sizeof(unsigned));
            rf.read((char *) &height, sizeof(unsigned));
            rf.read((char *) &nbChanels, sizeof(unsigned));
            rf.get(c);
        }
    }

    rf.close();

    if(!rf.good()) {
        std::cout << "Error occurred at writing time!" << std::endl;
    }

    return std::make_tuple(width, height, nbChanels);
}


std::tuple<unsigned, unsigned, unsigned, float*> rawls::getDataRAWLS(std::string filename){

    // only one read buffer used for the whole function
    std::ifstream rf(filename, std::ios::out | std::ios::binary);

    if(!rf) {
      std::cout << "Cannot open file!" << std::endl;
    }

    std::string line; 
    unsigned nbChanels, width, height;
    char c; // to get space of new line char

    // READ IHDR info
    bool ihdrBegin = false;

    while (!ihdrBegin && std::getline(rf, line)) { 

        if (line.find(std::string("IHDR")) != std::string::npos){
            ihdrBegin = true;
            std::getline(rf, line); // avoid data size line

            rf.read((char *) &width, sizeof(unsigned));
            rf.get(c);
            rf.read((char *) &height, sizeof(unsigned));
            rf.get(c);
            rf.read((char *) &nbChanels, sizeof(unsigned));
            rf.get(c);
        }
    }

    bool dataBegin = false;

    // READ DATA info
    // case of data chunck begin
    while (!dataBegin && std::getline(rf, line)) { 

        if (line.find(std::string("DATA")) != std::string::npos){
            dataBegin = true;
        }
    }

    // resize buffer if necessary
    float* buffer = new float[height * width * nbChanels];

    std::getline(rf, line);
    unsigned size = std::stoi(line);
    float chanelValue;

    for(unsigned y = 0; y < height; y++){

        for(unsigned x = 0; x < width; x++) {

            for(int j = 0; j < nbChanels; j++){
                rf.read((char *) &chanelValue, sizeof(float));  
                
                buffer[nbChanels * width * y + nbChanels * x + j] = chanelValue; 
            } 
        }

        // go to next line
        rf.get(c);
    }

    rf.close();

    if(!rf.good()) {
        std::cout << "Error occurred at writing time!" << std::endl;
    }

    return std::make_tuple(width, height, nbChanels, buffer);
}

std::string rawls::getCommentsRAWLS(std::string filename){

    std::string comments;

    // only one read buffer used for the whole function
    std::ifstream rf(filename, std::ios::out | std::ios::binary);

    if(!rf) {
      std::cout << "Cannot open file!" << std::endl;
    }

    std::string line; 
    unsigned nbChanels, width, height;
    char c; // to get space of new line char

    // READ IHDR info
    bool commentsBegin = false;

    while (!commentsBegin && std::getline(rf, line)) { 

        if (line.find(std::string("COMMENTS")) != std::string::npos){
            commentsBegin = true;
        }
    }

    bool dataBegin = false;

    // READ DATA info
    // case of data chunck begin
    while (!dataBegin && std::getline(rf, line)) { 

        if (line.find(std::string("DATA")) != std::string::npos){
            dataBegin = true;
        }
        else
        {
            comments = comments + line + '\n';
        }
    }

    return comments;
}