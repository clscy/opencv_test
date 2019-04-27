#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/edge_filter.hpp>
//-------------------------------------------------------------//
//-------------------------------------------------------------//
using namespace cv;
using namespace std;
using namespace ximgproc;
//-------------------------------------------------------------//
//-------------------------------------------------------------//
int main()
{
	Mat src, dst, srcGuide, srcGray;

	double t = getTickCount();

	src = imread("3.jpg",1);
	//resize(src,src,Size(800,500));
	cvtColor(src,srcGray,COLOR_BGR2GRAY);
	threshold(srcGray,srcGuide,100,255,THRESH_OTSU);
	guidedFilter(src,src,dst,100,50);

	imshow("ԭͼ",src);
	////imshow("�Ҷ�ͼ",srcGray);
	//imshow("��ֵͼ",srcGuide);
	//bitwise_not(srcGuide,srcGuide);
	//imshow("��ֵͼȡ��",srcGuide);
	//src.copyTo(srcGray, srcGuide);
	imshow("�����˲�",dst);
	imshow("test",srcGray);

	t = ((double) getTickCount() - t)/getTickFrequency();
	cout << "�����˲���ʱ��" <<t<<endl;

	t = getTickCount();
	//bilateralFilter(src, dst, 100, 25*2, 25/2);
	imshow("˫���˲�",dst);
	t = ((double)getTickCount() - t)/getTickFrequency();
	cout << "˫���˲���ʱ" <<t<<endl;

	waitKey(0);
}