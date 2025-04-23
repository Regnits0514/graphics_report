#include <windows.h>
#include <GL/gl.h>
#include <cmath>
#pragma comment(lib, "opengl32.lib")

LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(h, m, w, l);
}

float rainbow(float t, float phase) {
    return 0.5f + 0.5f * sinf(t + phase);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int) {
    WNDCLASS wc = { CS_OWNDC, WndProc, 0, 0, h, 0, 0, 0, 0, L"GL" };
    RegisterClass(&wc);
    HWND win = CreateWindow(L"GL", 0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 600, 0, 0, h, 0);
    HDC dc = GetDC(win);

    PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER };
    SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
    HGLRC rc = wglCreateContext(dc);
    wglMakeCurrent(dc, rc);

    glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(-1, 1, -1, 1, -1, 1);

    MSG msg = {};
    float t = 0.0f;
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) DispatchMessage(&msg);

        glClear(GL_COLOR_BUFFER_BIT);

        // 무지개 색상 계산
        float r = rainbow(t, 0.0f);
        float g = rainbow(t, 2.0f);
        float b = rainbow(t, 4.0f);
        glColor3f(r, g, b);

        glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f); glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f); glVertex2f(-0.5f, 0.5f);
        glEnd();

        SwapBuffers(dc);
        Sleep(16);
        t += 0.05f; // 색상 변화 속도
    }

    wglMakeCurrent(0, 0); wglDeleteContext(rc); ReleaseDC(win, dc); DestroyWindow(win);
    return 0;
}
