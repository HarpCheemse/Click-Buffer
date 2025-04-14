#include <windows.h>
#include <commctrl.h> 
#include <stdio.h>
#include "include/ClickBuffer.h"
#include "include/resources.h"

#define BUTTON_GITHUB 100
#define BUTTON_EXIT 101
#define ID_LABEL 102
#define ID_TRACKBAR 103

static HWND hTrack_left_click = NULL;
static HWND hTrack_right_click = NULL;
HBRUSH hWhiteBrush;
HFONT hFont13, hFont16;

char label_text[100];
int level_cps[6] = {7,14,18,24,30,40};
BITMAP bmp;
HBITMAP hBitmap = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
switch (msg) 
{
     case WM_CREATE:

     hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
     hFont13 = CreateFont(
          13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
          DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
  
     hFont16 = CreateFont(
          16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
          DEFAULT_PITCH | FF_DONTCARE, "Segoe UI"); 

     CreateWindow("BUTTON", "",
          WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
          10, 10, 230, 150,
          hwnd, NULL, NULL, NULL);

     HWND hLabel_1 = CreateWindow("STATIC", "Left Click Buffer",
          WS_VISIBLE | WS_CHILD,
          20, 10, 90, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);      
          SendMessage(hLabel_1, WM_SETFONT, (WPARAM)hFont13, TRUE); 

     HWND hLabel_2 = CreateWindow("STATIC", "Left Click Buffer Rate",
          WS_VISIBLE | WS_CHILD,
          20, 30, 150, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);
          SendMessage(hLabel_2, WM_SETFONT, (WPARAM)hFont13, TRUE);

     hTrack_left_click = CreateWindowEx(0, TRACKBAR_CLASS, "",           //display left click slide
                              WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS,
                              55, 50, 150, 30,
                              hwnd, (HMENU)ID_TRACKBAR, NULL, NULL);
     SendMessage(hTrack_left_click, TBM_SETRANGE, TRUE, MAKELPARAM(0, 5));
     SendMessage(hTrack_left_click, TBM_SETPOS, TRUE, 1);

     HWND hLabel_3 = CreateWindow("STATIC", "Level:",
          WS_VISIBLE | WS_CHILD,
          22, 50, 30, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);
          SendMessage(hLabel_3, WM_SETFONT, (WPARAM)hFont13, TRUE);

     for (int i = 0; i <= 5; i++) 
     {            //display 0 to 5 in the slide
          char buf[2];
          buf[0] = '0' + i;
          buf[1] = '\0';
      
          int x = 65 + i * 25;
          HWND hLabel_100 = CreateWindow("STATIC",
              buf,
              WS_VISIBLE | WS_CHILD,
              x, 85, 20, 20,
              hwnd, NULL, NULL, NULL);
              SendMessage(hLabel_100, WM_SETFONT, (WPARAM)hFont13, TRUE); 
     }

     sprintf(label_text, "Your Average 7 CPS Is Now %d CPS", level_cps[LEFT_CLICK_BUFFER_RATE]);
     HWND hLabel_7 = CreateWindow("STATIC", label_text, 
          WS_VISIBLE | WS_CHILD,
          20, 120, 200, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);
          SendMessage(hLabel_7, WM_SETFONT, (WPARAM)hFont16, TRUE);

     sprintf(label_text, "Your Average 7 CPS Is Now %d CPS", level_cps[RIGHT_CLICK_BUFFER_RATE]);
     HWND hLabel_8 = CreateWindow("STATIC", label_text, 
          WS_VISIBLE | WS_CHILD,
          20, 290, 200, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);
          SendMessage(hLabel_8, WM_SETFONT, (WPARAM)hFont16, TRUE);

     // RIGHT CLICK
     CreateWindow("BUTTON", "",
          WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
          10, 180, 230, 150,
          hwnd, NULL, NULL, NULL);

     HWND hLabel_4 = CreateWindow("STATIC", "Right Click Buffer",
          WS_VISIBLE | WS_CHILD,
          20, 180, 90, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);      
          SendMessage(hLabel_4, WM_SETFONT, (WPARAM)hFont13, TRUE); 

     HWND hLabel_5 = CreateWindow("STATIC", "Right Click Buffer Rate",
          WS_VISIBLE | WS_CHILD,
          20, 200, 150, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);
          SendMessage(hLabel_5, WM_SETFONT, (WPARAM)hFont13, TRUE);

     hTrack_right_click = CreateWindowEx(0, TRACKBAR_CLASS, "",        //Right Click Slide
          WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS,
          55, 220, 150, 30,
          hwnd, (HMENU)ID_TRACKBAR, NULL, NULL);
          SendMessage(hTrack_right_click, TBM_SETRANGE, TRUE, MAKELPARAM(0, 5));
          SendMessage(hTrack_right_click, TBM_SETPOS, TRUE, 1);

     HWND hLabel_6 = CreateWindow("STATIC", "Level:",
          WS_VISIBLE | WS_CHILD,
          22, 220, 30, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);
          SendMessage(hLabel_6, WM_SETFONT, (WPARAM)hFont13, TRUE);

     for (int i = 0; i <= 5; i++) 
     {
          char buf[2];
          buf[0] = '0' + i;
          buf[1] = '\0';
      
          int x = 65 + i * 25;
          HWND hLabel_101 = CreateWindow("STATIC",
              buf,
              WS_VISIBLE | WS_CHILD,
              x, 255, 20, 20,
              hwnd, NULL, NULL, NULL);
              SendMessage(hLabel_101, WM_SETFONT, (WPARAM)hFont13, TRUE); 
     }

     HWND hLabel_9 = CreateWindow("STATIC", "Left Click",
          WS_VISIBLE | WS_CHILD,
          310, 143, 50, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);
          SendMessage(hLabel_9, WM_SETFONT, (WPARAM)hFont13, TRUE);

     HWND hLabel_10 = CreateWindow("STATIC", "Right Click",
          WS_VISIBLE | WS_CHILD,
          310, 313, 55, 15,
          hwnd, (HMENU)ID_LABEL, NULL, NULL);
          SendMessage(hLabel_10, WM_SETFONT, (WPARAM)hFont13, TRUE);
          
     HWND github = CreateWindow("BUTTON", "GitHub", 
          WS_VISIBLE | WS_CHILD | BS_FLAT, 
          260, 340, 70, 20, 
          hwnd, (HMENU)BUTTON_GITHUB, NULL, NULL);
          SendMessage(github, WM_SETFONT, (WPARAM)hFont13, TRUE);

     HWND exit = CreateWindow("BUTTON", "Exit", 
          WS_VISIBLE | WS_CHILD | BS_FLAT, 
          340, 340, 70, 20, 
          hwnd, (HMENU)BUTTON_EXIT, NULL, NULL);
          SendMessage(exit, WM_SETFONT, (WPARAM)hFont13, TRUE);

     // image
     CreateWindow("BUTTON", "",
          WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
          260, 10, 150, 150,
          hwnd, NULL, NULL, NULL);

     CreateWindow("BUTTON", "",
          WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
          260, 180, 150, 150,
          hwnd, NULL, NULL, NULL);

     HBITMAP hLeftClickBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LEFT_CLICK));     //load left_click.bmp
     HWND hImage_Left_Click = CreateWindow("STATIC", NULL,
          WS_VISIBLE | WS_CHILD | SS_BITMAP,
          270, 30, 100, 100, // Set reasonable width and height
          hwnd, NULL, GetModuleHandle(NULL), NULL);
          SendMessage(hImage_Left_Click, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLeftClickBmp);

     HBITMAP hRightClickBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_RIGHT_CLICK));   //load right_click.bmp
     HWND hImage_Right_Click = CreateWindow("STATIC", NULL,
          WS_VISIBLE | WS_CHILD | SS_BITMAP,
          270, 200, 100, 100, // Set reasonable width and height
          hwnd, NULL, GetModuleHandle(NULL), NULL);
          SendMessage(hImage_Right_Click, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hRightClickBmp);

     case WM_HSCROLL:
     {
          if ((HWND)lParam == hTrack_left_click)
          {
               LEFT_CLICK_BUFFER_RATE = SendMessage(hTrack_left_click, TBM_GETPOS, 0, 0);                           //modify left click slide
               sprintf(label_text, "Your Average 7 CPS Is Now %d CPS", level_cps[LEFT_CLICK_BUFFER_RATE]);
               hLabel_7 = CreateWindow("STATIC", label_text, 
                    WS_VISIBLE | WS_CHILD,
                    20, 120, 200, 15,
                    hwnd, (HMENU)ID_LABEL, NULL, NULL);
                    SendMessage(hLabel_7, WM_SETFONT, (WPARAM)hFont16, TRUE);
          }
          if ((HWND)lParam == hTrack_right_click) 
          {
               RIGHT_CLICK_BUFFER_RATE = SendMessage(hTrack_right_click, TBM_GETPOS, 0, 0);
               sprintf(label_text, "Your Average 7 CPS Is Now %d CPS", level_cps[RIGHT_CLICK_BUFFER_RATE]);         //modify right click slide
               hLabel_8 = CreateWindow("STATIC", label_text, 
                    WS_VISIBLE | WS_CHILD,
                    20, 290, 200, 15,
                    hwnd, (HMENU)ID_LABEL, NULL, NULL);
                    SendMessage(hLabel_8, WM_SETFONT, (WPARAM)hFont16, TRUE);
          }
          break;
     }
             
     case WM_CTLCOLORSTATIC:
          HDC hdcStatic = (HDC)wParam;
          SetBkMode(hdcStatic, TRANSPARENT); 
          SetTextColor(hdcStatic, RGB(0, 0, 0));
          return (INT_PTR)hWhiteBrush;
          
     case WM_COMMAND:

          if (LOWORD(wParam) == BUTTON_GITHUB) ShellExecute(hwnd, "open", "https://github.com/HarpCheemse/Click-Buffer-Wind32-GUI-", NULL, NULL, SW_SHOWNORMAL);
          if (LOWORD(wParam) == BUTTON_EXIT) PostQuitMessage(0);
          break;
     case WM_DESTROY:
          PostQuitMessage(0);
          break;

     default:
          return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow) {
     (void)hPrevInst;
     (void)args;
     int width = 425;
     int height = 400;
     int screenWidth = GetSystemMetrics(SM_CXSCREEN);
     int screenHeight = GetSystemMetrics(SM_CYSCREEN);
     int top_left_cord_x = (screenWidth - width) / 2;
     int top_left_cord_y = (screenHeight - height) / 2;

     startClickThreads();
     INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_BAR_CLASSES };
     InitCommonControlsEx(&icex); // For slider

     WNDCLASSEX wc = {0};
     wc.cbSize = sizeof(WNDCLASSEX);
     wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
     wc.hInstance = hInst;
     wc.lpszClassName = "MouseApp";
     wc.lpfnWndProc = WndProc;
     wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
     wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));

     RegisterClassEx(&wc);

     HWND hwnd = CreateWindow("MouseApp", "Click Buffer",
                              WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                             top_left_cord_x, top_left_cord_y, width, height,
                             NULL, NULL, hInst, NULL);
     ShowWindow(hwnd, nCmdShow);
     SetWindowPos(hwnd, NULL, 0, 0, 0, 0, 
          SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
     SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)wc.hIcon);
     MSG msg = {0};
     while (GetMessage(&msg, NULL, 0, 0)) {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
     }
     return 0;
}
