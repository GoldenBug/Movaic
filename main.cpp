#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

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

        Mat topLeft(top_left.rows * top_left.cols, 3, CV_32F);
        Mat topRight(top_left.rows * top_left.cols, 3, CV_32F);
        Mat bottomLeft(top_left.rows * top_left.cols, 3, CV_32F);
        Mat bottomRight(top_left.rows * top_left.cols, 3, CV_32F);

        for( int y = 0; y < top_left.rows; y++ ) {
            for( int x = 0; x < top_left.cols; x++ ) {
                for( int z = 0; z < 3; z++) {
                    topLeft.at<float>(y + x*top_left.rows, z) = top_left.at<Vec3b>(y,x)[z];
                    topRight.at<float>(y + x*top_right.rows, z) = top_right.at<Vec3b>(y,x)[z];
                    bottomLeft.at<float>(y + x*bottom_left.rows, z) = bottom_left.at<Vec3b>(y,x)[z];
                    bottomRight.at<float>(y + x*bottom_right.rows, z) = bottom_right.at<Vec3b>(y,x)[z];
                }
            }
        }


        Mat center;
        Mat labels;
        Vec3b colors;
        int clusterCount = 1;
        int attempts = 5;

        kmeans(topLeft, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,
                10000, 0.0001), attempts, KMEANS_PP_CENTERS, center);
        int cluster_idx = labels.at<int>(0,0);
        colors[0] = center.at<float>(cluster_idx, 0);
        colors[1] = center.at<float>(cluster_idx, 1);
        colors[2] = center.at<float>(cluster_idx, 2);
        Mat new_image1(200,200, smaller.type(),Scalar(colors[0], colors[1], colors[2]));

        kmeans(topRight, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,
                10000, 0.0001), attempts, KMEANS_PP_CENTERS, center);
        cluster_idx = labels.at<int>(0,0);
        colors[0] = center.at<float>(cluster_idx, 0);
        colors[1] = center.at<float>(cluster_idx, 1);
        colors[2] = center.at<float>(cluster_idx, 2);
        Mat new_image2(200,200, smaller.type(), Scalar(colors[0], colors[1], colors[2]));

        kmeans(bottomLeft, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,
                10000, 0.0001), attempts, KMEANS_PP_CENTERS, center);
        cluster_idx = labels.at<int>(0,0);
        colors[0] = center.at<float>(cluster_idx, 0);
        colors[1] = center.at<float>(cluster_idx, 1);
        colors[2] = center.at<float>(cluster_idx, 2);
        Mat new_image3(200,200, smaller.type(), Scalar(colors[0], colors[1], colors[2]));

        kmeans(bottomRight, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,
                10000, 0.0001), attempts, KMEANS_PP_CENTERS, center);
        cluster_idx = labels.at<int>(0,0);
        colors[0] = center.at<float>(cluster_idx, 0);
        colors[1] = center.at<float>(cluster_idx, 1);
        colors[2] = center.at<float>(cluster_idx, 2);
        Mat new_image4(200,200, smaller.type(), Scalar(colors[0], colors[1], colors[2]));
        // Display the resulting frame
        imshow( "Frame1", smaller);
        imshow( "Top Left", new_image1);
        imshow( "Top Rigt", new_image2);
        imshow( "Bottom Left", new_image3);
        imshow( "Botom Right", new_image4);
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
