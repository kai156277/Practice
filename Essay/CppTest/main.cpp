#include <iomanip>
#include <iostream>
#include <time.h>
#include <windows.h>
using namespace std;

int main(int argc, char *argv[])
{
    LARGE_INTEGER timeStart;
    LARGE_INTEGER timeEnd;
    LARGE_INTEGER frequency;
    double        quadpart;
    double        c;
    double        a;
    double        b;

    for (int out = 0; out < 10; ++out)
    {
        c = 0;
        a = 1.111111111111111;
        QueryPerformanceFrequency(&frequency);
        quadpart = (double) frequency.QuadPart;
        QueryPerformanceCounter(&timeStart);
        for (int i = 0; i < 2e9; ++i)
        {
            c += a;
        }
        QueryPerformanceCounter(&timeEnd);
        cout << setprecision(10) << setw(16) << (timeEnd.QuadPart - timeStart.QuadPart) / quadpart << ';';

        c = 1;
        a = 1.111111111111111;
        QueryPerformanceFrequency(&frequency);
        quadpart = (double) frequency.QuadPart;
        QueryPerformanceCounter(&timeStart);
        for (int i = 0; i < 2e9; ++i)
        {
            c *= a;
        }
        QueryPerformanceCounter(&timeEnd);
        cout << setprecision(10) << setw(16) << (timeEnd.QuadPart - timeStart.QuadPart) / quadpart << ';';

        c = 1e10;
        a = 1.111111111111111;
        QueryPerformanceFrequency(&frequency);
        quadpart = (double) frequency.QuadPart;
        QueryPerformanceCounter(&timeStart);
        for (int i = 0; i < 2e9; ++i)
        {
            c /= a;
        }
        QueryPerformanceCounter(&timeEnd);
        cout << setprecision(10) << setw(16) << (timeEnd.QuadPart - timeStart.QuadPart) / quadpart << ';';

        QueryPerformanceFrequency(&frequency);
        quadpart = (double) frequency.QuadPart;
        QueryPerformanceCounter(&timeStart);
        for (int i = 0; i < 2e9; ++i)
        {
            ;
        }
        QueryPerformanceCounter(&timeEnd);
        cout << setprecision(10) << setw(16) << (timeEnd.QuadPart - timeStart.QuadPart) / quadpart << endl;
    }

    cout << "All done!" << endl;

    system("pause");

    return 0;
}
