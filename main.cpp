#include "opencv2/opencv.hpp"
#include <random>
#include "include/HealthBarAPI.h"
#include <stdint.h>

using namespace std;
using namespace cv;

#define SUBSECTIONSIZE 100
//#define DEBUG

Vec3b getLargestKMean(Mat section) {

    Mat samples;
    section.convertTo(samples, CV_32F);
    samples = samples.reshape(1, samples.total());

    Mat center;
    Mat labels;
    int attempts = 5;

    kmeans(samples, 1, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,
                                            10000, 0.0001), attempts, KMEANS_PP_CENTERS, center);

    Vec3b color;
    int cluster_idx = labels.at<int>(0, 0);
    color[0] = center.at<float>(cluster_idx, 0);
    color[1] = center.at<float>(cluster_idx, 1);
    color[2] = center.at<float>(cluster_idx, 2);

    return color;
}

void
analyzeMovieFrames(VideoCapture &cap, uint32_t frameInterval, Mat &frameData,
                   unordered_map<uint32_t, Mat> &frameCache) {

    int totalNumberFrames = cap.get(CV_CAP_PROP_FRAME_COUNT);
    uint32_t numFrames = totalNumberFrames / frameInterval;
    frameData = Mat(numFrames, 3, CV_32F);

    for (int currFrame = 0; currFrame < numFrames; currFrame++) {
        setBarPositionBottom(false);
        writeStatusBar((double) currFrame, (double) numFrames, "Analyzing movie frames");

        cap.set(CAP_PROP_POS_FRAMES, currFrame * frameInterval);

        Mat frame;
        Mat smaller;

        cap >> frame;
        resize(frame, smaller, Size(SUBSECTIONSIZE, SUBSECTIONSIZE));

        Vec3b color = getLargestKMean(smaller);
        frameData.at<float>(currFrame, 0) = color[0];
        frameData.at<float>(currFrame, 1) = color[1];
        frameData.at<float>(currFrame, 2) = color[2];
        frameCache[currFrame] = smaller;
    }

    return;
}

int
main(int argc, char *argv[]) {

    // Load the video
    VideoCapture cap("./test_files/movies/Totoro.mp4");

    // If the video could not be loaded.
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file.\n" << endl;
        return -1;
    }

    Mat frameData;
    uint32_t frameInterval = 20;
    unordered_map<uint32_t, Mat> frameCache;

    analyzeMovieFrames(cap, frameInterval, frameData, frameCache);

    const int width = 12000;
    const int height = 18000;

    Mat poster = imread("./test_files/posters/test_poster.jpg", CV_LOAD_IMAGE_COLOR);
    resize(poster, poster, Size(width, height));

    flann::KDTreeIndexParams indexParams;
    flann::Index kdtree(Mat(frameData).reshape(1), indexParams);

#ifdef DEBUG
    namedWindow( "Frame", WINDOW_AUTOSIZE );
    namedWindow( "Mean_Color", WINDOW_AUTOSIZE );
    namedWindow( "b", WINDOW_AUTOSIZE );
    namedWindow( "c", WINDOW_AUTOSIZE );
#endif

    for (int y = 0; y < height; y += SUBSECTIONSIZE) {
        for (int x = 0; x < width; x += SUBSECTIONSIZE) {
            setBarPositionBottom(false);
            writeStatusBar((double) y, (double) height, "Replacing movie frames");

            Mat subsection = poster(Range(y, y + SUBSECTIONSIZE - 1), Range(x, x + SUBSECTIONSIZE - 1)).clone();
            Vec3b color = getLargestKMean(subsection);

            vector<int> indices;
            vector<float> dists;
            vector<float> query;
            query.push_back(color[0]);
            query.push_back(color[1]);
            query.push_back(color[2]);

            kdtree.knnSearch(query, indices, dists, 10);

            Mat closestFrame = frameCache[indices[rand() % 10]];

            closestFrame.copyTo(poster(Rect(x, y, closestFrame.cols, closestFrame.rows)));

#ifdef DEBUG
            Mat temp_frame = subsection.clone();
            temp_frame.setTo(Scalar(color[0], color[1], color[2]));
            color = frameData.at<Vec3b>(indices[0]);
            Mat temp2_frame = subsection.clone();
            temp2_frame.setTo(Scalar(color[0], color[1], color[2]));
            imshow( "Frame", subsection);
            imshow( "Mean_Color", temp_frame);
            imshow( "b", closestFrame);
            imshow( "c", temp2_frame);
            waitKey(100);
#endif
        }
    }

    imwrite("./test_files/movies/output.jpg", poster);
    cout << "done" << endl;

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
