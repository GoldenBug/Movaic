#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

#define FRAMEINTERVAL 8

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

    Mat frameData(1000,3,CV_32F);
    //Mat frameFreq(1000,3,CV_32SC3);

    for(int frameNum = 0; frameNum < 2000; ++frameNum) {

        Mat frame;
        // Capture frame-by-frame
        cap.set(CAP_PROP_POS_FRAMES, frameNum * FRAMEINTERVAL);
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        Mat smaller;
        resize(frame, smaller, Size(400,400));

        /* Mat top_left = smaller(Range(0, smaller.rows / 2 - 1),
                                 Range(0, smaller.cols / 2 - 1));
          Mat top_right = smaller(Range(0, smaller.rows / 2 - 1),
                                  Range(smaller.cols / 2, smaller.cols - 1));
          Mat bottom_left = smaller(Range(smaller.rows / 2, smaller.rows - 1),
                                    Range(0, smaller.cols / 2 - 1));
          Mat bottom_right = smaller(Range(smaller.rows / 2, smaller.rows - 1),
                                     Range(smaller.cols / 2, smaller.cols - 1));
        */
        Vec3b color;

        /*color = getLargestKMean(top_left);
          Mat top_left_mean(200,200, smaller.type(),Scalar(color[0], color[1], color[2]));

        color = getLargestKMean(top_right);
          Mat top_right_mean(200,200, smaller.type(),Scalar(color[0], color[1], color[2]));

        color = getLargestKMean(bottom_left);
          Mat bottom_left_mean(200,200, smaller.type(),Scalar(color[0], color[1], color[2]));

        color = getLargestKMean(bottom_right);
          Mat bottom_right_mean(200,200, smaller.type(),Scalar(color[0], color[1], color[2]));
        */

        color = getLargestKMean(smaller);

        frameData.at<float>(frameNum, 0) = color[0];
        frameData.at<float>(frameNum, 1) = color[1];
        frameData.at<float>(frameNum, 2) = color[2];

        //if(frameNum % 100 == 0)
        cout << frameNum << endl;
        // Display the resulting frame
        /*        imshow( "Frame1", smaller);
                imshow( "Top Left", top_left_mean);
                imshow( "Top Rigt", top_right_mean);
                imshow( "Bottom Left", bottom_left_mean);
                imshow( "Botom Right", bottom_right_mean);
        */
        // Press  ESC on keyboard to exit
//        char c=(char)waitKey(25);
//        if(c==27)
//            break;
    }

	int width = 6000;
	int height = 9000;

    Mat poster = imread("../movie/porcoRossoPoster.jpg", CV_LOAD_IMAGE_COLOR);
    resize(poster, poster, Size(width,height));

	int subSectionSize = 100;
    for(int y = 0; y < height; y += subSectionSize) {
        for(int x = 0; x < width; x += subSectionSize) {
            Mat subsection = poster(Range(y, y+subSectionSize-1), Range(x, x+subSectionSize-1));
            Vec3b color = getLargestKMean(subsection);

            flann::KDTreeIndexParams indexParams;
            flann::Index kdtree(Mat(frameData).reshape(1), indexParams);
            vector<float> query;
            query.push_back(color[0]);
            query.push_back(color[1]);
            query.push_back(color[2]);
            vector<int> indices;
            vector<float> dists;
            kdtree.knnSearch(query, indices, dists, 10);

            Mat closestFrame;
            cap.set(CAP_PROP_POS_FRAMES, indices[rand() % 10] * FRAMEINTERVAL);
            cap >> closestFrame;
            resize(closestFrame, closestFrame, Size(subSectionSize,subSectionSize));

            for(int y_frame = 0; y_frame < subSectionSize; ++y_frame) {
                for(int x_frame = 0; x_frame < subSectionSize; ++x_frame) {
                    poster.at<Vec3b>(y+y_frame,x+x_frame)[0] = closestFrame.at<Vec3b>(y_frame,x_frame)[0];
                    poster.at<Vec3b>(y+y_frame,x+x_frame)[1] = closestFrame.at<Vec3b>(y_frame,x_frame)[1];
                    poster.at<Vec3b>(y+y_frame,x+x_frame)[2] = closestFrame.at<Vec3b>(y_frame,x_frame)[2];
                }
            }

            //imshow("Closest Frame", closestFrame);
            //imshow("Subsection", subsection);
			//imshow("Poster", poster);
		
			if(y % 100 == 0 && x % 100 == 0)
					cout << y << " " << x << endl;
            //char c=(char)waitKey(10);
            //if(c==27)
            //    break;
        }
    }

	imwrite("../movie/movaic.jpg", poster);
    cout << "done" << endl;
    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
