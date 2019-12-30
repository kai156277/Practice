#include <Eigen/Eigen>
#include <QDebug>
#include <QVector>
#include <utility>

#include <iostream>

using namespace Eigen;
using namespace std;

struct XYZ
{
    double x;
    double y;
    double z;
};

const XYZ operator-(const XYZ &left, const XYZ &right)
{
    return {left.x - right.x, left.y - right.y, left.z - right.z};
}

XYZ &operator-=(XYZ &left, const XYZ &right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    return left;
}

const XYZ operator+(const XYZ &left, const XYZ &right)
{
    return {left.x + right.x, left.y + right.y, left.z + right.z};
}

XYZ operator+=(const XYZ &left, const XYZ &right)
{
    qDebug() << "+=";
    qDebug() << QString("l: %1, %2, %3").arg(left.x).arg(left.y).arg(left.z);
    qDebug() << QString("r: %1, %2, %3").arg(right.x).arg(right.y).arg(right.z);
    return left + right;
}

int main(int argc, char *argv[])
{
    MatrixXd P = MatrixXd::Identity(10, 10);
    MatrixXd P1(10, 10);

    P1.setIdentity();

    cout << "P: \n"
         << P << endl;
    cout << "P1: \n"
         << P1 << endl;
}
