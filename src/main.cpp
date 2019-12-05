#include <stdint.h>

#include "opencv2/opencv.hpp"
#include "util.h"
#include "commandLineParser.h"

using namespace std;
using namespace cv;

#define WIDTH 12000
#define HEIGHT 18000

int
main(int argc, char *argv[]) {

    CDLArguments parsedArguments;
    try {
        parsedArguments = parseCommands(argc, argv);
    } catch (std::exception &e) {
        exit(EXIT_FAILURE);
    }

    if (parsedArguments.noParse)
        exit(EXIT_SUCCESS);

    // Load the video
    VideoCapture cap(parsedArguments.sourceVideoPath);

    // If the video could not be loaded.
    if (!cap.isOpened()) {
        cout << "ERROR: Can't open video stream.\n" << endl;
        exit(EXIT_FAILURE);
    }

    Mat frameData;
    uint32_t frameInterval = 20;
    unordered_map<uint32_t, Mat> frameCache;

    // Find the specified K-means from the source video and cache them.
    analyzeMovieFrames(cap, frameInterval, frameData, frameCache);

    Mat poster = imread(parsedArguments.sourcePosterPath, CV_LOAD_IMAGE_COLOR);
    if (poster.data == NULL) {
        cout << "Error: Can't open poster file.\n" << endl;
        exit(EXIT_FAILURE);
    }

    resize(poster, poster, Size(WIDTH, HEIGHT));

    // Substitute frames in the source poster with their closest matching frames from the source video.
    substituteFrames(frameData, WIDTH, HEIGHT, poster, frameCache);


    cap.release();

    bool result = false;
    try {
        cout << "Writing poster disk..." << endl;
        result = imwrite(parsedArguments.destinationPosterPath, poster);
    }
    catch (const cv::Exception &ex) {
        cerr << "Exception writing poster to path: " << ex.what() << endl;
    }

    if (!result) {
        cout << "ERROR: Can't save output poster to path: " << parsedArguments.destinationPosterPath << endl;
        exit(EXIT_FAILURE);
    }

    cout << "***FINISHED***" << endl;
    exit(EXIT_SUCCESS);
    return 0;
}
