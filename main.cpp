#include "opencv2/opencv.hpp"
#include <iostream>
#include <unordered_map>
#include <stdint.h>

using namespace std;
using namespace cv;

#define FRAMEINTERVAL 4

Vec3b getLargestKMean(Mat section) {

    Vec3b color;
    Mat samples(section.rows * section.cols, 3, CV_32F);

    for( int y = 0; y < section.rows; y++ ) {
        for( int x = 0; x < section.cols; x++ ) {
            for( int z = 0; z < 3; z++) {
                samples.at<float>(y + x*section.rows, z) = section.at<Vec3b>(y,x)[z];
            }
        }
    }

    Mat center;
    Mat labels;
    int clusterCount = 1;
    int attempts = 5;

    kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,
            10000, 0.0001), attempts, KMEANS_PP_CENTERS, center);

    int cluster_idx = labels.at<int>(0,0);
    color[0] = center.at<float>(cluster_idx, 0);
    color[1] = center.at<float>(cluster_idx, 1);
    color[2] = center.at<float>(cluster_idx, 2);

    return color;
}

int main() {

    // Load the video
    VideoCapture cap("../movie/gatsby.mp4");

    // If the vido could not be loaded.
    if(!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    Mat frameData(1770,3,CV_32F);

    for(int frameNum = 0; frameNum < 2000; ++frameNum) {

        Mat frame;
        cap.set(CAP_PROP_POS_FRAMES, frameNum * FRAMEINTERVAL);
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        Mat smaller;
        resize(frame, smaller, Size(400,400));

        Vec3b color;

        color = getLargestKMean(smaller);

        frameData.at<float>(frameNum, 0) = color[0];
        frameData.at<float>(frameNum, 1) = color[1];
        frameData.at<float>(frameNum, 2) = color[2];

    }

    const int width = 12000;
    const int height = 18000;

    Mat poster = imread("../movie/gatsbyPoster2.jpg", CV_LOAD_IMAGE_COLOR);
    resize(poster, poster, Size(width,height));

    int subSectionSize = 100;
    unordered_map<uint32_t, vector<int>> frameMap;
    unordered_map<uint32_t, Mat> frameCache;
    for(int y = 0; y < height; y += subSectionSize) {
        for(int x = 0; x < width; x += subSectionSize) {

            Mat subsection = poster(Range(y, y+subSectionSize-1), Range(x, x+subSectionSize-1));
            Vec3b color = getLargestKMean(subsection);

            vector<int> indices;

            uint32_t key = (color[0] << 16) + (color[1] << 8) + (color[2]);

            if(frameMap.find(key) != frameMap.end()) {
                indices = frameMap[key];
            } else {

                flann::KDTreeIndexParams indexParams;
                flann::Index kdtree(Mat(frameData).reshape(1), indexParams);
                vector<float> query;
                query.push_back(color[0]);
                query.push_back(color[1]);
                query.push_back(color[2]);
                vector<float> dists;
                kdtree.knnSearch(query, indices, dists, 10);
                frameMap[key] = indices;
            }

            Mat closestFrame;
            int chosenFrame = indices[rand() % 10] * FRAMEINTERVAL;

            if(frameCache.find(chosenFrame) != frameCache.end())
                closestFrame = frameCache[chosenFrame];
            else {
                cap.set(CAP_PROP_POS_FRAMES, indices[rand() % 10] * FRAMEINTERVAL);
                cap >> closestFrame;
                frameCache[chosenFrame] = closestFrame;
            }
	
            resize(closestFrame, closestFrame, Size(subSectionSize, subSectionSize));
            closestFrame.copyTo(poster(Rect(x, y, closestFrame.cols, closestFrame.rows)));

        }
    }

    imwrite("../movie/gatsbyPoster2_movaic.jpg", poster);
    cout << "done" << endl;
    
    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
