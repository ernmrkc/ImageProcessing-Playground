#include "ImageProcessor.h"

ImageProcessor::ImageProcessor()
{
}

float ImageProcessor::getPixel(float *frame, int nFrameWidth, int nFrameHeight, int x, int y)
{
	if(x >= 0 && x < nFrameWidth && y >= 0 && y < nFrameHeight)
	{
		return frame[y * nFrameWidth + x];
	}
	else
		return 0.0f;
}

void ImageProcessor::setPixel(float *frame, int nFrameWidth, int nFrameHeight, int x, int y, float value)
{
	if(x >= 0 && x < nFrameWidth && y >= 0 && y < nFrameHeight)
	{
		frame[y * nFrameWidth + x] = value;
	}
}

void ImageProcessor::performThreshold(float *frame, int nFrameWidth, int nFrameHeight, float fThresholdValue)
{
	for(int x = 0; x < nFrameWidth; x++)
	{
		for(int y = 0; y < nFrameHeight; y++)
		{
			setPixel(frame, nFrameWidth, nFrameHeight, x, y, getPixel(frame, nFrameWidth, nFrameHeight, x, y) >= fThresholdValue ? 1.0f : 0.0f);
		}
	}
}

void ImageProcessor::performConvolution(float *frame, int nFrameWidth, int nFrameHeight, ConvolutionMode mode)
{
	float *pConvoKernel = nullptr;
	float *frameCopy = new float[nFrameWidth * nFrameHeight];
	memcpy(frameCopy, frame, nFrameWidth * nFrameHeight * sizeof(float));

	if(mode == Blur)
		pConvoKernel = kernel_blur;
	else
		pConvoKernel = kernel_sharpen;

	for(int x = 0; x < nFrameWidth; x++)
	{
		for(int y = 0; y < nFrameHeight; y++)
		{
			float fSum = 0.0f;
			for(int n = -1; n < +2; n++)
			{
				for(int m = -1; m < +2; m++)
				{
					fSum += getPixel(frameCopy, nFrameWidth, nFrameHeight, x + n, y + m) * pConvoKernel[(m + 1) * 3 + (n + 1)];
				}
			}
			setPixel(frame, nFrameWidth, nFrameHeight, x, y, fSum);
		}
	}

	delete[] frameCopy;
}

void ImageProcessor::performSobelEdgeDetection(float *frame, int nFrameWidth, int nFrameHeight, SobelEdgeDetectionMode mode)
{
	float *frameCopy = new float[nFrameWidth * nFrameHeight];
	memcpy(frameCopy, frame, nFrameWidth * nFrameHeight * sizeof(float));

	if(mode == Horizontal_X)
	{
		for(int x = 0; x < nFrameWidth; x++)
		{
			for(int y = 0; y < nFrameHeight; y++)
			{
				float fKernelSumH = 0.0f;
				for(int n = -1; n < +2; n++)
				{
					for(int m = -1; m < +2; m++)
					{
						fKernelSumH += getPixel(frameCopy, nFrameWidth, nFrameHeight, x + n, y + m) * kernel_sobel_h[(m + 1) * 3 + (n + 1)];
					}
				}
				setPixel(frame, nFrameWidth, nFrameHeight, x, y, fKernelSumH);
			}
		}
	}
	else if(mode == Vertical_Y)
	{
		for(int x = 0; x < nFrameWidth; x++)
		{
			for(int y = 0; y < nFrameHeight; y++)
			{
				float fKernelSumV = 0.0f;
				for(int n = -1; n < +2; n++)
				{
					for(int m = -1; m < +2; m++)
					{
						fKernelSumV += getPixel(frameCopy, nFrameWidth, nFrameHeight, x + n, y + m) * kernel_sobel_v[(m + 1) * 3 + (n + 1)];
					}
				}
				setPixel(frame, nFrameWidth, nFrameHeight, x, y, fKernelSumV);
			}
		}
	}
	else
	{
		for(int x = 0; x < nFrameWidth; x++)
		{
			for(int y = 0; y < nFrameHeight; y++)
			{
				float fKernelSumH = 0.0f;
				float fKernelSumV = 0.0f;
				for(int n = -1; n < +2; n++)
				{
					for(int m = -1; m < +2; m++)
					{
						fKernelSumH += getPixel(frameCopy, nFrameWidth, nFrameHeight, x + n, y + m) * kernel_sobel_h[(m + 1) * 3 + (n + 1)];
						fKernelSumV += getPixel(frameCopy, nFrameWidth, nFrameHeight, x + n, y + m) * kernel_sobel_v[(m + 1) * 3 + (n + 1)];
					}
				}
				setPixel(frame, nFrameWidth, nFrameHeight, x, y, fabs((fKernelSumH + fKernelSumV) / 2.0f));
			}
		}
	}
}

void ImageProcessor::performMorphologicalOperation(float *frame, int nFrameWidth, int nFrameHeight, MorphologicalOperationMode mode, float nMorphCount, float fThresholdValue)
{
	if(nMorphCount > 10.0f) nMorphCount = 10.0f;
	if(nMorphCount < 1.0f) nMorphCount = 1.0f;

	float *activity = new float[nFrameWidth * nFrameHeight];
	memcpy(activity, frame, nFrameWidth * nFrameHeight * sizeof(float));

	performThreshold(activity, nFrameWidth, nFrameHeight, fThresholdValue);

	if(mode == Dilation)
	{
		for(int n = 0; n < nMorphCount; n++)
		{
			memcpy(frame, activity, nFrameWidth * nFrameHeight * sizeof(float));
			for(int x = 0; x < nFrameWidth; x++)
			{
				for(int y = 0; y < nFrameHeight; y++)
				{
					if(getPixel(activity, nFrameWidth, nFrameHeight, x, y) == 1.0f)
					{
						setPixel(frame, nFrameWidth, nFrameHeight, x, y, 1.0f);
						setPixel(frame, nFrameWidth, nFrameHeight, x - 1, y, 1.0f);
						setPixel(frame, nFrameWidth, nFrameHeight, x + 1, y, 1.0f);
						setPixel(frame, nFrameWidth, nFrameHeight, x, y - 1, 1.0f);
						setPixel(frame, nFrameWidth, nFrameHeight, x, y + 1, 1.0f);
						setPixel(frame, nFrameWidth, nFrameHeight, x - 1, y - 1, 1.0f);
						setPixel(frame, nFrameWidth, nFrameHeight, x + 1, y + 1, 1.0f);
						setPixel(frame, nFrameWidth, nFrameHeight, x + 1, y - 1, 1.0f);
						setPixel(frame, nFrameWidth, nFrameHeight, x - 1, y + 1, 1.0f);
					}
				}
			}
			memcpy(activity, frame, nFrameWidth * nFrameHeight * sizeof(float));
		}
	}
	else if(mode == Erosion)
	{
		for(int n = 0; n < nMorphCount; n++)
		{
			memcpy(frame, activity, nFrameWidth * nFrameHeight * sizeof(float));
			for(int x = 0; x < nFrameWidth; x++)
			{
				for(int y = 0; y < nFrameHeight; y++)
				{
					float sum = getPixel(activity, nFrameWidth, nFrameHeight, x - 1, y) + getPixel(activity, nFrameWidth, nFrameHeight, x + 1, y) +
						getPixel(activity, nFrameWidth, nFrameHeight, x, y - 1) + getPixel(activity, nFrameWidth, nFrameHeight, x, y + 1) +
						getPixel(activity, nFrameWidth, nFrameHeight, x - 1, y - 1) + getPixel(activity, nFrameWidth, nFrameHeight, x + 1, y + 1) +
						getPixel(activity, nFrameWidth, nFrameHeight, x + 1, y - 1) + getPixel(activity, nFrameWidth, nFrameHeight, x - 1, y + 1);

					if(getPixel(activity, nFrameWidth, nFrameHeight, x, y) == 1.0f && sum < 8.0f)
					{
						setPixel(frame, nFrameWidth, nFrameHeight, x, y, 0.0f);
					}
				}
			}
			memcpy(activity, frame, nFrameWidth * nFrameHeight * sizeof(float));
		}
	}
	else
	{
		memcpy(frame, activity, nFrameWidth * nFrameHeight * sizeof(float));
		for(int x = 0; x < nFrameWidth; x++)
		{
			for(int y = 0; y < nFrameHeight; y++)
			{
				float sum = getPixel(activity, nFrameWidth, nFrameHeight, x - 1, y) + getPixel(activity, nFrameWidth, nFrameHeight, x + 1, y) +
					getPixel(activity, nFrameWidth, nFrameHeight, x, y - 1) + getPixel(activity, nFrameWidth, nFrameHeight, x, y + 1) +
					getPixel(activity, nFrameWidth, nFrameHeight, x - 1, y - 1) + getPixel(activity, nFrameWidth, nFrameHeight, x + 1, y + 1) +
					getPixel(activity, nFrameWidth, nFrameHeight, x + 1, y - 1) + getPixel(activity, nFrameWidth, nFrameHeight, x - 1, y + 1);

				if(getPixel(activity, nFrameWidth, nFrameHeight, x, y) == 1.0f && sum < 8.0f)
				{
					setPixel(frame, nFrameWidth, nFrameHeight, x, y, 0.0f);
				}
			}
		}
	}

	delete[] activity;
}

void ImageProcessor::performMedianFiltering(float *frame, int nFrameWidth, int nFrameHeight)
{
	for(int x = 0; x < nFrameWidth; x++)
	{
		for(int y = 0; y < nFrameHeight; y++)
		{
			vector<float> v;
			for(int n = -2; n < +3; n++)
			{
				for(int m = -2; m < +3; m++)
				{
					v.push_back(getPixel(frame, nFrameWidth, nFrameHeight, x + n, y + m));
				}
			}
			sort(v.begin(), v.end(), greater<float>());
			setPixel(frame, nFrameWidth, nFrameHeight, x, y, v[12]);
		}
	}
}

void ImageProcessor::performLocallyAdaptiveThreshold(float *frame, int nFrameWidth, int nFrameHeight, float fAdaptiveBias)
{
	if(fAdaptiveBias > 1.5f) fAdaptiveBias = 1.5f;
	if(fAdaptiveBias < 0.5f) fAdaptiveBias = 0.5f;

	for(int x = 0; x < nFrameWidth; x++)
	{
		for(int y = 0; y < nFrameHeight; y++)
		{
			float fRegionSum = 0.0f;
			for(int n = -2; n < +3; n++)
			{
				for(int m = -2; m < +3; m++)
				{
					fRegionSum += getPixel(frame, nFrameWidth, nFrameHeight, x + n, y + m);
				}
			}
			fRegionSum /= 25.0f;
			setPixel(frame, nFrameWidth, nFrameHeight, x, y, getPixel(frame, nFrameWidth, nFrameHeight, x, y) > (fRegionSum * fAdaptiveBias) ? 1.0f : 0.0f);
		}
	}
}
