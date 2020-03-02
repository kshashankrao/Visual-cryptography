
/********************************************************************************************/
/* User : Karim Hammoudi*/
/* Source : main.cpp */
/* Description : exemples OpenCV  */
/* Comments : VS2015, OPENCV 2.4.9*/
/********************************************************************************************/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{

	Mat imgcolor;
	imgcolor = imread("0791.PNG", CV_LOAD_IMAGE_COLOR);
	Mat image_grey;
	cvtColor(imgcolor, image_grey, CV_BGR2GRAY);

		Mat imgBin;
		cv::threshold(image_grey, imgBin, 120, 255, CV_THRESH_BINARY);

		namedWindow("image couleur", WINDOW_AUTOSIZE);
		namedWindow("image binaire", WINDOW_AUTOSIZE);
		imshow("image couleur", imgcolor);
		imshow("image binaire", imgBin);

		Mat imgShare1 (imgBin.rows, imgBin.cols*2, CV_8UC1);
		Mat imgShare2 (imgBin.rows, imgBin.cols*2, CV_8UC1);

		int AI = 1;
		for (int j = 0; j < imgBin.rows; j++)
		{
			for (int i = 0; i < imgBin.cols; i++)
			{
				//AI = abs(1 - AI);
				AI=rand() % 2;
				//cout << "val : " << AI << endl;
				//cout << "val : " << AI << endl;
				int intensity = imgBin.at<uchar>(j, i);
				//cout << "intensity : " << intensity << endl;
				if (intensity == 255)//blanc
				{
					if (AI == 1)
					{
						imgShare1.at<uchar>(j, i * 2) = 0;
						imgShare1.at<uchar>(j, i * 2 + 1) = 255;

						imgShare2.at<uchar>(j, i * 2) = 0;
						imgShare2.at<uchar>(j, i * 2 + 1) = 255;
					}
					if (AI == 0)
					{
						imgShare1.at<uchar>(j, i * 2) = 255;
						imgShare1.at<uchar>(j, i * 2 + 1) = 0;

						imgShare2.at<uchar>(j, i * 2) = 255;
						imgShare2.at<uchar>(j, i * 2 + 1) = 0;
					}

				}

				if (intensity == 0)//noir
				{
					if (AI == 1)
					{
						imgShare1.at<uchar>(j, i * 2) = 0;
						imgShare1.at<uchar>(j, i * 2 + 1) = 255;

						imgShare2.at<uchar>(j, i * 2) = 255;
						imgShare2.at<uchar>(j, i * 2 + 1) = 0;
					}
					if (AI == 0)
					{
						imgShare1.at<uchar>(j, i * 2) = 255;
						imgShare1.at<uchar>(j, i * 2 + 1) = 0;

						imgShare2.at<uchar>(j, i * 2) = 0;
						imgShare2.at<uchar>(j, i * 2 + 1) = 255;
					}
				}



			}
		}
		Mat final;

		final = imgShare1 + imgShare2;
		namedWindow("share1", WINDOW_AUTOSIZE);
		namedWindow("share2", WINDOW_AUTOSIZE);
		namedWindow("Final", WINDOW_AUTOSIZE);
		imshow("share1", imgShare1);
		imshow("share2", imgShare2);
		imshow("Final", final);
		imwrite("imgShare1.png", imgShare1);
		imwrite("imgShare2.png", imgShare2);
		waitKey(0);                                          // Wait for a keystroke in the window
		return 0;
}
