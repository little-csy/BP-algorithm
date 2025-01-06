#include<bits/stdc++.h>
#include<graphics.h>
using namespace std;
#pragma GCC optimize(3)//���ٴ������� 
#define MAX_Count 10000 //���ѵ������ 
#define MIN_Error 0.0001//��С������ 
#define inf 999999999//���� 
typedef vector< vector<double> > Mat;
typedef vector<double> Vec;
Vec operator + (const Vec& x, const Vec& y) {//���غ���+ �൱��������� 
	Vec tmp(x);
	int r = x.size();
	for (int a = 0; a < r; a++)
		tmp[a] += y[a];
	return tmp;
}
Vec operator - (const double& u, const Vec& x) {//���غ���- �൱��������� 
	Vec tmp(x);
	for (auto& i : tmp)
		i = u - i;
	return tmp;
}

Vec operator * (const Vec& x, const Mat& y) {//���غ���* �൱���������
	if (x.size() != y.size() || x.empty() || y.empty()) { cout << "error!" << endl; return Vec(); }//�ж��Ƿ���Ͼ�����˵����� 
	int r = y.size(), c = y[0].size();
	Vec tmp(c, 0.0);

	for (int a = 0; a < c; a++)
		for (int b = 0; b < r; b++)
			tmp[a] += x[b] * y[b][a];
	return tmp;
}

Mat operator ! (const Mat& x) { //���غ������൱�ڶ�W�������ת�� 
	if (x.empty()) { cout << "error!" << endl; return Mat(); }
	int r = x.size(), c = x[0].size();
	Mat tmp(c, Vec(r));
	for (int a = 0; a < c; a++)
		for (int b = 0; b < r; b++)
			tmp[a][b] = x[b][a];
	return tmp;
}

Vec Sigmoid(Vec x) {//����� 
	for (auto& i : x)
		i = 1.0 / (1.0 + exp(-i));
	return x;
}

vector<double>Forward(Vec one_data, Mat W[], Vec b[], int n, Vec& Hide1) {
	Vec out = one_data;
	for (int p = 0; p < n; p++) {
		out = Sigmoid(out * !W[p] + b[p]);//��W����ת���ٺ�����out��ˣ��ټ�b 
		if (p == 0) Hide1 = out;//�������->������������ΪHide1 
	}
	return out;
}

class Update {
public:
	void getsita3(Vec& sita3, Vec realY, Vec nowY, int n3) {// ��������㷴�򴫲��Ĳв�
		for (int i = 0; i < n3; i++)
		 sita3[i] = nowY[i] * (1 - nowY[i]) * (nowY[i] - realY[i]);//�вʽ[f��z��-y ]*f'(z)
	}
	void getsita2(Vec& sita2, Mat W2, Vec sita3, Vec nowHideY, int n2, int n3) {//�������ز㷴�򴫲��Ĳв� 

		for (int i = 0; i < n2; i++) {
			double sum = 0;
			for (int j = 0; j < n3; j++) {//�Ƶ��ĲвʽΪ[W*sita3]*f'(z)
				sum += W2[j][i] * sita3[j];
			}
			sita2[i] = sum * nowHideY[i] * (1 - nowHideY[i]);
		}
	}
	void update(Mat& W1, Vec& b1, Mat& W2, Vec& b2, Vec a1, Vec a2, Vec sita2, Vec sita3, int n1, int n2, int n3, double C) {

		for (int i = 0; i < n1; i++) {//���������->�������Ȩֵ 
			for (int j = 0; j < n2; j++) {
				W1[j][i] = W1[j][i] - C * sita2[j] * a1[i];
			}
		}

		for (int i = 0; i < n2; i++) {//����������->������Ȩֵ 
			for (int j = 0; j < n3; j++) {
				W2[j][i] = W2[j][i] - C * sita3[j] * a2[i];
			}
		}
		for (int i = 0; i < n2; i++) {//���������->�������ƫֵ 
			b1[i] = b1[i] - C * sita2[i];
		}
		for (int i = 0; i < n3; i++) {//����������->������Ȩƫֵ 
			b2[i] = b2[i] - C * sita3[i];
		}
	}
	double getError(Vec realY, Vec nowY, int n) {//����������ǩ��ŷʽ���� 
		double ans = 0;
		for (int i = 0; i < n; i++) {
			ans += (realY[i] - nowY[i]) * (realY[i] - nowY[i]);
		}
		ans *= 0.5;
		return ans;
	}
}upd;

class inputWork
{
public:
	struct node
	{
		double lab, sp, sw, pl, pw;
		char Name[25];
	}Load[5005], Min, Max;
	double Make(double t1, double tMax, double tMin)//��ÿ����й�һ�� 
	{
		return (t1 - tMin) / (tMax - tMin);
	}
	void readdata(int N, vector < vector<double> >& traindata, int& train_num, vector < vector<double> >& testdata, int& test_num)
	{//�����ݼ��ֻ���ѵ�����Ͳ��Լ� 
		train_num = 0, test_num = 0;
		traindata.clear();//��ʼ������ 
		testdata.clear();
		string csh, Nowcsh;//����Ϊstring���� 

		for (int i = 1; i <= N; i++)
		{
			cin >> csh;
			for (int j = 0; j <= 2; j++)//���೤�� ���ַ��Ͷ�ȡС������5.1���������ֽڷֱ�洢5��.��1 
				Nowcsh[j] = csh[j];
			Load[i].sp = atof(Nowcsh.c_str());//����ȡ�������ֽ�����atof����ת����С������ 
			for (int j = 0; j <= 2; j++)//������ 
				Nowcsh[j] = csh[j + 4];
			Load[i].sw = atof(Nowcsh.c_str());
			for (int j = 0; j <= 2; j++)//���곤�� 
				Nowcsh[j] = csh[j + 8];
			Load[i].pl = atof(Nowcsh.c_str());
			for (int j = 0; j <= 2; j++)//������ 
				Nowcsh[j] = csh[j + 12];
			Load[i].pw = atof(Nowcsh.c_str());
			for (int j = 0; (csh[j + 17] >= 'a' && csh[j + 17] <= 'z') || (csh[j + 17] >= 'A' && csh[j + 17] <= 'Z'); j++)
				Load[i].Name[j] = csh[j + 16];//��ǩ���� 
			Max.sp = max(Load[i].sp, Max.sp);
			Max.sw = max(Load[i].sw, Max.sw);
			Max.pl = max(Load[i].pl, Max.pl);
			Max.pw = max(Load[i].pw, Max.pw);
			Min.sp = min(Load[i].sp, Min.sp);
			Min.sw = min(Load[i].sw, Min.sw);
			Min.pl = min(Load[i].pl, Min.pl);
			Min.pw = min(Load[i].pw, Min.pw);
			Load[i].lab = (i - 1) / 50 + 1;//����ǩ����1��2��3��ֵ 

		}
		for (int i = 1; i <= N; i++)
		{
			Load[i].sp = Make(Load[i].sp, Max.sp, Min.sp);//��ÿ����й�һ�� 
			Load[i].sw = Make(Load[i].sw, Max.sw, Min.sw);
			Load[i].pw = Make(Load[i].pw, Max.pw, Min.pw);
			Load[i].pl = Make(Load[i].pl, Max.pl, Min.pl);
		}
		for (int i = 1; i <= N; i++)//�������ݼ��ֳ�ѵ�����Ͳ��Լ� 
		{
			vector <double> tmp{ Load[i].lab,Load[i].sp,Load[i].sw,Load[i].pw,Load[i].pl };
			if (i % 5 != 0)
			{
				train_num++;
				traindata.push_back(tmp);
			}
			else
			{
				test_num++;
				testdata.push_back(tmp);
			}
		}
	}
};

void MakeRand(Mat W[], Vec b[], int n)//��ʼ��W��bΪ0~1  
{
	for (int t = 0; t < n; t++)
		for (int i = 0; i < W[t].size(); i++)
			for (int j = 0; j < W[t][0].size(); j++)
				W[t][i][j] = 1.0 * rand() / RAND_MAX;
	for (int t = 0; t < n; t++)
		for (int i = 0; i < b[t].size(); i++)
			b[t][i] = 1.0 * rand() / RAND_MAX;
}

int main() {
	Mat W[2] = { Mat(4,Vec(4)),Mat(3,Vec(4)) };//W[0]Ϊ4*4����������������㵽������Ȩֵ ��W[0]Ϊ3*4�����������������㵽�����Ȩֵ
	Vec b[2] = { Vec(4),Vec(3) };//b[0]Ϊ����㵽�������ƫ�� ��b[1]Ϊ�����㵽�����ƫֵ

	srand(time(NULL));
	MakeRand(W, b, 2);//��W��b��ʼ��Ϊ0~1 
	
	inputWork Now;//inputWork�� 
	vector < vector<double> > Nowtraindata;//ѵ���� 
	vector < vector<double> > Nowtestdata;//���Լ� 
	int train_num, test_num;
	freopen("iris.data", "r", stdin);//�����ļ� 
	Now.readdata(150, Nowtraindata, train_num, Nowtestdata, test_num);//����150�����ݣ��ֻ���ѵ�����Ͳ��Լ� 
	
	int cnt = 0;//�������� 
	int n1 = 4, n2 = 4, n3 = 3;//�����n1Ϊ4���� ������n2Ϊ4���������n3Ϊ3�� 
	double C = 0.001;//CΪѧϰ�� 
	double error = inf;
	initgraph( 1000, 800);
	while (error > MIN_Error && cnt < MAX_Count) {//ѭ��ֹͣ������������С��0.0001��ѵ������Ϊ10000 

		error = 0;//ÿ�ֵ�������ʧ�ʼ�Ϊ0 
		for (int i = 0; i < train_num; i++) {

			Vec tmp = Nowtraindata[i];
			tmp.erase(tmp.begin());//tmp����ĵ�һ��ֵΪ���� ������ȥ��
			Vec sita3(3, 0.0), sita2(4, 0.0), nowHideY(4, 0.0);//sita3Ϊ�����Ĳвsita2Ϊ������ĲвnowHiderYΪ�����㵱ǰ�ļ���ֵ 
			Vec nowY = Forward(tmp, W, b, 2, nowHideY), realY(3, 0.0);//nowY����ǰ�򴫲��Ľ����realYΪ��ʵ��ǩ 
			realY[(int)Nowtraindata[i][0] - 1] = 1.0;//����i�����ݵ���ʵ�����ǵ�j�࣬����һ��1*3�����¼����jλΪ1����Ϊ0 

			upd.getsita3(sita3, realY, nowY, n3);// ���������Ĳв�

			upd.getsita2(sita2, W[1], sita3, nowHideY, n2, n3);// ����������Ĳв�

			upd.update(W[0], b[0], W[1], b[1], tmp, nowHideY, sita2, sita3, n1, n2, n3, C);//����W��b 
			
			error += upd.getError(realY, nowY, n3);//����ÿ�ֵ���������ʧ�� 
			
		}
		//��ͼ
		char title[20]="��ʧ������";
		setfont(30,15,"����");
		outtextxy(400, 50, title);
		if(cnt==0){
			char array[20];
			_gcvt_s(array,20,error,4);
			outtextxy(50, 50, array);
		}
		if(cnt==9999){
					char array1[20];
					_gcvt_s(array1,20,error,4);
					outtextxy(900, 650, array1);
				}
	    setcolor( WHITE );
	    setfillcolor(WHITE);
	    line(25,0,25,1000);
	    line(0,700,1000,700);
		fillellipse(cnt+26,1000-error*10,2,2);
		cnt++;	
	}
	Sleep(5000);
	Vec Load;
	int Ans = 0;
	for (int i = 0; i < test_num; i++)//���н������ 
	{
		Vec tmp = Nowtestdata[i], tmp2;//tmp������Լ���tmp2���������->������������� 
		tmp.erase(tmp.begin());//��tmp�������ǩ��ȥ 
		Load = Forward(tmp, W, b, 2, tmp2);//tmp���򴫲��öԸ��������Ԥ��ֵ 

		if (Nowtestdata[i][0] == 1)//�ڲ��Լ���ʵ��ǩΪ����1������� 
		{
			if (Load[0] > Load[1] && Load[0] > Load[2])//���Ԥ��ֵ������1ȡ�����ֵ����Ԥ����ȷ��������һ 
				Ans++;
		}
		if (Nowtestdata[i][0] == 2)
		{
			if (Load[1] > Load[0] && Load[1] > Load[2])
				Ans++;
		}
		if (Nowtestdata[i][0] == 3)
		{
			if (Load[2] > Load[1] && Load[2] > Load[0])
				Ans++;
		}
	}
	cout << 100.0 * Ans / test_num << "%" << endl;//�����ȷ�� 

	return 0;
}
