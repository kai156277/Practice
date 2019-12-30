
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define RE_WGS84 6378137.0             /* earth semimajor axis (WGS84) (m) */
#define FE_WGS84 (1.0 / 298.257223563) /* earth flattening (WGS84) */
#define PI 3.1415926535897932          /* pi */
#define DEG2RAD 0.017453292519943295
#define RAD2DEG 57.29577951308232087

void ecef2pos(const double *r, double *pos);
void pos2ecef(const double *pos, double *r);
void ecef2enu(const double *pos, const double *r, double *e);

int main()
{
    double *pos = new double[3], *r = new double[3],
           *e = new double[3], *xyz = new double[3];
    xyz[0] = -2197954.4434, xyz[1] = 5180834.9628, xyz[2] = 2991568.4824;   //需要修改
    ecef2pos(xyz, pos);                                                     //xyz转blh
    printf("pos: %.9f, %.9f, %.9f \n", pos[0], pos[1], pos[2]);
    printf("pos: %.9f, %.9f, %.9f \n", (pos[0] * RAD2DEG), (pos[1] * RAD2DEG), pos[2]);

    FILE *fpRead  = fopen("C:\\Users\\zhao\\Desktop\\spp.txt", "r");     //需要修改
    FILE *fpWrite = fopen("C:\\Users\\zhao\\Desktop\\enu1.txt", "w+");   //需要修改

    char StrLine[1024];   //每行最大读取的字符数
    if (fpRead == NULL)
    {
        printf("fpread open failed！");
        return -1;
    }

    if (fpWrite == NULL)
    {
        printf("fpWrite open failed！");
        return -1;
    }
    double degBLH[3]   = {0};
    double radBLH[3]   = {0};
    double ecefXYZ[3]  = {0};
    double dECEFXYZ[3] = {0};
    double enu[3]      = {0};
    while (!std::feof(fpRead))
    {
        r[0] = 0, r[1] = 0, r[2] = 0;
        e[0] = 0, e[1] = 0, e[2] = 0;
        fgets(StrLine, 1024, fpRead);   //读取一行
        char *temp = new char[1024];
        // 获取原始数据
        strncpy(temp, StrLine + 25, 12);
        degBLH[0] = atof(temp);
        radBLH[0] = degBLH[0] * DEG2RAD;
        strncpy(temp, StrLine + 40, 13);
        degBLH[1] = atof(temp);
        radBLH[1] = degBLH[1] * DEG2RAD;
        strncpy(temp, StrLine + 56, 8);
        degBLH[2] = atof(temp);
        radBLH[2] = degBLH[2];
        printf("read blh %15.9f %15.9f %15.9f \n", degBLH[0], degBLH[1], degBLH[2]);
        printf("read xyz %15.9f %15.9f %15.9f \n", radBLH[0], radBLH[1], radBLH[2]);
        // blh(rad) 转换至 ecef 的xyz
        pos2ecef(radBLH, ecefXYZ);
        printf("pos2ecef %15.9f %15.9f %15.9f \n", ecefXYZ[0], ecefXYZ[1], ecefXYZ[2]);
        dECEFXYZ[0] = xyz[0] - ecefXYZ[0];
        dECEFXYZ[1] = xyz[1] - ecefXYZ[1];
        dECEFXYZ[2] = xyz[2] - ecefXYZ[2];
        // dxyz
        printf("ddECEF %15.9f %15.9f %15.9f \n", dECEFXYZ[0], dECEFXYZ[1], dECEFXYZ[2]);
        ecef2enu(pos, dECEFXYZ, enu);
        //printf("%f %f %f\n", *e, *(e + 1), *(e + 2));
        fprintf(fpWrite, "%13.6f %13.6f %13.6f\n", enu[0], enu[1], enu[2]);
        delete[] temp;
        temp = NULL;
    }
    delete[] pos;
    delete[] r;
    delete[] e;
    delete[] xyz;
    pos = r = e = xyz = NULL;
    std::fclose(fpRead);    //关闭文件
    std::fclose(fpWrite);   //关闭文件
    printf("END!\n");
    system("pause");
    return 0;
}

void xyz2enu(const double *pos, double *E)
{
    double sinp = sin(pos[0]), cosp = cos(pos[0]), sinl = sin(pos[1]), cosl = cos(pos[1]);

    E[0] = -sinl;
    E[3] = cosl;
    E[6] = 0.0;
    E[4] = -sinp * sinl;
    E[7] = cosp;
    E[2] = cosp * cosl;
    E[5] = cosp * sinl;
    E[8] = sinp;
}
void matmul(const char *tr, int n, int k, int m, double alpha, const double *A, const double *B, double beta, double *C)
{
    double d;
    int    i, j, x, f = tr[0] == 'N' ? (tr[1] == 'N' ? 1 : 2) : (tr[1] == 'N' ? 3 : 4);

    for (i = 0; i < n; i++)
        for (j = 0; j < k; j++)
        {
            d = 0.0;
            switch (f)
            {
            case 1:
                for (x = 0; x < m; x++)
                    d += A[i + x * n] * B[x + j * m];
                break;
            case 2:
                for (x = 0; x < m; x++)
                    d += A[i + x * n] * B[j + x * k];
                break;
            case 3:
                for (x = 0; x < m; x++)
                    d += A[x + i * m] * B[x + j * m];
                break;
            case 4:
                for (x = 0; x < m; x++)
                    d += A[x + i * m] * B[j + x * k];
                break;
            }
            if (beta == 0.0)
                C[i + j * n] = alpha * d;
            else
                C[i + j * n] = alpha * d + beta * C[i + j * n];
        }
}

void ecef2enu(const double *pos, const double *r, double *e)
{
    double E[9];

    xyz2enu(pos, E);
    matmul("NN", 3, 1, 3, 1.0, E, r, 0.0, e);
}

double dot(const double *a, const double *b, int n)
{
    double c = 0.0;

    while (--n >= 0)
        c += a[n] * b[n];
    return c;
}

void ecef2pos(const double *r, double *pos)
{
    double e2 = FE_WGS84 * (2.0 - FE_WGS84), r2 = dot(r, r, 2), z, zk, v = RE_WGS84, sinp;

    for (z = r[2], zk = 0.0; fabs(z - zk) >= 1E-4;)
    {
        zk   = z;
        sinp = z / sqrt(r2 + z * z);
        v    = RE_WGS84 / sqrt(1.0 - e2 * sinp * sinp);
        z    = r[2] + v * e2 * sinp;
    }
    pos[0] = r2 > 1E-12 ? atan(z / sqrt(r2)) : (r[2] > 0.0 ? PI / 2.0 : -PI / 2.0);
    pos[1] = r2 > 1E-12 ? atan2(r[1], r[0]) : 0.0;
    pos[2] = sqrt(r2 + z * z) - v;
}

void pos2ecef(const double *pos, double *r)
{
    double sinp = sin(pos[0]), cosp = cos(pos[0]), sinl = sin(pos[1]), cosl = cos(pos[1]);
    double e2 = FE_WGS84 * (2.0 - FE_WGS84), v = RE_WGS84 / sqrt(1.0 - e2 * sinp * sinp);

    r[0] = (v + pos[2]) * cosp * cosl;
    r[1] = (v + pos[2]) * cosp * sinl;
    r[2] = (v * (1.0 - e2) + pos[2]) * sinp;
}
