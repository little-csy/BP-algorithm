#include<bits/stdc++.h>
#include<graphics.h>
using namespace std;
#pragma GCC optimize(3)//加速传播速率 
#define MAX_Count 10000 //最大训练轮数 
#define MIN_Error 0.0001//最小错误率 
#define inf 999999999//无穷 
typedef vector< vector<double> > Mat;
typedef vector<double> Vec;
Vec operator + (const Vec& x, const Vec& y) {//重载函数+ 相当于向量相加 
	Vec tmp(x);
	int r = x.size();
	for (int a = 0; a < r; a++)
		tmp[a] += y[a];
	return tmp;
}
Vec operator - (const double& u, const Vec& x) {//重载函数- 相当于向量相减 
	Vec tmp(x);
	for (auto& i : tmp)
		i = u - i;
	return tmp;
}

Vec operator * (const Vec& x, const Mat& y) {//重载函数* 相当于向量相乘
	if (x.size() != y.size() || x.empty() || y.empty()) { cout << "error!" << endl; return Vec(); }//判断是否符合矩阵相乘的条件 
	int r = y.size(), c = y[0].size();
	Vec tmp(c, 0.0);

	for (int a = 0; a < c; a++)
		for (int b = 0; b < r; b++)
			tmp[a] += x[b] * y[b][a];
	return tmp;
}

Mat operator ! (const Mat& x) { //重载函数！相当于对W矩阵进行转置 
	if (x.empty()) { cout << "error!" << endl; return Mat(); }
	int r = x.size(), c = x[0].size();
	Mat tmp(c, Vec(r));
	for (int a = 0; a < c; a++)
		for (int b = 0; b < r; b++)
			tmp[a][b] = x[b][a];
	return tmp;
}

Vec Sigmoid(Vec x) {//激活函数 
	for (auto& i : x)
		i = 1.0 / (1.0 + exp(-i));
	return x;
}

vector<double>Forward(Vec one_data, Mat W[], Vec b[], int n, Vec& Hide1) {
	Vec out = one_data;
	for (int p = 0; p < n; p++) {
		out = Sigmoid(out * !W[p] + b[p]);//将W进行转置再和输入out相乘，再加b 
		if (p == 0) Hide1 = out;//将输入层->隐含层的输出记为Hide1 
	}
	return out;
}

class Update {
public:
	void getsita3(Vec& sita3, Vec realY, Vec nowY, int n3) {// 计算输出层反向传播的残差
		for (int i = 0; i < n3; i++)
		 sita3[i] = nowY[i] * (1 - nowY[i]) * (nowY[i] - realY[i]);//残差公式[f（z）-y ]*f'(z)
	}
	void getsita2(Vec& sita2, Mat W2, Vec sita3, Vec nowHideY, int n2, int n3) {//计算隐藏层反向传播的残差 

		for (int i = 0; i < n2; i++) {
			double sum = 0;
			for (int j = 0; j < n3; j++) {//推导的残差公式为[W*sita3]*f'(z)
				sum += W2[j][i] * sita3[j];
			}
			sita2[i] = sum * nowHideY[i] * (1 - nowHideY[i]);
		}
	}
	void update(Mat& W1, Vec& b1, Mat& W2, Vec& b2, Vec a1, Vec a2, Vec sita2, Vec sita3, int n1, int n2, int n3, double C) {

		for (int i = 0; i < n1; i++) {//更新输入层->隐含层的权值 
			for (int j = 0; j < n2; j++) {
				W1[j][i] = W1[j][i] - C * sita2[j] * a1[i];
			}
		}

		for (int i = 0; i < n2; i++) {//更新隐含层->输出层的权值 
			for (int j = 0; j < n3; j++) {
				W2[j][i] = W2[j][i] - C * sita3[j] * a2[i];
			}
		}
		for (int i = 0; i < n2; i++) {//更新输入层->隐含层的偏值 
			b1[i] = b1[i] - C * sita2[i];
		}
		for (int i = 0; i < n3; i++) {//更新隐含层->输出层的权偏值 
			b2[i] = b2[i] - C * sita3[i];
		}
	}
	double getError(Vec realY, Vec nowY, int n) {//计算输出与标签的欧式距离 
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
	double Make(double t1, double tMax, double tMin)//对每项进行归一划 
	{
		return (t1 - tMin) / (tMax - tMin);
	}
	void readdata(int N, vector < vector<double> >& traindata, int& train_num, vector < vector<double> >& testdata, int& test_num)
	{//总数据集分化成训练集和测试集 
		train_num = 0, test_num = 0;
		traindata.clear();//初始化数组 
		testdata.clear();
		string csh, Nowcsh;//数组为string类型 

		for (int i = 1; i <= N; i++)
		{
			cin >> csh;
			for (int j = 0; j <= 2; j++)//花萼长度 以字符型读取小数，如5.1，用三个字节分别存储5和.和1 
				Nowcsh[j] = csh[j];
			Load[i].sp = atof(Nowcsh.c_str());//将读取的三个字节利用atof函数转换成小数类型 
			for (int j = 0; j <= 2; j++)//花萼宽度 
				Nowcsh[j] = csh[j + 4];
			Load[i].sw = atof(Nowcsh.c_str());
			for (int j = 0; j <= 2; j++)//花瓣长度 
				Nowcsh[j] = csh[j + 8];
			Load[i].pl = atof(Nowcsh.c_str());
			for (int j = 0; j <= 2; j++)//花瓣宽度 
				Nowcsh[j] = csh[j + 12];
			Load[i].pw = atof(Nowcsh.c_str());
			for (int j = 0; (csh[j + 17] >= 'a' && csh[j + 17] <= 'z') || (csh[j + 17] >= 'A' && csh[j + 17] <= 'Z'); j++)
				Load[i].Name[j] = csh[j + 16];//标签名称 
			Max.sp = max(Load[i].sp, Max.sp);
			Max.sw = max(Load[i].sw, Max.sw);
			Max.pl = max(Load[i].pl, Max.pl);
			Max.pw = max(Load[i].pw, Max.pw);
			Min.sp = min(Load[i].sp, Min.sp);
			Min.sw = min(Load[i].sw, Min.sw);
			Min.pl = min(Load[i].pl, Min.pl);
			Min.pw = min(Load[i].pw, Min.pw);
			Load[i].lab = (i - 1) / 50 + 1;//将标签进行1、2、3赋值 

		}
		for (int i = 1; i <= N; i++)
		{
			Load[i].sp = Make(Load[i].sp, Max.sp, Min.sp);//对每项进行归一化 
			Load[i].sw = Make(Load[i].sw, Max.sw, Min.sw);
			Load[i].pw = Make(Load[i].pw, Max.pw, Min.pw);
			Load[i].pl = Make(Load[i].pl, Max.pl, Min.pl);
		}
		for (int i = 1; i <= N; i++)//将总数据集分成训练集和测试集 
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

void MakeRand(Mat W[], Vec b[], int n)//初始化W和b为0~1  
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
	Mat W[2] = { Mat(4,Vec(4)),Mat(3,Vec(4)) };//W[0]为4*4矩阵用来储存输入层到隐含层权值 ，W[0]为3*4矩阵用来储存隐含层到输出层权值
	Vec b[2] = { Vec(4),Vec(3) };//b[0]为输入层到隐含层的偏置 ，b[1]为隐含层到输出层偏值

	srand(time(NULL));
	MakeRand(W, b, 2);//将W和b初始化为0~1 
	
	inputWork Now;//inputWork类 
	vector < vector<double> > Nowtraindata;//训练集 
	vector < vector<double> > Nowtestdata;//测试集 
	int train_num, test_num;
	freopen("iris.data", "r", stdin);//读入文件 
	Now.readdata(150, Nowtraindata, train_num, Nowtestdata, test_num);//读入150个数据，分化成训练集和测试集 
	
	int cnt = 0;//迭代轮数 
	int n1 = 4, n2 = 4, n3 = 3;//输入层n1为4个， 隐含层n2为4个，输出层n3为3个 
	double C = 0.001;//C为学习率 
	double error = inf;
	initgraph( 1000, 800);
	while (error > MIN_Error && cnt < MAX_Count) {//循环停止条件：错误率小于0.0001，训练轮数为10000 

		error = 0;//每轮迭代将损失率记为0 
		for (int i = 0; i < train_num; i++) {

			Vec tmp = Nowtraindata[i];
			tmp.erase(tmp.begin());//tmp数组的第一个值为种类 ，将其去掉
			Vec sita3(3, 0.0), sita2(4, 0.0), nowHideY(4, 0.0);//sita3为输出层的残差，sita2为隐含层的残差，nowHiderY为隐含层当前的计算值 
			Vec nowY = Forward(tmp, W, b, 2, nowHideY), realY(3, 0.0);//nowY储存前向传播的结果，realY为真实标签 
			realY[(int)Nowtraindata[i][0] - 1] = 1.0;//若第i个数据的真实分类是第j类，创建一个1*3数组记录，第j位为1其余为0 

			upd.getsita3(sita3, realY, nowY, n3);// 计算输出层的残差

			upd.getsita2(sita2, W[1], sita3, nowHideY, n2, n3);// 计算隐含层的残差

			upd.update(W[0], b[0], W[1], b[1], tmp, nowHideY, sita2, sita3, n1, n2, n3, C);//更新W和b 
			
			error += upd.getError(realY, nowY, n3);//计算每轮迭代的总损失率 
			
		}
		//作图
		char title[20]="损失率曲线";
		setfont(30,15,"宋体");
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
	for (int i = 0; i < test_num; i++)//进行结果测试 
	{
		Vec tmp = Nowtestdata[i], tmp2;//tmp储存测试集，tmp2储存输入层->隐含层的输出结果 
		tmp.erase(tmp.begin());//将tmp的种类标签擦去 
		Load = Forward(tmp, W, b, 2, tmp2);//tmp正向传播得对各个种类的预测值 

		if (Nowtestdata[i][0] == 1)//在测试集真实标签为种类1的情况下 
		{
			if (Load[0] > Load[1] && Load[0] > Load[2])//如果预测值在种类1取得最大值，则预测正确，个数加一 
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
	cout << 100.0 * Ans / test_num << "%" << endl;//输出正确率 

	return 0;
}
