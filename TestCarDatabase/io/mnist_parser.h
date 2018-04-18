/*
    Copyright (c) 2013, Taiga Nomi
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include "util/util.h"
#include <fstream>
#include <cstdint>

#include <cv.h>
#include <highgui.h> 
#include <cstring>

namespace tiny_cnn {

inline std::string * readTxt(const std::string& fileName, int size = 1)
{
	std::ifstream infile;
	infile.open(fileName);  //  ���ļ���
	std::string temp;
	std::string * imageInfo = new std::string[size];  //����������ÿ��ͼƬ������������Ϣ
	int count = 0;
	while (getline(infile, temp))  //
	{
		imageInfo[count] = temp;
		count++;
		//
	}

	return imageInfo;
	delete[] imageInfo;
}

inline void loadImage(const std::string& fileName, std::string& imageInfo, vec_t& imageGray, vec_t& label, vec_t& faceArea, std::string& imgName)
{
	//
	const int featurePointNum = 10;

	std::string  imageName;  // ����������ѵ��ͼƬ����
	std::string facePoint[14];  // ****����ȫ�ֱ������ÿ��ͼƬ�Ĵ�С��������  
	std::ifstream infile;
	infile.open(fileName);  //  ���ļ���
	int count = 0;

	int ii = 0;   //ͳ�Ƶ�һ���ո�ǰ�ж����ַ�
	for (unsigned int i = 0; i < imageInfo.length(); i++)   //����ͼƬ��//��ȡ�����ѵ��ͼƬ�����ƣ���С��������ľ�ȷλ��
	{
		if (imageInfo[i] != ' ')
		{
			ii++;
		}
		else
		{
			break;
		}
	}
	imageName = imageInfo.substr(0, ii);
	imgName = imageName;

	int n = 0;   // ����������
	for (unsigned int j = ii + 1; j < imageInfo.length(); j++)   // ����ͼƬ��С��������λ��
	{
		if (imageInfo[j] != ' ')
		{
			continue;
		}
		facePoint[n] = imageInfo.substr(ii + 1, j - ii - 1);
		ii = j;
		n++;
		//break;
	}
	//
	facePoint[13] = imageInfo.substr(ii + 1, imageInfo.length() - ii - 1);
	//
	faceArea.resize(4);  //  ����ͼƬ��������λ��
	faceArea[0] = atof(facePoint[0].c_str());
	faceArea[1] = atof(facePoint[1].c_str());
	faceArea[2] = atof(facePoint[2].c_str());
	faceArea[3] = atof(facePoint[3].c_str());

	float_t faceWidth = (atof(facePoint[1].c_str()) - atof(facePoint[0].c_str()));   //  ����ͼƬ�Ŀ�ȣ������꣩
	float_t faceHeight = (atof(facePoint[3].c_str()) - atof(facePoint[2].c_str()));  //  ����ͼƬ�ĸ߶ȣ������꣩
	label.resize(featurePointNum); // ��vector��������ռ�
	for (int i = 0; i < featurePointNum; i++)
	{
		if (i % 2 == 0)
		{
			label[i] = (atof(facePoint[4 + i].c_str()) - atof(facePoint[0].c_str())) / faceWidth; // �������һ��
		}
		else
		{
			label[i] = (atof(facePoint[4 + i].c_str()) - atof(facePoint[2].c_str())) / faceHeight; // �������һ��
		}

	}
	//
	count++;

	IplImage * img;
	IplImage * dst;
	//std::string path;  //  �洢�ļ���·��
	//path = "D:\\vs2013\\cnnEyesDetection\\cnnEyesDetection\\";
	//imageName = path + imageName;  //���ͼƬ�ľ���·��

	//cout << imageName << endl;
	const char * image = imageName.c_str();
	img = cvLoadImage(image, 0);  // ��ȡͼ��
	//img = imageName
	//img = cvLoadImage("D:\\vs2013\\cnnEyesDetection\\cnnEyesDetection\\Ariel_Sharon\\Ariel_Sharon_0001.jpg", 1);
	if (img == NULL)
	{
		std::cout << "�޷���ʾͼƬ" << std::endl;

	}

	// �������ͼ��Ҫ��ü�
	cvSetImageROI(img, cvRect(atoi(facePoint[0].c_str()), atoi(facePoint[2].c_str()),
		atoi(facePoint[1].c_str()) - atoi(facePoint[0].c_str()),
		atoi(facePoint[3].c_str()) - atoi(facePoint[2].c_str())));
	//�½�һ������Ҫ���е�ͼ�񲿷�ͬ����С����ͼ��
	dst = cvCreateImage(cvSize(atoi(facePoint[1].c_str()) - atoi(facePoint[0].c_str()), atoi(facePoint[3].c_str()) - atoi(facePoint[2].c_str())),
		IPL_DEPTH_8U,
		img->nChannels);
	//��Դͼ���Ƶ��½���ͼ����
	cvCopy(img, dst, 0);
	//�ͷ�ROI����
	cvResetImageROI(img);
	//���ü����ͼ�����ų�32 * 32
	CvSize czSize;
	czSize.height = 32;
	czSize.width = 32;
	IplImage * scalimage = cvCreateImage(czSize, dst->depth, dst->nChannels);
	cvResize(dst, scalimage, CV_INTER_AREA);

	//
	int width = scalimage->width;//ͼƬ���
	int height = scalimage->height;//ͼƬ�߶�
	//vec_t intensity;
	int i = 0;
	imageGray.resize(width * height);
	for (size_t row = 0; row < height; row++)
	{
		uchar* ptr = (uchar*)scalimage->imageData + row * scalimage->width;//��ûҶ�ֵ����ָ��
		for (size_t cols = 0; cols < width; cols++)
		{
			imageGray[i] = -(ptr[cols] / 255.0 * 2.0 - 1.0);
			i++;
		}
	}
	/*/
	for (int i = 0; i < width * height; i++)
	{
	std::cout << imageGray[i] << "  ";
	if (i % 10 == 9)
	{
	std::cout << std::endl;
	}
	}
	std::cin.get();
	//
	cvNamedWindow("Ariel_Sharon_0001", 1);  // ����ͼ���
	cvShowImage("Ariel_Sharon_0001", img);  //  ��ʾͼ��
	cvWaitKey(0);

	cvNamedWindow("������ͼ��", 1);  // �����µ�ͼ���
	cvShowImage("������ͼ��", dst);  //  ��ʾ�ü����ͼ��
	cvWaitKey(0);

	cvNamedWindow("���ź��ͼ��", 1);  // �����µ�ͼ���
	cvShowImage("���ź��ͼ��", scalimage);  // ��ʾ���ź��ͼ��
	cvWaitKey(0);

	cvDestroyWindow("���ź��ͼ��");
	cvDestroyWindow("������ͼ��");
	cvDestroyWindow("Ariel_Sharon_0001");
	cvReleaseImage(&img);
	cvReleaseImage(&dst);
	cvReleaseImage(&scalimage);
	/*/
}

inline void loadTestImage(const std::string path, vec_t& imageGray, vec_t& faceArea)  // faceArea������¼ͼƬ�Ĵ�С
{

	//imgName = imageName;  // ����ͼƬ��

	IplImage * img;
	IplImage * dst;

	//const char * image = path.c_str();

//	*img = IplImage(src);
	img = cvLoadImage(path.c_str(),0);  // ��ȡͼ��
	//img = imageName
	//img = cvLoadImage("D:\\vs2013\\cnnEyesDetection\\cnnEyesDetection\\Ariel_Sharon\\Ariel_Sharon_0001.jpg", 1);
	if (img == NULL)
	{
		std::cout << "�޷���ʾͼƬ" << std::endl;

	}

	// �������ͼ��Ҫ��ü�
	//cvSetImageROI(img, cvRect(atoi(facePoint[0].c_str()), atoi(facePoint[2].c_str()),
	//	atoi(facePoint[1].c_str()) - atoi(facePoint[0].c_str()),
	//	atoi(facePoint[3].c_str()) - atoi(facePoint[2].c_str())));
	//�½�һ������Ҫ���е�ͼ�񲿷�ͬ����С����ͼ��

	faceArea.resize(2);
	faceArea[0] = img->width * 1.0;
	faceArea[1] = img->height * 1.0;  // ȷ���ĸ��泤�ĸ����

	dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, img->nChannels);

	//��Դͼ���Ƶ��½���ͼ����
	cvCopy(img, dst, 0);
	//�ͷ�ROI����
	cvResetImageROI(img);
	//���ü����ͼ�����ų�32 * 32
	CvSize czSize;
	czSize.height = 32;
	czSize.width = 32;
	IplImage * scalimage = cvCreateImage(czSize, dst->depth, dst->nChannels);
	cvResize(dst, scalimage, CV_INTER_AREA);

	//
	int width = scalimage->width;//ͼƬ���
	int height = scalimage->height;//ͼƬ�߶�
	//vec_t intensity;
	int i = 0;
	imageGray.resize(width * height);
	for (size_t row = 0; row < height; row++)
	{
		uchar* ptr = (uchar*)scalimage->imageData + row * scalimage->width;//��ûҶ�ֵ����ָ��
		for (size_t cols = 0; cols < width; cols++)
		{
			imageGray[i] = -(ptr[cols] / 255.0 * 2.0 - 1.0);
			i++;
		}
	}
	/*/
	for (int i = 0; i < width * height; i++)
	{
		std::cout << imageGray[i] << "  ";
		if (i % 10 == 9)
		{
			std::cout << std::endl;
		}
	}
	std::cin.get();
	//
	cvNamedWindow("Ariel_Sharon_0001", 1);  // ����ͼ���
	cvShowImage("Ariel_Sharon_0001", img);  //  ��ʾͼ��
	cvWaitKey(0);

	cvNamedWindow("������ͼ��", 1);  // �����µ�ͼ���
	cvShowImage("������ͼ��", dst);  //  ��ʾ�ü����ͼ��
	cvWaitKey(0);

	cvNamedWindow("���ź��ͼ��", 1);  // �����µ�ͼ���
	cvShowImage("���ź��ͼ��", scalimage);  // ��ʾ���ź��ͼ��
	cvWaitKey(0);

	cvDestroyWindow("���ź��ͼ��");
	cvDestroyWindow("������ͼ��");
	cvDestroyWindow("Ariel_Sharon_0001");
	cvReleaseImage(&img);
	cvReleaseImage(&dst);
	cvReleaseImage(&scalimage);
	/*/
}

inline void getImageAndLabel(const std::string& image_file,
	std::vector<vec_t> *images, std::vector<vec_t> *labels, std::vector<vec_t> *faceAreas, std::vector<std::string> *imgNames)
{
	int size = 0;  //�����������ѵ��ͼƬ����

	std::ifstream infile;
	infile.open(image_file);  //  ���ļ���

	assert(infile.is_open());
	/*/
	if(!infile.is_open())   //  �ж��ļ�����û�д򿪳ɹ�
	{
		std::cout << "��������ļ��п��ܲ����ڣ������ļ�·�� " << std::endl;
		//std::cin >> fileName;
		//infile.open(image_file);	
		std::cin.get();
		exit(0);
	}
	/*/

	std::string temp;
	while (getline(infile, temp))  //��ȡ�ļ���ͼƬ���ܸ���
	{
		size++;
	}
	//cout << size << endl;
	infile.close();
	infile.clear();
	//infile.open(image_file);   //�����������ļ�

	std::string * imageInfo = new std::string[size];  //  ��readtxt�����ķ���ֵ��ȡ����
	imageInfo = readTxt(image_file, size);

	//
	for (int i = 0; i < size; i++)
	{
		vec_t image;
		vec_t label;
		vec_t faceArea;
		std::string imgName;
		//			std::cout << imageInfo[i] << std::endl;
		loadImage(image_file, imageInfo[i], image, label, faceArea, imgName);
		//			loadImage(imageInfo[i], image_file, image);
		images->push_back(image);
		labels->push_back(label);
		faceAreas->push_back(faceArea);
		imgNames->push_back(imgName);
	}
	//

}

inline void getTestImage(const std::string path,
	std::vector<vec_t> *images, std::vector<vec_t> *faceAreas,  int size = 1)
{
	for (int i = 0; i < size; i++)
	{
		vec_t image;
		vec_t faceArea;
		//std::string imgName;
		//			std::cout << imageInfo[i] << std::endl;
		loadTestImage(path, image, faceArea);

		images->push_back(image);
		faceAreas->push_back(faceArea);
		//imgNames->push_back(imgName);
	}

}

} // namespace tiny_cnn

