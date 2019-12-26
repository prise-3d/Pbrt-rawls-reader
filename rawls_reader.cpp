#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include <bitset>

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
    

    std::cout << "Read image `" << imageName << "` and save it into " << outfileName << std::endl;

    std::ifstream rf(imageName, std::ios::out | std::ios::binary);

    if(!rf) {
      std::cout << "Cannot open file!" << std::endl;
      return 1;
    }


    std::string line; 
    int nbChanels, width, height;
    char c; // to get space of new line char

    // READ IHDR info
    bool ihdrBegin = false;

    while (!ihdrBegin && std::getline(rf, line)) { 

        if (line.find(std::string("IHDR")) != std::string::npos){
            ihdrBegin = true;
            std::getline(rf, line); // avoid data size line

            rf.read((char *) &width, sizeof(int));
            rf.get(c);
            rf.read((char *) &height, sizeof(int));
            rf.get(c);
            rf.read((char *) &nbChanels, sizeof(int));
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

    std::getline(rf, line);

    int size = std::stoi(line);


    // compute number of pixels into image
    int nbPixels = (size / (nbChanels * 4));


    // create outfile 
    // now write image as .ppm
    std::ofstream outfile(outfileName);

    outfile << "P" << nbChanels << std::endl;
    outfile << width << " " << height << std::endl;
    outfile << "255" << std::endl;

    for (int i = 0; i < nbPixels; i++){

        float chanelValue;

        // for each chanel read and keep save float value into new file
        for(int j = 0; j < nbChanels; j++){
            rf.read((char *) &chanelValue, sizeof(float));  
            outfile << int(chanelValue) << " "; 
        } 

    // go to next line
        rf.get(c);

        if ((i + 1) % height == 0){
            outfile << std::endl;
        }
    }

    rf.close();

    if(!rf.good()) {
        std::cout << "Error occurred at reading time!" << std::endl;
        return 1;
    }

    std::cout << "Image is now saved as .ppm into " << outfileName << std::endl;

    outfile.close();
}