#include "stdafx.h"
#include "Circle.h"
#include "Line.h"
#include "Shapes.h"
#include "Polylines.h"
#include "Ellipses.h"

Shapes shapes;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Graphics graphics(hdc);
        graphics.Clear(Color(255, 255, 255));

        Shapes* data = reinterpret_cast<Shapes*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        drawCircle(&graphics, data->circles);
        drawPolylines(&graphics, data->polylines);
        drawLine(&graphics, data->lines);
        drawEllipses(&graphics, data->ellipses);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AllocConsole();
    FILE* in;
    FILE* out;
    freopen_s(&in, "CONIN$", "r", stdin);
    freopen_s(&out, "CONOUT$", "w", stdout);

    const string file = "sample.svg";

    shapes.circles = parseCircle(file);
    shapes.polylines = parsePolylines(file);
    shapes.lines = parseLine(file);
    shapes.ellipses = parseEllipses(file);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GettingStarted";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        L"GettingStarted",
        L"Circle and Line SVG",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&shapes);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}
