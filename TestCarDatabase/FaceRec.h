#pragma once
#include <opencv.hpp>

class FaceRec
{
public:
	FaceRec(void);
	~FaceRec(void);


public:
	int* recognition(cv::Mat_<double> A,cv::Mat_<double> D,cv::Mat_<double> y_AsCol,int RecNum,int nTotals);   //������
	int  recognition2(cv::Mat_<double> A,cv::Mat_<double> D,cv::Mat_<double> y_AsCol);   //������2
	int sgn(double x);   //���ź���
	cv::Mat_<double> BPDN_homotopy_function(cv::Mat_<double> A,cv::Mat_<double> y,double tolerance);  //ͬ���㷨������
	//���²�������
	//gamma_x:֧�ż���x_k���ⲽ�����õ��Ľ⣻del_x_vec������������pk��dk��epsilon�������ô��� ��������������������ã�i_delta����֧�ż���������֧�ż����Ǹ�Ԫ�ص�λ�ã�out_x���Ƴ�֧�ż����Ǹ�Ԫ�ص�λ��
	double update_primal(cv::Mat_<int> gamma_x,cv::Mat_<double> x_k,cv::Mat_<double> del_x_vec,cv::Mat_<double> pk,cv::Mat_<double> dk,double epsilon,cv::Mat_<int> &i_delta,cv::Mat_<int> &out_x);
	cv::Mat_<double> update_inverse(cv::Mat_<double> AtB,cv::Mat_<double> iAtB_old,int flag);   //����iAtgxAgx�ĺ���
};

