#include <QCoreApplication>
#include <QDebug>
#include <Windows.h>
#include <iostream>

using namespace std;
#pragma comment(lib, "version.lib")
std::wstring GetSoftVersion(const wchar_t *exepath)
{
    std::wstring r = L"";
    if (!exepath)
        return r;
    UINT sz = GetFileVersionInfoSizeW(exepath, 0);
    if (sz != 0)
    {
        r.resize(sz, 0);
        wchar_t *pBuf = NULL;
        pBuf          = new wchar_t[sz];
        VS_FIXEDFILEINFO *pVsInfo;
        if (GetFileVersionInfoW(exepath, 0, sz, pBuf))
        {
            if (VerQueryValueW(pBuf, L"\\", (void **) &pVsInfo, &sz))
            {
                swprintf(pBuf, sz, L"%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
                r = pBuf;
            }
        }
        delete pBuf;
    }
    return r;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString          appName = QCoreApplication::applicationFilePath();
    qDebug() << "Version: " << QCoreApplication::applicationVersion();
    std::wstring winfo = GetSoftVersion(appName.toStdWString().data());
    std::wcout << winfo << std::endl;
    std::wstring info = GetSoftVersion(L"getVersionInfo.exe");
    qDebug() << "Info:" << QString::fromStdWString(info);

    return a.exec();
}
