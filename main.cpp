#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

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
    VideoCapture cap("../movie/Porco.Rosso.mp4");

    // If the vido could not be loaded.
    if(!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while(1) {

        Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        Mat smaller;
        resize(frame, smaller, Size(1200,1200));

        Mat top_left = smaller(Range(0, smaller.rows / 2 - 1),
                               Range(0, smaller.cols / 2 - 1));
        Mat top_right = smaller(Range(0, smaller.rows / 2 - 1),
                                Range(smaller.cols / 2, smaller.cols - 1));
        Mat bottom_left = smaller(Range(smaller.rows / 2, smaller.rows - 1),
                                  Range(0, smaller.cols / 2 - 1));
        Mat bottom_right = smaller(Range(smaller.rows / 2, smaller.rows - 1),
                                   Range(smaller.cols / 2, smaller.cols - 1));

        Vec3b color;

		color = getLargestKMean(top_left);
        Mat top_left_mean(200,200, smaller.type(),Scalar(color[0], color[1], color[2]));

		color = getLargestKMean(top_right);
        Mat top_right_mean(200,200, smaller.type(),Scalar(color[0], color[1], color[2]));
		
		color = getLargestKMean(bottom_left);
        Mat bottom_left_mean(200,200, smaller.type(),Scalar(color[0], color[1], color[2]));
		
		color = getLargestKMean(bottom_right);
        Mat bottom_right_mean(200,200, smaller.type(),Scalar(color[0], color[1], color[2]));


        // Display the resulting frame
        imshow( "Frame1", smaller);
        imshow( "Top Left", top_left_mean);
        imshow( "Top Rigt", top_right_mean);
        imshow( "Bottom Left", bottom_left_mean);
        imshow( "Botom Right", bottom_right_mean);

        // Press  ESC on keyboard to exit
        char c=(char)waitKey(25);
        if(c==27)
            break;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
