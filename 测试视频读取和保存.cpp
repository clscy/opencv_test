#include <iostream>	// for standard I/O
#include <string>   // for strings

#include <opencv2/core/core.hpp>    
#include <opencv2/videoio/videoio.hpp>  // Video write
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
	const string source      = "hehe.mp4";           //��Ƶ�ļ�λ��
	const bool askOutputType = false;			  //��������ʾ���ú�ԭ��Ƶ��ͬ�����ʽ����ȷ���Լ�ѡ

	VideoCapture inputVideo(source);              //��Ƶ�Ľṹ��

	string::size_type pAt = source.find_last_of('.');                  // Find extension point
	const string NAME = source.substr(0, pAt) + "new" +".avi";   // Form the new name with container
	int ex = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));     // Get Codec Type- Int form

	// Transform from int to char via Bitwise operators
	char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};

	Size S = Size((int) inputVideo.get(CAP_PROP_FRAME_WIDTH), (int) inputVideo.get(CAP_PROP_FRAME_HEIGHT));

	VideoWriter outputVideo;                                        // Open the output
	if (askOutputType)
		outputVideo.open(NAME, ex=-1, inputVideo.get(CAP_PROP_FPS), S, true);
	else
		outputVideo.open(NAME, ex, inputVideo.get(CAP_PROP_FPS), S, true);


	cout << "��Ƶ��С����*�ߣ�: Width=" << S.width << "  Height=" << S.height<<endl;
	cout << "��֡��: " << inputVideo.get(CAP_PROP_FRAME_COUNT) << endl;
	cout << "��������: " << EXT << endl;

	Mat src, res;

	for(;;) //Show the image captured in the window and repeat
	{
		inputVideo >> src;              // read
		if (src.empty()) break;         // check if at end
		//cv::imshow("Ԥ��",src);
		//waitKey(50);
		//outputVideo.write(src); //save or
		outputVideo << src;
	}

	cout << "Finished writing" << endl;
	system("pause");
	return 0;
}
