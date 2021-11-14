
#include <stdio.h>
using namespace std;

#include <iostream>

// OpenCV includes
#include <opencv2/opencv.hpp>
using namespace cv;
Mat grayImage;
int threshold_value = 100;
const int max_value = 255;
Mat binarizedImage;
void Threshold(int pos, void* userdata) {
	threshold(grayImage, binarizedImage, threshold_value, max_value, THRESH_BINARY);
	imshow("Binarized image", binarizedImage);
}
int main()
{
	// reading source file srcImage
	Mat srcImage;
	srcImage = imread( "Samples/ryba.jpg" );
	if ( !srcImage.data )
	{
		cout << "Error! Cannot read source file. Press ENTER.";
		waitKey();
		return( -1 );
	}
	
	
	cvtColor(srcImage, grayImage,COLOR_BGR2GRAY);
	imwrite("Samples/Gray image.jpg", grayImage);
	Mat resizedImage(100,100, srcImage.type());
	resize(srcImage, resizedImage,resizedImage.size());

	namedWindow("Piotr Garbowski");
	moveWindow("Piotr Garbowski", 0, 0);
	imshow("Piotr Garbowski", srcImage);

	namedWindow("Gray image");
	moveWindow("Gray image", 300, 0);
	imshow("Gray image", grayImage);

	namedWindow("Resized image");
	moveWindow("Resized image", 600, 0);
	imshow("Resized image", resizedImage);

	Mat blurImage;
	blur(srcImage, blurImage, Size(5, 5));
	namedWindow("Blur image");
	moveWindow("Blur image", 900, 0);
	imshow("Blur image", blurImage);

	Mat CannyImage;
	Canny(srcImage, CannyImage, 90, 90);
	namedWindow("Canny image");
	moveWindow("Canny image", 1200, 0);
	imshow("Canny image", CannyImage);

	Mat LaplacianImage,scaledLaplacianImage;
	Laplacian(grayImage, LaplacianImage,CV_16S, 3);
	convertScaleAbs(LaplacianImage, scaledLaplacianImage);
	namedWindow("Laplacian image");
	moveWindow("Laplacian image", 1500, 0);
	imshow("Laplacian image", scaledLaplacianImage);

	Mat brightImage;
	srcImage.copyTo(brightImage);
	for (int i = 0; i < brightImage.cols; i++) {
		for (int j = 0; j < brightImage.rows; j++) {
			Vec3b pixelColor;
			pixelColor = brightImage.at<Vec3b>(Point(i, j));
			for (int k = 0; k < 3; k++) {
				if (pixelColor[k] + 100 > 255) pixelColor[k] = 255;
				else pixelColor[k] += 100;
			}
			brightImage.at<Vec3b>(Point(i, j)) = pixelColor;
		}
	}
	namedWindow("Bright image");
	moveWindow("Bright image", 900, 300);
	imshow("Bright image", brightImage);

	namedWindow("Binarization");
	moveWindow("Binarization", 0, 600);
	
	createTrackbar("Threshold value", "Binarization", &threshold_value, max_value, Threshold);
	Threshold(threshold_value, NULL);
	//waitKey();

	namedWindow("Src video");
	moveWindow("Src video", 300, 600);

	namedWindow("Dst video");
	moveWindow("Dst video", 900, 600);

	Mat srcFrame, dstFrame;
	VideoCapture capture("Samples/Dino.avi");
	capture >> srcFrame;
	VideoWriter writer("Samples/Dino2.avi",-1,25,srcFrame.size());
	while (waitKey(25) != 27 && !srcFrame.empty()) {
		blur(srcFrame, dstFrame, Size(5, 5));
		writer << dstFrame;
		imshow("Src video", srcFrame);
		imshow("Dst video", dstFrame);
		capture >> srcFrame;
	}

	const int histSize = 256;
	const int hist_w= 256;
	const int hist_h = 256;
	float range[2] = { 0,256 };
	const float* histRange = range;
	Mat histImageGray(Size(hist_h, hist_w), CV_8UC3, Scalar(0, 0, 0));
	Mat histogramGray;
	calcHist(&grayImage,1,0,Mat(),histogramGray,1,&histSize,&histRange);
	normalize(histogramGray,histogramGray, range[0], range[1],NORM_MINMAX);
	for (int i = 0; i < histSize; i++) {
		line(histImageGray, Point(i, hist_h),Point(i,hist_h - cvRound(histogramGray.at<float>(i))),Scalar(255,0,0),2);
	} 
	namedWindow("Histogram Gray");
	moveWindow("Histogram Gray", 0, 300);
	imshow("Histogram Gray", histImageGray);
	
	Mat equalizeHistImage;
	equalizeHist(grayImage, equalizeHistImage);
	namedWindow("Equalized Histogram Image");
	moveWindow("Equalized Histogram Image", 300, 300);
	imshow("Equalized Histogram Image", equalizeHistImage);
	
	Mat histogramEqualized(Size(hist_h, hist_w), CV_8UC3, Scalar(0, 0, 0));
	Mat hist2;
	calcHist(&equalizeHistImage, 1, 0, Mat(), hist2, 1, &histSize, &histRange);
	normalize(hist2, hist2, range[0], range[1], NORM_MINMAX);
	for (int i = 0; i < histSize; i++) {
		line(histogramEqualized, Point(i, hist_h), Point(i, hist_h - cvRound(hist2.at<float>(i))), Scalar(255, 0, 0), 2);
	}
	namedWindow("Histogram Equalized");
	moveWindow("Histogram Equalized", 600, 300);
	imshow("Histogram Equalized", histogramEqualized);
	waitKey();
}
