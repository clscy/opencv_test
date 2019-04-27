#include <opencv2/photo.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

void loadExposureSeq(String, vector<Mat>&, vector<float>&);

int main(int, char**argv)
{
	vector<Mat> images;
	vector<float> times;
	loadExposureSeq("D:\\computer visio\\���Ե����˲�\\����HDR", images, times);

	Mat response;
	//Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();//��ȡ�����������
	//calibrate->process(images, response, times);

	Mat hdr;
	Ptr<MergeDebevec> merge_debevec = createMergeDebevec();//�ϳ�HDRͼ��
	merge_debevec->process(images, hdr, times, response);

	Mat ldr;
	Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);//����ɫ��ӳ������LDRͼ��
	tonemap->process(hdr, ldr);

	Mat fusion;
	Ptr<MergeMertens> merge_mertens = createMergeMertens(); //�������������ֱ��Ӧ(�Աȶȣ����Ͷȣ��ع��)
	merge_mertens->process(images, fusion);

	imwrite("fusion.png", fusion*255 );
	imwrite("ldr.png", ldr*255);
	hdr.convertTo(hdr, CV_8UC3, 1, 50);
	imwrite("hdr.hdr", hdr);

	Mat temp;
	resize(hdr, temp, Size(500, 300));
	imshow("hdr", temp);
	resize(ldr, temp, Size(500, 300));
	imshow("LDR", temp);
	resize(fusion, temp, Size(500, 300));
	imshow("fusion", temp);
	waitKey(0);
	return 0;
}

void loadExposureSeq(String path, vector<Mat>& images, vector<float>& times)
{
	path = path + std::string("/");
	ifstream list_file((path + "test.txt").c_str());
	string name;
	float val;
	while(list_file >> name >> val) {
		Mat img = imread(path + name);
		images.push_back(img);
		times.push_back(1/val);
	}
	list_file.close();
}