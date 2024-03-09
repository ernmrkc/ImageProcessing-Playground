#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class ImageProcessor
{
public:

	/********************************/
	/*    Enumeration Definition    */
	/********************************/

	enum ConvolutionMode
	{
		Blur,
		Sharpen
	};

	enum SobelEdgeDetectionMode
	{
		Horizontal_X,
		Vertical_Y,
		Both_XY
	};

	enum MorphologicalOperationMode
	{
		Dilation,
		Erosion,
		Edge
	};

	/*************************/
	/*    Mask Definition    */
	/*************************/

	float kernel_blur[9] =
	{
		0.0f,   0.125,  0.0f,
		0.125f, 0.5f,   0.125f,
		0.0f,   0.125f, 0.0f,
	};

	float kernel_sharpen[9] =
	{
		0.0f,  -1.0f,  0.0f,
		-1.0f,  5.0f, -1.0f,
		0.0f,  -1.0f,  0.0f,
	};

	float kernel_sobel_v[9] =
	{
		-1.0f, 0.0f, +1.0f,
		-2.0f, 0.0f, +2.0f,
		-1.0f, 0.0f, +1.0f,
	};

	float kernel_sobel_h[9] =
	{
		-1.0f, -2.0f, -1.0f,
		 0.0f, 0.0f, 0.0f,
		+1.0f, +2.0f, +1.0f,
	};

	ImageProcessor();
	float getPixel(float *frame, int nFrameWidth, int nFrameHeight, int x, int y);
	void setPixel(float *frame, int nFrameWidth, int nFrameHeight, int x, int y, float value);
	void performThreshold(float *frame, int nFrameWidth, int nFrameHeight, float fThresholdValue);
	void performConvolution(float *frame, int nFrameWidth, int nFrameHeight, ConvolutionMode mode);
	void performSobelEdgeDetection(float *frame, int nFrameWidth, int nFrameHeight, SobelEdgeDetectionMode mode);
	void performMorphologicalOperation(float *frame, int nFrameWidth, int nFrameHeight, MorphologicalOperationMode mode, float nMorphCount, float fThresholdValue);
	void performMedianFiltering(float *frame, int nFrameWidth, int nFrameHeight);
	void performLocallyAdaptiveThreshold(float *frame, int nFrameWidth, int nFrameHeight, float fAdaptiveBias);
};

#endif // !IMAGEPROCESSOR_H