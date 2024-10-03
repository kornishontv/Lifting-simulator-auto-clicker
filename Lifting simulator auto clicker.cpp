#include <iostream>

#include <windows.h>

using namespace std;

void mouseGoto(int x, int y) {
    POINT mousepos;

    GetCursorPos(&mousepos);

    while (mousepos.x != x || mousepos.y != y) {
        GetCursorPos(&mousepos);

        int dx = x - mousepos.x;

        if (dx > 0) {
            dx = 1;
        }

        else if (dx < 0) {
            dx = -1;
        }

        int dy = y - mousepos.y;

        if (dy > 0) {
            dy = 1;
        }

        else if (dy < 0) {
            dy = -1;
        }

        mousepos.x += dx;

        mousepos.y += dy;

        INPUT Input[1] = { 0 };

        Input[0].type = INPUT_MOUSE;

        Input[0].mi.dwFlags = MOUSEEVENTF_MOVE;

        Input[0].mi.dx = dx;

        Input[0].mi.dy = dy;

        SendInput(1, Input, sizeof(INPUT));

        Sleep(10);
    }
}

int main() {
    POINT instrumentpos;

    HDC hdc = GetDC(0);

    int sr = 0, sg = 0, sb = 0;

    while (!(GetAsyncKeyState('S') < 0)) {
        GetCursorPos(&instrumentpos);

        COLORREF color = GetPixel(hdc, instrumentpos.x, instrumentpos.y);

        sr = color & 0xff;

        sg = (color >> 8) & 0xff;

        sb = (color >> 16) & 0xff;

        cout << sr << ", " << sg << ", " << sb << endl;
    }

    INPUT Inputs[2] = { 0 };

    Inputs[0].type = INPUT_MOUSE;

    Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    Inputs[1].type = INPUT_MOUSE;

    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    bool bStopped = false;

    while (!(GetAsyncKeyState(VK_ESCAPE) < 0)) {
        if (GetAsyncKeyState('P') & 1) {
            bStopped = !bStopped;
        }

        if (bStopped) continue;

        COLORREF color = GetPixel(hdc, instrumentpos.x, instrumentpos.y);

        int r = color & 0xff;

        int g = (color >> 8) & 0xff;

        int b = (color >> 16) & 0xff;

        if (r == sr && g == sg && b == sb) {
            int destx = instrumentpos.x + 30;

            int desty = instrumentpos.y - 60;

            mouseGoto(destx, desty);

            SendInput(2, Inputs, sizeof(INPUT));

            Sleep(1000.0f);
        }

        else {
            POINT mousepos;

            GetCursorPos(&mousepos);

            int destx = instrumentpos.x + 30;

            int desty = instrumentpos.y + 30;

            mouseGoto(destx, desty);

            SendInput(2, Inputs, sizeof(INPUT));

            Sleep(1000.0f);
        }
    }
}