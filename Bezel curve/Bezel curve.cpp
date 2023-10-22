// Bezel curve.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Bezel curve.h"
#include<stdio.h>
#define MAX_LOADSTRING 100
#define _CRT_SECURE_NO_WARNINGS


// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BEZELCURVE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BEZELCURVE));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BEZELCURVE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BEZELCURVE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//


// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{   
    static TCHAR szAppName[] = TEXT("BezierCurve");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }
    hwnd = CreateWindow(szAppName, TEXT("Bezier Curve"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int cxClient, cyClient;
    static POINT apt[4];
    static int iCtrlPt = -1;
    FILE* fp;
    fopen_s(&fp,"init.txt", "r");
    static int x1, y1, x2, y2, x3, y3, x4, y4;
    fscanf_s(fp, "%d %d %d %d %d %d %d %d", &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
    fclose(fp);

    switch (message)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        apt[0].x = x1+cxClient / 3;
        apt[0].y = y1+2*cyClient / 3;
        apt[1].x = x2+cxClient / 3;
        apt[1].y = y2+cyClient / 4;
        apt[2].x = x3+2*cxClient / 3;
        apt[2].y = y3+cyClient / 4;
        apt[3].x = x4+2*cxClient / 3;
        apt[3].y = y4+2*cyClient / 3;
        return 0;
    case WM_PAINT:
    {
        HDC hdc;
        PAINTSTRUCT ps;
        HPEN hPen;
        hdc = BeginPaint(hwnd, &ps);
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        SelectObject(hdc, hPen);
        PolyBezier(hdc, apt, 4);
        DeleteObject(hPen);
        for (int i = 0; i < 4; i++)
        {
            hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            SelectObject(hdc, hPen);
            Ellipse(hdc, apt[i].x - 4, apt[i].y - 4, apt[i].x + 4, apt[i].y + 4);
            DeleteObject(hPen);
        }
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        for (int i = 0; i < 4; i++)
        {
            if (x >= apt[i].x - 4 && x <= apt[i].x + 4 && y >= apt[i].y - 4 && y <= apt[i].y + 4)
            {
                iCtrlPt = i;
                break;
            }
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {
        iCtrlPt = -1;
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (iCtrlPt != -1)
        {
            apt[iCtrlPt].x = LOWORD(lParam);
            apt[iCtrlPt].y = HIWORD(lParam);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}