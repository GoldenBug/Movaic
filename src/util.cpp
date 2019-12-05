#include "util.h"
#include "include/HealthBarAPI.h"

#define SUBSECTIONSIZE 100

Vec3b
getLargestKMean(Mat section) {

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

    writeStatusBar((double) numFrames, (double) numFrames, "Analyzing movie frames");
    return;
}

void substituteFrames(Mat frameData, int width, int height, Mat poster, unordered_map<uint32_t, Mat> &frameCache) {

    flann::KDTreeIndexParams indexParams;
    flann::Index kdtree(Mat(frameData).reshape(1), indexParams);

    for (int y = 0; y < height; y += SUBSECTIONSIZE) {

        setBarPositionBottom(false);
        writeStatusBar((double) y, (double) height, "Replacing movie frames");

        for (int x = 0; x < width; x += SUBSECTIONSIZE) {

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

        }
    }

    writeStatusBar((double) height, (double) height, "Replacing movie frames");
}
