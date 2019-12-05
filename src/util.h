#ifndef MOVIAC_UTIL_H
#define MOVIAC_UTIL_H

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Vec3b getLargestKMean(Mat section);

void analyzeMovieFrames(VideoCapture &cap, uint32_t frameInterval, Mat &frameData,
                        unordered_map<uint32_t, Mat> &frameCache);

void substituteFrames(Mat frameData, int width, int height, Mat poster,
                      unordered_map<uint32_t, Mat> &frameCache);

#endif //MOVIAC_UTIL_H
