/// Terminal emulator overview (on Win32):
///
/// - Start ConPTY session, setup pipes, start a shell process
///
/// - Create virtual screen buffer
///   (ScreenCell buffer[width_in_char][height_in_char])
///
/// - Read ConPTY output (async?)
///
/// - Write ConPTY input (async?)
///
/// - Render screen buffer (OpenGL)
///
/// Considerations:
///
/// - Configuration (lua/toml/etc)
/// - Resizing
/// - Scrolling
/// - Mouse click support
/// - Line wrapping via renderer

#include "def.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

// clang-format off
#include <glad/glad.h>
#include <gl/gl.h>
// clang-format on

static LRESULT CALLBACK
wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_KEYDOWN:
        LOG("WM_KEYDOWN: %llu", wparam);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_NCCALCSIZE: {
        if (!wparam) break;
        NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lparam;
        params->rgrc[0].top -= 7;  // Removes white stripe at the top (?)
    } break;
    default:
        break;
    }

    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

static HWND
init_window()
{
    WNDCLASSA wndclass     = { 0 };
    wndclass.style         = 0;
    wndclass.lpfnWndProc   = wndproc;
    wndclass.hInstance     = GetModuleHandleA(NULL);
    wndclass.hIcon         = 0;  // TODO: Window icon
    wndclass.hCursor       = 0;  // TODO: What is this?
    wndclass.lpszClassName = "Nzdxv001";

    if (!RegisterClassA(&wndclass)) {
        // TODO: Better error handling
        fprintf(stderr, "RegisterClass failed.\n");
        return 0;
    }

    // TODO: Configurable
    uint32_t windowstyles = WS_BORDER | WS_POPUP | WS_THICKFRAME | WS_VISIBLE;

    HWND hwnd = CreateWindowExA(0, wndclass.lpszClassName, "term", windowstyles,
                                CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL,
                                NULL, wndclass.hInstance, NULL);

    if (!hwnd || hwnd == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "CreateWindowEx failed.\n");
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);
    LOG("Created window");

    return hwnd;
}

static void
cleanup_window(HWND hwnd)
{
    LOG("Destroyed window");
    DestroyWindow(hwnd);
}

static int
init_gl(HWND hwnd, HDC* outhdc, HGLRC* outhglrc)
{
    HDC hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pixelformatdesc = { 0 };

    pixelformatdesc.nSize    = sizeof(pixelformatdesc);
    pixelformatdesc.nVersion = 1;

    pixelformatdesc.dwFlags =
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

    pixelformatdesc.iPixelType = PFD_TYPE_RGBA;
    pixelformatdesc.cColorBits = 32;
    pixelformatdesc.cDepthBits = 24;
    pixelformatdesc.iLayerType = PFD_MAIN_PLANE;

    int32_t pixelformat = ChoosePixelFormat(hdc, &pixelformatdesc);
    if (!pixelformat || !SetPixelFormat(hdc, pixelformat, &pixelformatdesc)) {
        fprintf(stderr, "SetPixelFormat failed.\n");
        return FALSE;
    }

    HGLRC glctx = wglCreateContext(hdc);
    wglMakeCurrent(hdc, glctx);

    if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress)) {
        fprintf(stderr, "gladLoadGLLoader failed.\n");
        return FALSE;
    }

    LOG("OpenGL version: %s", glGetString(GL_VERSION));

    *outhdc = hdc;
    *outhglrc = glctx;
    LOG("Created graphics context");

    return TRUE;
}

static void
cleanup_gl(HWND hwnd, HDC* hdc, HGLRC* hglrc)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(*hglrc);
    ReleaseDC(hwnd, *hdc);

    LOG("Destroyed graphics context");
}

// TODO: This becomes WinMain when we have a render accessible
// log method
int
main(int argc, char** argv)
{
    HWND hwnd = init_window();

    HDC hdc = 0;
    HGLRC hglrc = 0;
    if (!init_gl(hwnd, &hdc, &hglrc)) {
        return -1;
    }

    // Message pump
    MSG msg = { 0 };
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        // Render
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SwapBuffers(hdc);
    }

    cleanup_gl(hwnd, &hdc, &hglrc);

    cleanup_window(hwnd);

    return 0;
}
