#include "StdAfx.h"
#include "FaceRec.h"


FaceRec::FaceRec(void)
{
}


FaceRec::~FaceRec(void)
{
}


int* FaceRec::recognition(cv::Mat_<double> A,cv::Mat_<double> D,cv::Mat_<double> y_AsCol,int RecNum,int nTotals)
{
	//��һ��
	double y_AsCol_norm=cv::norm(y_AsCol);
	for (int i=0;i<y_AsCol.rows;i++)
	{
		y_AsCol.at<double>(i)=y_AsCol.at<double>(i)/y_AsCol_norm;
	}
	
	//A��ÿ�й�һ��
	for (int j=0;j<A.cols;j++)
	{
		double a=cv::norm(A.col(j));
		for (int i=0;i<A.rows;i++)
		{
			A.at<double>(i,j)=A.at<double>(i,j)/a;
		}
	}

	//D��ÿ�й�һ��
	for (int j=0;j<D.cols;j++)
	{
		double a=cv::norm(D.col(j));
		for (int i=0;i<D.rows;i++)
		{
			D.at<double>(i,j)=D.at<double>(i,j)/a;
		}
	}  

	//�ϲ�A��D
	cv::Mat_<double> A_D;   //A��D���������ϲ���
    cv::Mat_<double> A_t,D_t;   //A,D��ת��

	A_t=A.t();
	D_t=D.t();

	A_D.push_back(A_t);
	A_D.push_back(D_t);
	A_D=A_D.t();  
	cv::Mat_<double> x_out;  //�����ϡ�����
	x_out=BPDN_homotopy_function(A_D,y_AsCol,0);

	//std::cout<<x_out;

	//���� 
	cv::Mat_<double> r_y=cv::Mat::zeros(A.cols,1,CV_64FC1);   //�������
	cv::Mat_<double> d_out=x_out.rowRange(A.cols,A_D.cols);   //x_out��D��ϵ��

	for (int i=0;i<A.cols;i++)
	{
		cv::Mat_<double> x_out_sub=cv::Mat::zeros(A.cols,1,CV_64FC1);  //x_out��A��ϵ��ֻ��һ��Ϊ��0ʱ�����
		x_out_sub.at<double>(i)=x_out.at<double>(i);
		x_out_sub.push_back(d_out);

		double a=cv::norm(y_AsCol-A_D*x_out_sub);
		r_y.at<double>(i)=a;
	}

	int *RecState=new int[RecNum]();   //ʶ��״̬��1��ʾ��ʶ��0��ʾδ��ʶ��

	for (int k=0;k<RecNum;k++)
	{
		double c1,c;   //r_y����С�����ֵ
		cv::Point d1,d;  //c1,c��r_y�е�λ��
		int i1;  //c1,c��r_y�е�����

		cv::minMaxLoc(r_y,&c1,&c,&d1,&d);   //Ѱ��r_y����С�����ֵ��������Ӧλ��
		i1=d1.y;   

		RecState[k]=i1;   //ʶ��ɹ�������1
		r_y.at<double>(i1)=DBL_MAX;
	}

	return RecState;
}

int  FaceRec::recognition2(cv::Mat_<double> A,cv::Mat_<double> D,cv::Mat_<double> y_AsCol)
{
	//��һ��
	double y_AsCol_norm=cv::norm(y_AsCol);
	for (int i=0;i<y_AsCol.rows;i++)
	{
		y_AsCol.at<double>(i)=y_AsCol.at<double>(i)/y_AsCol_norm;
	}

	//A��ÿ�й�һ��
	for (int j=0;j<A.cols;j++)
	{
		double a=cv::norm(A.col(j));
		for (int i=0;i<A.rows;i++)
		{
			A.at<double>(i,j)=A.at<double>(i,j)/a;
		}
	}

	//D��ÿ�й�һ��
	for (int j=0;j<D.cols;j++)
	{
		double a=cv::norm(D.col(j));
		for (int i=0;i<D.rows;i++)
		{
			D.at<double>(i,j)=D.at<double>(i,j)/a;
		}
	}  

	//�ϲ�A��D
	cv::Mat_<double> A_D;   //A��D���������ϲ���
	cv::Mat_<double> A_t,D_t;   //A,D��ת��

	A_t=A.t();
	D_t=D.t();

	A_D.push_back(A_t);
	A_D.push_back(D_t);
	A_D=A_D.t();  
	cv::Mat_<double> x_out;  //�����ϡ�����
	x_out=BPDN_homotopy_function(A_D,y_AsCol,0);

	//std::cout<<x_out;

	//���� 
	cv::Mat_<double> r_y=cv::Mat::zeros(A.cols,1,CV_64FC1);   //�������
	cv::Mat_<double> d_out=x_out.rowRange(A.cols,A_D.cols);   //x_out��D��ϵ��

	for (int i=0;i<A.cols;i++)
	{
		cv::Mat_<double> x_out_sub=cv::Mat::zeros(A.cols,1,CV_64FC1);  //x_out��A��ϵ��ֻ��һ��Ϊ��0ʱ�����
		x_out_sub.at<double>(i)=x_out.at<double>(i);
		x_out_sub.push_back(d_out);

		double a=cv::norm(y_AsCol-A_D*x_out_sub);
		r_y.at<double>(i)=a;
	}

	double c1,c;   //r_y����С�����ֵ
	cv::Point d1,d;  //c1,c��r_y�е�λ��
	int i1;  //c1,c��r_y�е�����

	cv::minMaxLoc(r_y,&c1,&c,&d1,&d);   //Ѱ��r_y����С�����ֵ��������Ӧλ��
	i1=d1.y;   

	int result=i1+1;
	return result;
}

//ͬ���㷨������
cv::Mat_<double> FaceRec::BPDN_homotopy_function(cv::Mat_<double> A,cv::Mat_<double> y,double tolerance)
{
	int N=A.cols;   
	int K=A.rows;
	int maxiter=10*K;  //����������
	double tau=1e-2;   //���򻯲�������ֵ����namuta������ֵ

	cv::Mat_<double> z_x=cv::Mat::zeros(N,1,CV_64FC1);  //������
	cv::Mat_<int> gamma_x;   //֧�ż�

	//��ʼ����
	cv::Mat_<double> Primal_constrk = -A.t()*y;
	cv::Mat_<double> abs_Primal_constrk=cv::abs(Primal_constrk);
	double c1,c;   //abs_Primal_constrk����С�����ֵ
	cv::Point d1,d;  //c1,c��abs_Primal_constrk�е�λ��
	int i;  //c1,c��abs_Primal_constrk�е�����

	cv::minMaxLoc(abs_Primal_constrk,&c1,&c,&d1,&d);   //Ѱ��abs_Primal_constrk����С�����ֵ��������Ӧλ��
	i=d.y;

	cv::Mat_<int> gamma_xk(1,1);  //֧�ż�gamma_x���м�ֵ
	gamma_xk.at<int>(0,0)=i;

	double epsilon = c;
	cv::Mat_<double> xk_1=cv::Mat::zeros(N,1,CV_64FC1);   //���Ϊ������

	double f=epsilon*cv::norm(xk_1,cv::NORM_L1)+0.5*cv::pow(cv::norm(y-A*xk_1),2); //��Լ��������ʽ

	z_x.at<double>(i)=-sgn(Primal_constrk.at<double>(i));
	Primal_constrk.at<double>(i)=sgn(Primal_constrk.at<double>(i))*epsilon;
	cv::Mat_<double> z_xk=z_x.clone();  //z_x���м�ֵ

	//��·����
	int iter=0;   //��������
	double old_delta = 0;  //ǰһ���Ĳ���
	//cv::Mat_<int> out_x;  //�Ƴ�֧�ż������
	int count_delta_stop = 0;

	cv::Mat_<double> AtgxAgx=A.col(i).t()*A.col(i);     //�������е�AI��ת�ó���AI
	cv::Mat_<double> iAtgxAgx=AtgxAgx.inv();

	while (iter<maxiter)
	{
		iter++;   //����������1

		//���²���
		gamma_x=gamma_xk.clone();
		z_x=z_xk.clone();
		cv::Mat_<double> x_k=xk_1.clone();

		//���·���
		cv::Mat_<double> z_x_sub;   //z_x��֧�ż���Ӧ�Ĳ���
		for (int i=0;i<gamma_x.rows;i++)
		{
			z_x_sub.push_back(z_x.row(gamma_x.at<int>(i)));
		}

		cv::Mat_<double> del_x = iAtgxAgx*z_x_sub;  //֧�ż��ķ�������
		cv::Mat_<double> del_x_vec = cv::Mat::zeros(N,1,CV_64FC1);  //�����ķ�������
		for (int i=0;i<gamma_x.rows;i++)
		{
			del_x_vec.at<double>(gamma_x.at<int>(i))=del_x.at<double>(i);
		}

		cv::Mat_<double> pk=Primal_constrk.clone();  //��¼Primal_constrk���������Primal_constrkҪ��

		cv::Mat_<double> A_sub;   //A����֧�ż���Ӧ����������ɵ�
		for (int i=0;i<gamma_x.rows;i++)
		{
			cv::Mat_<double> p=(A.col(gamma_x.at<int>(i))).t();
			A_sub.push_back(p);
		}
		A_sub=A_sub.t();

		cv::Mat_<double> dk=A.t()*(A_sub*del_x);  //A_sub*del_x�������е�vl

		cv::Mat_<double> pk_temp=Primal_constrk.clone();    //pk����ʱֵ
		cv::Mat_<double> xk_temp=x_k.clone();

		cv::Mat_<int> i_delta,out_x;  //��ӡ��Ƴ�֧�ż������
		//���㲽��
		double delta=update_primal(gamma_x,xk_temp,del_x_vec,pk_temp,dk,epsilon,i_delta,out_x);   

		//���²����������⡢C(I)�����򻯲���
		old_delta=delta;
		xk_1=x_k+delta*del_x_vec;
		Primal_constrk=pk+delta*dk;
		double epsilon_old=epsilon;
		epsilon=epsilon-delta;

		//��ֹ����1�����򻯲���С���趨ֵ
		if (epsilon<=tau)
		{
			xk_1=x_k+(epsilon_old-tau)*del_x_vec;
			break;
		}

		//��ֹ����2��
		bool keep_going;
		if (delta)
		{
			double prev_f=f;
			f=tau*cv::norm(xk_1,cv::NORM_L1)+0.5*cv::pow(cv::norm(y-A*xk_1),2);
			keep_going=abs((prev_f-f)/prev_f)>tolerance;
		}
		if(!keep_going)
			break;

		//��Ԫ�ش�֧�ż����Ƴ�
		if (out_x.data)
		{
			int len_gamma=gamma_x.rows; //֧�ż��ĳ���
			int outx_index;   //�Ƴ���Ԫ����֧�ż��е�λ��
			for (int i=0;i<gamma_x.rows;i++)
			{
				if(gamma_x.at<int>(i)==out_x.at<int>(0))
					outx_index=i;
			}

			//gamma_x�����һ�к͵�outx_index�л���
			gamma_x.at<int>(outx_index)=gamma_x.at<int>(len_gamma-1);
			gamma_x.at<int>(len_gamma-1)=out_x.at<int>(0);
			gamma_xk=(gamma_x.rowRange(0,len_gamma-1)).clone();

			//����gamma_x�仯���AtgxAgx��iAtgxAgx������ķ�����ֱ�Ӽ���ļ�����������
			//����AtgxAgx
			int rowi=outx_index;
			int colj=outx_index;
			cv::Mat_<double> AtgxAgx_ij = AtgxAgx.clone();
			cv::Mat_<double> temp_row=(AtgxAgx_ij.row(rowi)).clone();
			///AtgxAgx_ij�����һ�к͵�rowi�л���
			for (int i=0;i<AtgxAgx_ij.cols;i++)
			{
				AtgxAgx_ij.at<double>(rowi,i)=AtgxAgx_ij.at<double>(len_gamma-1,i);
				AtgxAgx_ij.at<double>(len_gamma-1,i)=temp_row.at<double>(i);
			}
			///AtgxAgx_ij�����һ�к͵�colj�л���
			cv::Mat_<double> temp_col=(AtgxAgx_ij.col(colj)).clone();
			for (int i=0;i<AtgxAgx_ij.rows;i++)
			{
				AtgxAgx_ij.at<double>(i,colj)=AtgxAgx_ij.at<double>(i,len_gamma-1);
				AtgxAgx_ij.at<double>(i,len_gamma-1)=temp_col.at<double>(i);
			}

			//����iAtgxAgx
			cv::Mat_<double> iAtgxAgx_ij=iAtgxAgx.clone();
			temp_row=(iAtgxAgx_ij.row(rowi)).clone();
			///iAtgxAgx_ij�����һ�к͵�rowi�л���
			for (int i=0;i<iAtgxAgx_ij.cols;i++)
			{
				iAtgxAgx_ij.at<double>(rowi,i)=iAtgxAgx_ij.at<double>(len_gamma-1,i);
				iAtgxAgx_ij.at<double>(len_gamma-1,i)=temp_row.at<double>(i);
			}
			///iAtgxAgx_ij�����һ�к͵�colj�л���
			temp_col=(iAtgxAgx_ij.col(colj)).clone();
			for (int i=0;i<iAtgxAgx_ij.rows;i++)
			{
				iAtgxAgx_ij.at<double>(i,colj)=iAtgxAgx_ij.at<double>(i,len_gamma-1);
				iAtgxAgx_ij.at<double>(i,len_gamma-1)=temp_col.at<double>(i);
			}

			AtgxAgx=(AtgxAgx_ij.rowRange(0,len_gamma-1).colRange(0,len_gamma-1)).clone();
			iAtgxAgx = update_inverse(AtgxAgx_ij, iAtgxAgx_ij,2);

			xk_1.at<double>(out_x.at<int>(0,0))=0;
		} 
		else  //����Ԫ�ؼ���֧�ż�
		{
			gamma_xk=gamma_x.clone();
			gamma_xk.push_back(i_delta);

			cv::Mat_<double> A_sub_t;   //A����֧�ż���Ӧ����������ɵľ����ת��
			for (int i=0;i<gamma_x.rows;i++)
			{
				cv::Mat_<double> p=(A.col(gamma_x.at<int>(i))).t();
				A_sub_t.push_back(p);
			}

			cv::Mat_<double> AtgxAnx=A_sub_t*A.col(i_delta.at<int>(0,0));
			int n=AtgxAgx.rows+1;  
			cv::Mat_<double> AtgxAgx_mod=cv::Mat::zeros(n,n,CV_64FC1);

			for (int i=0;i<n-1;i++)
				for(int j=0;j<n-1;j++)
					AtgxAgx_mod.at<double>(i,j)=AtgxAgx.at<double>(i,j);

			for(int i=0;i<n-1;i++)
				AtgxAgx_mod.at<double>(i,n-1)=AtgxAnx.at<double>(i);

			cv::Mat_<double> AtgxAnx_t=AtgxAnx.t();
			for(int i=0;i<n-1;i++)
				AtgxAgx_mod.at<double>(n-1,i)=AtgxAnx_t.at<double>(i);

			cv::Mat_<double> A_i_delta=A.col(i_delta.at<int>(0,0)).clone();
			cv::Mat_<double> A_i_delta_t=A_i_delta.t();
			cv::Mat_<double> A_i_delta_t_A_i_delta=A_i_delta_t*A_i_delta;
			AtgxAgx_mod.at<double>(n-1,n-1)=A_i_delta_t_A_i_delta.at<double>(0,0);

			AtgxAgx=AtgxAgx_mod.clone();
			iAtgxAgx = update_inverse(AtgxAgx, iAtgxAgx,1);

			xk_1.at<double>(i_delta.at<int>(0,0))=0;
			gamma_x=gamma_xk.clone();
		}

		z_xk=cv::Mat::zeros(N,1,CV_64FC1);

		for (int i=0;i<gamma_xk.rows;i++)
			z_xk.at<double>(gamma_xk.at<int>(i))=-sgn(Primal_constrk.at<double>(gamma_xk.at<int>(i)));
		for(int i=0;i<gamma_x.rows;i++)
			Primal_constrk.at<double>(gamma_x.at<int>(i))=sgn(Primal_constrk.at<double>(gamma_x.at<int>(i)))*epsilon;
	}

	cv::Mat_<double> x_out=xk_1.clone();
	return x_out;
}


//���ź���
int FaceRec::sgn(double x)
{
	if (x>0) return(1);    
	if (x<0) return(-1);   
	return(0);             

}

//���²�������
//gamma_x:֧�ż���x_k���ⲽ�����õ��Ľ⣻del_x_vec������������pk��dk��epsilon�������ô��� ��������������������ã�i_delta����֧�ż���������֧�ż����Ǹ�Ԫ�ص�λ�ã�out_x���Ƴ�֧�ż����Ǹ�Ԫ�ص�λ��
double FaceRec::update_primal(cv::Mat_<int> gamma_x,cv::Mat_<double> x_k,cv::Mat_<double> del_x_vec,cv::Mat_<double> pk,cv::Mat_<double> dk,double epsilon,cv::Mat_<int> &i_delta,cv::Mat_<int> &out_x)
{
	int N=x_k.rows;
	cv::Mat_<int> temp_gamma=cv::Mat::zeros(N,1,CV_32SC1);
	for (int i=0;i<gamma_x.rows;i++)
		temp_gamma.at<int>(gamma_x.at<int>(i))=gamma_x.at<int>(i)+1;

	double delta;  //Ҫ���صĲ���

	//����Ҫ����һ��д��ʱ������ˣ���֧�ż�����������Nʱ��gamma_lc��Ϊ���ˣ������ټ�����
	if (gamma_x.rows<N)
	{
		cv::Mat_<int> gamma_lc=cv::Mat::zeros(N-gamma_x.rows,1,CV_32SC1);  //֧�ż��Ĳ���
		int j=0;  
		for (int i=0;i<N;i++)
		{
			int a=temp_gamma.at<int>(i);
			if(a==0)
			{
				gamma_lc.at<int>(j)=i;
				j++;
			}
		}

		//���������е�ʽ8
		cv::Mat_<double> pk_gamma_lc,dk_gamma_lc;  //pk��dk���Ӽ�����֧�ż��Ĳ���λ�ô���Ԫ����ɣ�
		for (int i=0;i<gamma_lc.rows;i++)
		{
			pk_gamma_lc.push_back(pk.row(gamma_lc.at<int>(i)));
			dk_gamma_lc.push_back(dk.row(gamma_lc.at<int>(i)));
		}

		cv::Mat_<double> delta1_constr;   //������ʽ8�����ŵ�ǰһ��
		cv::Mat_<double> delta2_constr;    //������ʽ8�����ŵĺ�һ��
		cv::divide(epsilon-pk_gamma_lc,1+dk_gamma_lc,delta1_constr);
		cv::divide(epsilon+pk_gamma_lc,1-dk_gamma_lc,delta2_constr);

		//�����Ǽ����������еĴ��������Сֵ������֧�ż������е�λ��
		cv::Mat_<int> delta1_pos_ind;
		cv::Mat_<int> delta2_pos_ind;
		for (int i=0;i<delta1_constr.rows;i++)
		{
			double a=delta1_constr.at<double>(i);
			if(a>0)
			{
				cv::Mat_<int> b=(cv::Mat_<int>(1,1)<<i);
				delta1_pos_ind.push_back(b);
			}
		}
		for (int i=0;i<delta2_constr.rows;i++)
		{
			double a=delta2_constr.at<double>(i);
			if(a>0)
			{
				cv::Mat_<int> b=(cv::Mat_<int>(1,1)<<i);
				delta2_pos_ind.push_back(b);
			}
		}

		cv::Mat_<double> delta1_pos;
		cv::Mat_<double> delta2_pos;
		for (int i=0;i<delta1_pos_ind.rows;i++)
			delta1_pos.push_back(delta1_constr.row(delta1_pos_ind.at<int>(i)));
		for (int i=0;i<delta2_pos_ind.rows;i++)
			delta2_pos.push_back(delta2_constr.row(delta2_pos_ind.at<int>(i)));

		double c1,c2;   //delta1_pos����С�����ֵ
		cv::Point d1,d2;  //c1,c��delta1_pos�е�λ��
		cv::minMaxLoc(delta1_pos,&c1,&c2,&d1,&d2);   //Ѱ��abs_Primal_constrk����С�����ֵ��������Ӧλ��

		double delta1=c1;
		int i_delta1=d1.y;
		if (!delta1_pos.data)
			delta1=1e100;

		double e1,e2;   //delta1_pos����С�����ֵ
		cv::Point f1,f2;  //c1,c��delta1_pos�е�λ��
		cv::minMaxLoc(delta2_pos,&e1,&e2,&f1,&f2);   //Ѱ��abs_Primal_constrk����С�����ֵ��������Ӧλ��

		double delta2=e1;
		int i_delta2=f1.y;
		if (!delta2_pos.data)
			delta2=1e100;

		if (delta1>delta2)
		{
			delta=delta2;
			i_delta.push_back(gamma_lc.row(delta2_pos_ind.at<int>(i_delta2)));
		} 
		else
		{
			delta=delta1;
			i_delta.push_back(gamma_lc.row(delta1_pos_ind.at<int>(i_delta1)));
		}
	}
	else
		delta=1e100;

	//���������е�ʽ9
	cv::Mat_<double> x_k_gamma_x,del_x_vec_gamma_x;  //x_k��del_x_vec���Ӽ�����֧�ż�λ�ô���Ԫ����ɣ�
	for (int i=0;i<gamma_x.rows;i++)
	{
		x_k_gamma_x.push_back(x_k.row(gamma_x.at<int>(i)));
		del_x_vec_gamma_x.push_back(del_x_vec.row(gamma_x.at<int>(i)));
	}

	cv::Mat_<double> delta3_constr;   //������ʽ9��������������
	cv::divide(-x_k_gamma_x,del_x_vec_gamma_x,delta3_constr);

	cv::Mat_<int> delta3_pos_ind;
	for (int i=0;i<delta3_constr.rows;i++)
	{
		double a=delta3_constr.at<double>(i);
		if(a>0)
		{
			cv::Mat_<int> b=(cv::Mat_<int>(1,1)<<i);
			delta3_pos_ind.push_back(b);
		}
	}

	cv::Mat_<double> delta3_pos;
	for (int i=0;i<delta3_pos_ind.rows;i++)
		delta3_pos.push_back(delta3_constr.row(delta3_pos_ind.at<int>(i)));

	double g1,g2;   //delta1_pos����С�����ֵ
	cv::Point h1,h2;  //c1,c��delta1_pos�е�λ��
	cv::minMaxLoc(delta3_pos,&g1,&g2,&h1,&h2);   //Ѱ��abs_Primal_constrk����С�����ֵ��������Ӧλ��

	double delta3=g1;
	int i_delta3=h1.y;
	//cv::Mat_<int> out_x_index;


	if (!delta3_pos.data)
		delta3=1e100;

	if (delta3 <= delta)
	{
		delta=delta3;
		out_x.push_back(gamma_x.row(delta3_pos_ind.at<int>(i_delta3)));
	}

	return delta;
}

//����iAtgxAgx�ĺ���
cv::Mat_<double> FaceRec::update_inverse(cv::Mat_<double> AtB,cv::Mat_<double> iAtB_old,int flag)
{
	int n=AtB.rows;

	cv::Mat_<double> A12=AtB.rowRange(0,n-1).col(n-1).clone();
	cv::Mat_<double> A21=AtB.row(n-1).colRange(0,n-1).clone();
	double A22=AtB.at<double>(n-1,n-1);

	if(flag==1)
	{
		cv::Mat_<double> iA11=iAtB_old.clone();
		cv::Mat_<double> iA11A12=iA11*A12;
		cv::Mat_<double> A21iA11=A21*iA11;
		cv::Mat_<double> S_mat=A22-A21*iA11A12;
		double S=S_mat.at<double>(0,0);
		cv::Mat_<double> Q11_right=iA11A12*(A21iA11/S);

		cv::Mat_<double> iAtB=cv::Mat::zeros(n,n,CV_64FC1);
		cv::Mat_<double> a1=iA11+Q11_right;

		for (int i=0;i<n-1;i++)
			for(int j=0;j<n-1;j++)
				iAtB.at<double>(i,j)=a1.at<double>(i,j);

		a1= -iA11A12/S;
		for(int i=0;i<n-1;i++)
			iAtB.at<double>(i,n-1)=a1.at<double>(i);

		a1=-A21iA11/S;
		for(int i=0;i<n-1;i++)
			iAtB.at<double>(n-1,i)=a1.at<double>(i);

		iAtB.at<double>(n-1,n-1)=1/S;
		return iAtB;
	}
	else if (flag==2)
	{
		cv::Mat_<double> Q11=iAtB_old.rowRange(0,n-1).colRange(0,n-1).clone();
		cv::Mat_<double> Q12=iAtB_old.rowRange(0,n-1).col(n-1).clone();
		cv::Mat_<double> Q21=iAtB_old.row(n-1).colRange(0,n-1).clone();
		double Q22=iAtB_old.at<double>(n-1,n-1);
		cv::Mat_<double> Q21_div_Q22;
		cv::divide(Q21,Q22,Q21_div_Q22);
		cv::Mat_<double> Q12Q21_Q22=Q12*Q21_div_Q22;
		cv::Mat_<double> iAtB=Q11-Q12Q21_Q22;
		return iAtB;
	}
}