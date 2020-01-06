#include <string.h>
#include <iostream>
#include <math.h>
#include <tuple>

namespace rawls{

/*
 * Gamma conversion functions
 */
inline float GammaCorrect(float value) {
    if (value <= 0.0031308f) return 12.92f * value;
    return 1.055f * std::pow(value, (float)(1.f / 2.4f)) - 0.055f;
}

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
    if (val < low)
        return low;
    else if (val > high)
        return high;
    else
        return val;
}

inline bool HasExtension(const std::string &value, const std::string &ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(
        ending.rbegin(), ending.rend(), value.rbegin(),
        [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}

//#define GAMMA_CONVERT(v) (float) rawls::Clamp(255.f * rawls::GammaCorrect(v) + 0.5f, 0.f, 255.f)
inline float GammaConvert(float value){
    return rawls::Clamp(255.f * rawls::GammaCorrect(value) + 0.5f, 0.f, 255.f);
}


/*
 * Save and write functions
 */
bool convertToPPM(std::string imageName, std::string outfileName);

bool convertToPNG(std::string imageName, std::string outfileName);

bool saveAsPPM(unsigned width, unsigned height, unsigned nbChanels, float* buffer, std::string outfileName);

bool saveAsPNG(unsigned width, unsigned height, unsigned nbChanels, float* buffer, std::string outfileName);

/*
 * Read `.rawls` image and fill buffer pass as parameter
 *
 * `float*` is pointer to data buffer and needed to be deleted
 */
float* getPixelsRAWLS(std::string filename);

/*
 * Returns tuple with <width, height, channels> information of image
 */
std::tuple<unsigned, unsigned, unsigned> getDimensionsRAWLS(std::string filename);

/*
 * Returns tuple with <width, height, channels, buffer*> information of image
 *
 * `float*` is pointer to data buffer and needed to be deleted
 */
std::tuple<unsigned, unsigned, unsigned, float*> getDataRAWLS(std::string filename);

};