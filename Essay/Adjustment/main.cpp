#include "Eigen/Eigen"

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace Eigen;
using namespace std;
class GetData
{
public:
    //建立需要用到的数组
    vector<double>               GaoCha;         //储存高差值
    vector<double>               Distance;       //储存距离值
    vector<string>               KownPoint;      //储存已知点的点名
    vector<string>               UnKownPoint;    //储存未知点的点名
    map<string, double>          Height;         //储存高程点名和对应的高程(未知点高程为-1)
    vector<pair<string, string>> Relationship;   //储存照准方向

public:
    GetData() {}
    ~GetData() {}

    void Estimation()   //计算最佳估值
    {
        vector<string>::iterator               p1;   //建立unknown的迭代器
        vector<string>::iterator               p2;   //建立known的迭代器
        vector<pair<string, string>>::iterator p3;
        map<string, double>::iterator          l_it1;   //建立map迭代器的指针，以用于返回值参与计算
        map<string, double>::iterator          l_it3;
        for (p1 = UnKownPoint.begin(); p1 != UnKownPoint.end(); p1++)
        {
            for (p3 = Relationship.begin(); p3 != Relationship.end(); p3++)
            {
                int flag = 0;   //设置flag使循环跳出
                if (*p1 == p3->first)
                {
                    for (l_it3 = Height.begin(); l_it3 != Height.end(); l_it3++)
                        if (l_it3->second != -1)
                        {
                            l_it1         = Height.find(*p1);   //用find函数在Height数组中寻找unknown和known数组中对应的名字的具体值
                            l_it1->second = l_it3->second - GaoCha[p3 - Relationship.begin()];
                            flag          = 1;
                            break;
                        }
                        else
                            continue;
                }
                else if (*p1 == p3->second)
                {
                    for (l_it3 = Height.begin(); l_it3 != Height.end(); l_it3++)
                        if (l_it3->second != -1)
                        {
                            l_it1         = Height.find(*p1);
                            l_it1->second = l_it3->second + GaoCha[p3 - Relationship.begin()];
                            flag          = 1;
                            break;
                        }
                        else
                            continue;
                }
                if (flag)
                    break;
            }
        }
    }

    void MatrixB(MatrixXd &B)   //计算并打印B矩阵
    {
        //B矩阵的计算方法是，让UnKnownPoint中的点依此遍历Pair，如果出
        //出现对应名字，且出现在first位置，则赋值1，在second位置，赋值
        //为-1，如果没有出现，赋值为0.
        int i = 0, j = 0;
        for (i = 0; i < GaoCha.size(); i++)
        {
            for (j = 0; j < UnKownPoint.size(); j++)
            {
                if (UnKownPoint[j] == Relationship[i].first)
                    B(i, j) = -1;
                else if (UnKownPoint[j] == Relationship[i].second)
                    B(i, j) = 1;
                else
                    B(i, j) = 0;
            }
        }
    }

    void MatrixL(VectorXd &L)   //计算并打印L矩阵
    {
        double *l = new double[GaoCha.size()];

        int                           i;
        map<string, double>::iterator l_it1;
        map<string, double>::iterator l_it2;
        for (i = 0; i < GaoCha.size(); i++)
        {
            l_it1 = Height.find(Relationship[i].first);
            l_it2 = Height.find(Relationship[i].second);
            l[i]  = GaoCha[i] + l_it1->second - l_it2->second;
        }
        for (i = 0; i < GaoCha.size(); i++)
            L(i) = l[i];
    }

    void MatrixP(MatrixXd &P)   //计算并打印P矩阵
    {
        int i = 0, j = 0;
        P(i, j)    = 0;
        double max = Distance[0];
        for (i = 0; i < Distance.size(); i++)
            if (Distance[i] > max)
                max = Distance[i];

        for (i = 0; i < Distance.size(); i++)
        {
            P(i, i) = max / Distance[i];
        }
    }
};
void foo(MatrixXf &m)
{
    Matrix3f m2 = Matrix3f::Zero(3, 3);
    m2(0, 0)    = 1;
    m           = m2;
}

int main()
{
    GetData Data1;
    //    Data1.GetHight();
    Data1.Height = {{"A", 5.530},
                    {"B", 8.220},
                    {"C", -1},
                    {"D", -1}};   //将未知点的值初始化为-1，以方便区分已知值

    Data1.KownPoint   = {"A", "B"};   //将录入的已知点点名存储到KownPoint
    Data1.UnKownPoint = {"C", "D"};   //将录入的未知点点名存储到UnKownPoint
    //        Data1.GetGaoCha();
    Data1.GaoCha = {1.157, 1.532, -2.025, -0.663, 0.498};
    //    Data1.GetRelationship();
    Data1.Relationship = {{"A", "D"},
                          {"D", "B"},
                          {"B", "C"},
                          {"C", "A"},
                          {"C", "D"}};
    //    Data1.GetDistant();
    Data1.Distance = {2, 2, 2, 2, 4};
    Data1.Estimation();
    map<string, double>::iterator p;

    cout << "平差结果：" << endl;
    cout << "待求量估值：" << endl;
    for (p = Data1.Height.begin(); p != Data1.Height.end(); p++)
        cout << p->first << " : " << p->second << endl;

    //创建一个B矩阵
    MatrixXd B(Data1.GaoCha.size(), Data1.UnKownPoint.size());
    B = MatrixXd::Zero(Data1.GaoCha.size(), Data1.UnKownPoint.size());

    //创建一个P矩阵
    MatrixXd P(Data1.Distance.size(), Data1.Distance.size());
    P = MatrixXd::Zero(Data1.Distance.size(), Data1.Distance.size());

    //创建一个L矩阵
    VectorXd L(Data1.GaoCha.size());

    //创建一个Nbb矩阵
    MatrixXd Nbb(Data1.UnKownPoint.size(), Data1.UnKownPoint.size());
    Nbb = MatrixXd::Zero(Data1.UnKownPoint.size(), Data1.UnKownPoint.size());

    //创建一个W矩阵
    MatrixXd W(Data1.GaoCha.size(), Data1.UnKownPoint.size());
    W = MatrixXd::Zero(Data1.GaoCha.size(), Data1.UnKownPoint.size());

    //创建一个x矩阵
    VectorXd x(Data1.UnKownPoint.size());

    //创建一个v矩阵
    VectorXd v(Data1.GaoCha.size());

    //创建一个justL矩阵
    VectorXd justL(Data1.GaoCha.size());

    //创建一个justX矩阵
    VectorXd justX(Data1.UnKownPoint.size());

    Data1.MatrixB(B);
    cout << "B矩阵是：" << endl
         << B << endl;

    Data1.MatrixL(L);
    cout << "L矩阵是：" << endl
         << L << endl;

    Data1.MatrixP(P);
    cout << "P矩阵是：" << endl
         << P << endl;

    Nbb = B.transpose() * P * B;
    cout << "Nbb是：" << endl
         << Nbb << endl;

    W = B.transpose() * P * L;
    cout << "W是：" << endl
         << W << endl;

    x = Nbb.inverse() * W;
    cout << "x是：" << endl
         << x << endl;

    v = B * x - L;
    cout << "改正数v是：" << endl
         << v << endl;

    justL = L + v;
    cout << "h的平差值是：" << endl
         << justL << endl;

    vector<string>::iterator      p1;
    map<string, double>::iterator l_it1;
    VectorXd                      NOjustX(Data1.UnKownPoint.size());
    for (p1 = Data1.UnKownPoint.begin(); p1 != Data1.UnKownPoint.end(); p1++)
    {
        l_it1                                   = Data1.Height.find(*p1);
        NOjustX(p1 - Data1.UnKownPoint.begin()) = l_it1->second;
    }
    justX = NOjustX + x;
    cout << "待求点的平差值是：" << endl
         << justX << endl;

    int    r = Data1.Height.size() - Data1.UnKownPoint.size();
    double a = v.transpose() * P * v;
    double c = sqrt(a / r);
    cout << "中误差是：" << endl
         << c << endl;
    getchar();
    return 0;
}
