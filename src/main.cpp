#include "opencv2/opencv.hpp"
#include <random>
#include <stdint.h>

#include "include/HealthBarAPI.h"
#include "util.h"

using namespace std;
using namespace cv;

#define WIDTH 12000
#define HEIGHT 18000

int
main(int argc, char *argv[]) {

    // Load the video
    VideoCapture cap("./test_files/movies/gatsby.mp4");

    // If the video could not be loaded.
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file.\n" << endl;
        return -1;
    }

    Mat frameData;
    uint32_t frameInterval = 20;
    unordered_map<uint32_t, Mat> frameCache;

    // Find the specified K-means from the source video and cache them.
    analyzeMovieFrames(cap, frameInterval, frameData, frameCache);

    Mat poster = imread("./test_files/posters/gatsby_poster.jpg", CV_LOAD_IMAGE_COLOR);
    resize(poster, poster, Size(WIDTH, HEIGHT));

    substituteFrames(frameData, WIDTH, HEIGHT, poster, frameCache);

    imwrite("./test_files/movies/output.jpg", poster);
    cout << "done" << endl;

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
