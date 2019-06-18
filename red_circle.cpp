#include "stdafx.h"
#include "red_circle.h"
#include "E:\opencv\opencv\build\include\opencv2\opencv.hpp"
#include "E:\opencv\opencv\build\include\opencv2\core\core.hpp"
#include "E:\opencv\opencv\build\include\opencv2\highgui\highgui.hpp"
#include "E:\opencv\opencv\build\include\opencv2\imgproc\imgproc.hpp"
#include "E:\opencv\opencv\build\include\opencv2\imgproc\imgproc_c.h"
#include <windows.h>
#include <iostream>
#include <atlstr.h>

using namespace cv;
using namespace std;
#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR   lpCmdLine, _In_ int  nCmdShow)
{
	Mat image0 = cv::imread("F:\\green_light.jpg");

	if (image0.empty())
	{
		std::exit(-1);
	}

	Mat image1 = image0.clone();

	medianBlur(image0, image0, 3);//��ֵ�˲�

	Mat hsv_image;
	cvtColor(image0, hsv_image, COLOR_BGR2HSV);//RGB to HSV

	Mat lower_red_hue_range;
	Mat upper_red_hue_range;
	inRange(hsv_image, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
	inRange(hsv_image, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);//��ɫ���ص�HSV��Χ
	Mat red_hue_image;
	addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
	GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 2, 2);//��˹�˲���ƽ������


	Mat green_hue_range;
	inRange(hsv_image, Scalar(35, 100, 100), Scalar(77, 255, 255), green_hue_range);//��ɫ���ص�HSV��Χ
	GaussianBlur(green_hue_range, green_hue_range, Size(9, 9), 2, 2);//��˹�˲���ƽ������


	std::vector<Vec3f> circles_red;
	circles_red.clear();
	HoughCircles(red_hue_image, circles_red, CV_HOUGH_GRADIENT, 1, red_hue_image.rows / 8, 100, 20, 0, 0);//����Բ�������

	std::vector<Vec3f> circles_green;
	circles_green.clear();
	HoughCircles(green_hue_range, circles_green, CV_HOUGH_GRADIENT, 1, green_hue_range.rows / 8, 100, 20, 0, 0);//����Բ������̵�
	
	// ���ݻ�Բ�ĸ������ж�ָʾ�Ƶ���ɫ
	if (circles_red.size() == 0 && circles_green.size() == 0)
	{
		MessageBox(NULL, _T("��ָʾ��"), _T("���"), MB_OK | MB_ICONWARNING);
	}
	
	for (size_t current_circle = 0; current_circle < circles_red.size(); ++current_circle)
	{	
		Point center(std::round(circles_red[current_circle][0]), std::round(circles_red[current_circle][1]));
		int radius = std::round(circles_red[current_circle][2]);

		if(circles_red.size() == 1 && radius != 0)
			MessageBox(NULL, _T("ָʾ��Ϊ��ɫ"), _T("���"), MB_OK | MB_ICONWARNING);
		else
			MessageBox(NULL, _T("ָʾ��Ϊ��ɫ"), _T("���"), MB_OK | MB_ICONWARNING);

		circle(image1, center, radius, Scalar(0, 0, 255), 5);
	}

	namedWindow("���̵Ƽ����", WINDOW_AUTOSIZE);
	imshow("���̵Ƽ����", image1);

	waitKey(0);

	return 0;
}