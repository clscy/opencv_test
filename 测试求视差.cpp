#include "opencv2/stereo.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv/cvaux.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <opencv2/viz.hpp>
//--------------------------------------------------------//
//--------------------------------------------------------//
using namespace std;
using namespace cv;
using namespace cv::stereo;
//--------------------------------------------------------//
//--------------------------------------------------------//
int main()
{
	Mat left = imread("imL2l.bmp", 0);
	Mat right = imread("imL2.bmp", 0);
	Mat dst;
	double minV, maxV;

	resize(left, left, Size(), 1, 1);
	resize(right, right, Size(), 1, 1);

	double t1 = getTickCount();
	Ptr<StereoBM> sBM = StereoBM::create();
	sBM->setPreFilterType(CV_STEREO_BM_NORMALIZED_RESPONSE);//Ԥ�����˲��������ͣ����ֿ�ѡ��CV_STEREO_BM_NORMALIZED_RESPONSE����CV_STEREO_BM_XSOBEL
	sBM->setBlockSize(15);//SAD���ڴ�С��һ����5*5��21*21֮�䣬��������
	sBM->setUniquenessRatio(15);//�Ӳ�Ψһ�԰ٷֱȣ�һ��5-15֮��
	sBM->setNumDisparities(32);//�Ӳ�ڣ�����Ӳ�ֵ����С�Ӳ�ֵ�Ĳ��С������16��������
	sBM->compute(left, right, dst);
	minMaxLoc(dst, &minV, &maxV);
	dst.convertTo(dst, CV_8UC1, 255/(maxV - minV));

	double t2 = getTickCount();
	double t = (double)(t2 - t1)/getTickFrequency();
	//cout << dst <<endl;
	imshow("src1", left);
	imshow("src2", right);
	imshow("BM", dst);

	cout << "BM�㷨��ʱ��"<< t <<endl;
	//waitKey(0);
	//-------------------------------------------------------------------------------------
	t1 = getTickCount();

	Ptr<StereoSGBM> sbbm = StereoSGBM::create(0, 16, 5);
	sbbm->setNumDisparities(32);////�Ӳ�ڣ�����Ӳ�ֵ����С�Ӳ�ֵ�Ĳ��С������16��������
	sbbm->setP1(484);//�����Ӳ�仯ƽ���ԵĲ������������ص��Ӳ�����1ʱ�ĳͷ�ϵ��
	sbbm->setP2(3872);//�����Ӳ�仯ƽ���ԵĲ������������ص��Ӳ�仯ֵ����1ʱ�ĳͷ�ϵ����P2�������P1
	sbbm->setBlockSize(11);//SAD���ڴ�С��һ��3*3��11*11֮�䣬����������
	sbbm->compute(left, right, dst);
	minMaxLoc(dst, &minV, &maxV);
	dst.convertTo(dst, CV_8UC1, 255/(maxV - minV));

	t2 = getTickCount();
	t = (double)(t2 - t1)/getTickFrequency();
	cout << "SGBM�㷨��ʱ��"<< t <<endl;
	//cout << dst <<endl;
	imshow("SGBM", dst);
	waitKey(0);
	//-------------------------------------------------------------------------------------



	return 0;
}

