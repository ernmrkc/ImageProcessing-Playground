#include <iostream>
#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"

using namespace std;
using namespace cv;

int main()
{
	ImageProcessor processor;

	Mat img = imread("Data/Photovoltaic-System.jpg");
	Mat grayImg;
	cvtColor(img, grayImg, COLOR_BGR2GRAY);

	Mat opencvImg32FC1;											// will be used for opencv operations
	Mat bitwiseImg32FC1;											// will be used for bitwise processing
	grayImg.convertTo(opencvImg32FC1, CV_32FC1, 1.0 / 255.0);
	grayImg.convertTo(bitwiseImg32FC1, CV_32FC1, 1.0 / 255.0);

	int nFrameWidth = opencvImg32FC1.cols;
	int nFrameHeight = opencvImg32FC1.rows;
	int nChannels = opencvImg32FC1.channels();

	float *bitwiseImg32FC1_data = reinterpret_cast<float *>(bitwiseImg32FC1.data);


	// Bitwise Threshold Operations
	auto start = chrono::high_resolution_clock::now();
	processor.performThreshold(bitwiseImg32FC1_data, nFrameWidth, nFrameHeight, 0.3);
	auto stop = chrono::high_resolution_clock::now();
	auto duration_bitwise = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for bitwise operations: " << duration_bitwise.count() << " nanosecond" << endl;
	imshow("bitwiseImg32FC1 Image", bitwiseImg32FC1);

	// OpenCV Threshold Operations
	start = chrono::high_resolution_clock::now();
	double thresh = 0.3;
	double maxVal = 1.0;
	threshold(opencvImg32FC1, opencvImg32FC1, thresh, maxVal, THRESH_BINARY);
	stop = chrono::high_resolution_clock::now();
	auto duration_opencv = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for OpenCV operations: " << duration_opencv.count() << " nanosecond" << endl;
	imshow("opencvImg32FC1 Image", opencvImg32FC1);

	imshow("Original Image", img);
	

	/*

	// Bitwise Convolution Operations (Blur)
	auto start = chrono::high_resolution_clock::now();
	processor.performConvolution(bitwiseImg32FC1_data, nFrameWidth, nFrameHeight, processor.Blur);
	auto stop = chrono::high_resolution_clock::now();
	auto duration_bitwise = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for bitwise operations: " << duration_bitwise.count() << " nanosecond" << endl;
	imshow("bitwiseImg32FC1 Image", bitwiseImg32FC1);

	// OpenCV Convolution Operations (Blur)
	start = chrono::high_resolution_clock::now();
	Mat kernel = (Mat_<float>(3, 3) <<
					  0.0f, 0.125, 0.0f,
					  0.125f, 0.5f, 0.125f,
					  0.0f, 0.125f, 0.0f);
	filter2D(opencvImg32FC1, opencvImg32FC1, -1, kernel);
	stop = chrono::high_resolution_clock::now();
	auto duration_opencv = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for OpenCV operations: " << duration_opencv.count() << " nanosecond" << endl;
	imshow("opencvImg32FC1 Image", opencvImg32FC1);
	
	imshow("Original Image", img);
	*/

	/*
	
	// Bitwise Convolution Operations (Sharpen)
	auto start = chrono::high_resolution_clock::now();
	processor.performConvolution(bitwiseImg32FC1_data, nFrameWidth, nFrameHeight, processor.Sharpen);
	auto stop = chrono::high_resolution_clock::now();
	auto duration_bitwise = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for bitwise operations: " << duration_bitwise.count() << " nanosecond" << endl;
	imshow("bitwiseImg32FC1 Image", bitwiseImg32FC1);

	// OpenCV Convolution Operations (Sharpen)
	start = chrono::high_resolution_clock::now();
	Mat kernel = (Mat_<float>(3, 3) <<
				  0.0f, -1.0f, 0.0f,
				  -1.0f, 5.0f, -1.0f,
				  0.0f, -1.0f, 0.0f);
	filter2D(opencvImg32FC1, opencvImg32FC1, -1, kernel);
	stop = chrono::high_resolution_clock::now();
	auto duration_opencv = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for OpenCV operations: " << duration_opencv.count() << " nanosecond" << endl;
	imshow("opencvImg32FC1 Image", opencvImg32FC1);

	imshow("Original Image", img);
	*/

	/*

	// Bitwise Filtering Operations (Median Filtering)
	auto start = chrono::high_resolution_clock::now();
	processor.performMedianFiltering(bitwiseImg32FC1_data, nFrameWidth, nFrameHeight);
	auto stop = chrono::high_resolution_clock::now();
	auto duration_bitwise = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for bitwise operations: " << duration_bitwise.count() << " nanosecond" << endl;
	imshow("bitwiseImg32FC1 Image", bitwiseImg32FC1);

	// OpenCV Filtering Operations (Median Filtering)
	start = chrono::high_resolution_clock::now();
	int kernelSize = 5;
	medianBlur(opencvImg32FC1, opencvImg32FC1, kernelSize);
	stop = chrono::high_resolution_clock::now();
	auto duration_opencv = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for OpenCV operations: " << duration_opencv.count() << " nanosecond" << endl;
	imshow("opencvImg32FC1 Image", opencvImg32FC1);

	imshow("Original Image", img);
	*/

	/*
	
	// Bitwise Locally Adaptive Threshold Operation
	auto start = chrono::high_resolution_clock::now();
	processor.performLocallyAdaptiveThreshold(bitwiseImg32FC1_data, nFrameWidth, nFrameHeight, 0.3);
	auto stop = chrono::high_resolution_clock::now();
	auto duration_bitwise = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for bitwise operations: " << duration_bitwise.count() << " nanosecond" << endl;
	imshow("bitwiseImg32FC1 Image", bitwiseImg32FC1);

	imshow("Original Image", img);
	*/

	/*

	// Bitwise Morphological Operations (Dilation)
	auto start = chrono::high_resolution_clock::now();
	processor.performMorphologicalOperation(bitwiseImg32FC1_data, nFrameWidth, nFrameHeight, processor.Dilation, 3.0f, 0.35f);
	auto stop = chrono::high_resolution_clock::now();
	auto duration_bitwise = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for bitwise operations: " << duration_bitwise.count() << " nanosecond" << endl;
	imshow("bitwiseImg32FC1 Image", bitwiseImg32FC1);

	imshow("Original Image", img);
	*/

	/*

	// Bitwise Morphological Operations (Erosion)
	auto start = chrono::high_resolution_clock::now();
	processor.performMorphologicalOperation(bitwiseImg32FC1_data, nFrameWidth, nFrameHeight, processor.Erosion, 1.0f, 0.35f);
	auto stop = chrono::high_resolution_clock::now();
	auto duration_bitwise = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for bitwise operations: " << duration_bitwise.count() << " nanosecond" << endl;

	imshow("Original Image", img);
	*/

	/*

	// Bitwise Morphological Operations (Edge)
	auto start = chrono::high_resolution_clock::now();
	processor.performMorphologicalOperation(bitwiseImg32FC1_data, nFrameWidth, nFrameHeight, processor.Edge, 1.0f, 0.35f);
	auto stop = chrono::high_resolution_clock::now();
	auto duration_bitwise = chrono::duration_cast<chrono::nanoseconds>(stop - start);
	cout << "Time taken for bitwise operations: " << duration_bitwise.count() << " nanosecond" << endl;

	imshow("Original Image", img);
	*/


	waitKey(0);
}

