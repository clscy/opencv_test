#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <io.h>
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
using namespace std;
using namespace cv;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
Rect myRoi;
Mat imgFrame;
string imgCurrentAddr;
bool flagStart = false;
bool flagEnd = false;
void getFiles(string path, vector<string>& files, vector<string>& fileNames);
//----------------------------------------------------------------------------------
static void myOnMouse(int event, int x, int y, int, void*)
{
	switch (event)
	{
	case EVENT_MOUSEMOVE:
	{
		if (flagStart && !flagEnd)
		{
			Mat temp;
			temp = imgFrame.clone();
			//imgFrame = imread(imgCurrentAddr);
			rectangle(temp, Point(myRoi.x, myRoi.y), Point(x, y), Scalar(0, 0, 255), 2);
			imshow("ģ��", temp);
		}

		break;
	}
	case EVENT_LBUTTONDOWN:
	{
		flagStart = true;
		myRoi.x = x;
		myRoi.y = y;
		flagEnd = false;

		break;
	}
	case EVENT_LBUTTONUP:
	{
		myRoi.width = abs(x - myRoi.x);
		myRoi.height = abs(y - myRoi.y);
		flagEnd = true;
		break;

	}
	}

}

int main()
{

	char* fileAddr = "..\\��λ�Ƽ����//roiModel";
	vector <string> vecFilesAddr, vecFileNames;

	getFiles(fileAddr, vecFilesAddr, vecFileNames);

	fstream wr("�궨��Ϣroi.txt", ios::out);

	for (int i = 0; i < vecFilesAddr.size(); i++)
	{
		namedWindow("ģ��", 1);
		Mat imgSrc = imread(vecFilesAddr[i]);
		//resize(imgSrc, imgSrc, Size(), 0.5, 0.5);
		imgFrame = imgSrc.clone();


		//imshow("ģ��", imgSrc);
		setMouseCallback("ģ��", myOnMouse, NULL);

		imshow("ģ��", imgFrame);

		waitKey(0);

		string tempName;
		char* nameEnd = strchr((char*)(vecFileNames[i].c_str()), '_');
		char* nameStart = (char*)vecFileNames[i].c_str();
		tempName = vecFileNames[i].substr(0, (size_t)(nameEnd - nameStart));

		wr << tempName << " " << myRoi.x  << " " << myRoi.y  << " " << myRoi.width  << " " << myRoi.height  << endl;
		nameEnd = NULL;
		nameStart = NULL;
	}
	wr.close();
	return 1;
}

void getFiles(string path, vector<string>& files, vector<string>& fileNames)
{
	//�ļ����
	long hFile = 0;
	//�ļ�����Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼���ͽ��е���
			//�������Ŀ¼���ͼ����ļ��б�files
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files, fileNames);
				}
			}
			else
			{
				fileNames.push_back(fileinfo.name);
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

int getDataLabel(string fileName, string labelStopFlage)//�����ļ����õ������ı�ǩ���
{
	string file = fileName;
	string label[20];

	for (int i = 0; i < file.size(); i++)
	{
		if (file.substr(i, 1).compare(labelStopFlage))
		{
			label[i] = file.substr(i, 1);
		}
		else
		{
			break;
		}
	}

	std::stringstream temp;
	int labelTrainData;

	temp << label[0] + label[1];
	temp >> labelTrainData;

	return labelTrainData;
}