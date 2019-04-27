#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/edge_filter.hpp>
#include <opencv2/text.hpp>
//-------------------------------------------------------------//
//-------------------------------------------------------------//
using namespace cv;
using namespace std;
using namespace ximgproc;
using namespace cv::text;
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void MyGuidedFilter(Mat& source, Mat& guided_image, Mat& output, int radius, float epsilon);
void makeDepth32f(Mat& source, Mat& output);
static void on_MyGuidedFilter(int, int, void *);
static void on_MyGuidedFilter1(int,  void *);
static void on_MyGuidedFilter2(int,  void *);

int g_radius = 10;
int g_epsilon = 2;
Mat src, dst, srcGuide, srcGray;
double my_g_epsilon;
//--------------------------------------------------------------//
int main()
{

	//***************************��һ�ֵ����˲�***************************//
	double t = getTickCount();

	src = imread("3.jpg",1);
	cout << "ͼ��ߴ磺" << src.cols << "*" << src.rows << endl;
	//resize(src,src,Size(800,500));
	cvtColor(src,srcGray,COLOR_BGR2GRAY);
	threshold(srcGray,srcGuide,100,255,THRESH_OTSU);
	srcGray = src.clone();
	guidedFilter(srcGuide, src, dst, 2, 300);
	imshow("ԭͼ",srcGray);
	imshow("����ͼ",srcGuide);
	imshow("�����˲�1",dst);

	t = ((double) getTickCount() - t)/getTickFrequency();
	cout << "�����˲�1��ʱ��" <<t<<endl;
	//***************************�ڶ��ֵ����˲�***************************//
	//t = getTickCount();
	//namedWindow("�����˲�2", 1);
	//createTrackbar("����ֵ�ߴ磺", "�����˲�2",&g_radius, 50,on_MyGuidedFilter1);	createTrackbar("�ο�ֵ�޶�ֵ:","�����˲�2", &g_epsilon, 50 ,on_MyGuidedFilter2);
	//on_MyGuidedFilter(g_radius,g_epsilon, 0);

	//MyGuidedFilter(srcGray,srcGray,dst,15,0.05);
	//imshow("�����˲�2",dst);

	//t = ((double) getTickCount() - t)/getTickFrequency();
	//cout << "�����˲�2��ʱ��" <<t<<endl;
	//***************************��һ�ֵ����˲�***************************//

	t = getTickCount();
	bilateralFilter(srcGray, dst, 20, 25*2, 25/2);
	imshow("˫���˲�",dst);

	t = ((double)getTickCount() - t)/getTickFrequency();
	cout << "˫���˲���ʱ��" <<t<<endl;
	GaussianBlur(srcGray,dst,Size(51,51),1.0);
	imshow("��˹�˲�",dst);
	waitKey(0);
	return 1;
}

void MyGuidedFilter(Mat& source, Mat& guided_image, Mat& output, int radius, float epsilon)
{
	//CV_Assert(radius >= 2 && epsilon > 0);
	//CV_Assert(source.data != NULL && source.channels() == 1);
	//CV_Assert(guided_image.channels() == 1);
	//CV_Assert(source.rows == guided_image.rows && source.cols == guided_image.cols);

	Mat guided;
	if (guided_image.data == source.data)
	{
		//make a copy
		guided_image.copyTo(guided);
	}
	else
	{
		guided = guided_image;
	}

	//��������չΪ32λ�����ͣ��Ա��Ժ����˷�
	Mat source_32f, guided_32f;
	makeDepth32f(source, source_32f);
	makeDepth32f(guided, guided_32f);

	//����I*p��I*I
	Mat mat_Ip, mat_I2;
	multiply(guided_32f, source_32f, mat_Ip);
	multiply(guided_32f, guided_32f, mat_I2);

	//������־�ֵ
	Mat mean_p, mean_I, mean_Ip, mean_I2;
	Size win_size(2*radius + 1, 2*radius + 1);
	boxFilter(source_32f, mean_p, CV_32F, win_size);
	boxFilter(guided_32f, mean_I, CV_32F, win_size);
	boxFilter(mat_Ip, mean_Ip, CV_32F, win_size);
	boxFilter(mat_I2, mean_I2, CV_32F, win_size);

	//����Ip��Э�����I�ķ���
	Mat cov_Ip = mean_Ip - mean_I.mul(mean_p);
	Mat var_I = mean_I2 - mean_I.mul(mean_I);
	var_I += epsilon;

	//��a��b
	Mat a, b;
	divide(cov_Ip, var_I, a);
	b = mean_p - a.mul(mean_I);

	//�԰�������i������a��b��ƽ��
	Mat mean_a, mean_b;
	boxFilter(a, mean_a, CV_32F, win_size);
	boxFilter(b, mean_b, CV_32F, win_size);

	//������� (depth == CV_32F)
	output = mean_a.mul(guided_32f) + mean_b;
}
//
void makeDepth32f(Mat& source, Mat& output)
{
	if (source.depth() != CV_32F ) 
	{
		source.convertTo(output, CV_32F,1/255.0);
	}
	else
	{
		output = source;
	}
}
//
static void on_MyGuidedFilter(int,int, void *)
{
	on_MyGuidedFilter1(g_radius,0);
	on_MyGuidedFilter2(g_epsilon,0);
}
static void on_MyGuidedFilter1(int,  void *)
{
	my_g_epsilon = (double)g_epsilon/100;
	MyGuidedFilter(srcGray,srcGray,dst,g_radius,my_g_epsilon);
	imshow("�����˲�2",dst);
}
static void on_MyGuidedFilter2(int,  void *)
{
	my_g_epsilon = (double)g_epsilon/100;
	MyGuidedFilter(srcGray,srcGray,dst,g_radius,my_g_epsilon);
	imshow("�����˲�2",dst);
}
