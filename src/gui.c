#include <windows.h>
#include <commctrl.h> 
#include <stdio.h>
#include "include/ClickBuffer.h"
#include "include/resources.h"

#define ID_BUTTON_GITHUB 100
#define ID_BUTTON_EXIT 101
#define ID_LABEL 102
#define ID_TRACKBAR 103
#define ID_BUTTON 104

static HWND hTrack_left_click = NULL;
static HWND hTrack_right_click = NULL;
HBRUSH hWhiteBrush;
HFONT hFont13_NORMAL, hFont16_BOLD, hFont13_BOLD, hFont16_NORMAL;

char current_leftclick_cps[100];
char current_rightclick_cps[100];

int level_cps[6] = {7,14,18,24,30,40};

HWND hLabel_Current_LeftClick_CPS;
HWND hLabel_Current_RightClick_CPS;

HFONT createUIFont(int height, int weight, LPCSTR font) 
{
    return CreateFont(
        height, 0, 0, 0,
        weight, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, font
    );
}
HWND createLabel(HWND parent, const char* text, int x, int y, int w, int h, DWORD style, int id, HFONT font)
{
    HWND hwndLabel = CreateWindow("STATIC", text,
        WS_VISIBLE | WS_CHILD | style,
        x, y, w, h,
        parent, (HMENU)(intptr_t)id, NULL, NULL);
    SendMessage(hwndLabel, WM_SETFONT, (WPARAM)font, TRUE);
    return hwndLabel;
}
HWND createButton(HWND parent, const char* text, int x, int y, int w, int h, DWORD style, int id, HFONT font)
{
    HWND hwndButton = CreateWindow("BUTTON", text,
        WS_VISIBLE | WS_CHILD | style,
        x,y,w,h,
        parent, (HMENU)(intptr_t)id, NULL, NULL);
        SendMessage(hwndButton, WM_SETFONT, (WPARAM)font, TRUE);
        return hwndButton;
}
HWND createSlide(HWND parent, const char* text, int x, int y, int w, int h, DWORD style, int id, int start, int end, int pos)
{
     HWND hwndSlide = CreateWindowEx(0, TRACKBAR_CLASS, text,
     WS_VISIBLE | WS_CHILD | style,
     x, y, w, h,
     parent, (HMENU)(intptr_t)id, NULL, NULL);
     SendMessage(hwndSlide, TBM_SETRANGE, TRUE, MAKELPARAM(start, end));
     SendMessage(hwndSlide, TBM_SETPOS, TRUE, pos);
     return hwndSlide;
}
HWND displayBitMap(HWND parent, int x, int y, int w, int h, int id)
{
    HBITMAP load_bitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(id));
    HWND image = CreateWindow("STATIC", NULL,
        WS_VISIBLE | WS_CHILD | SS_BITMAP,
        x, y, w, h,
        parent, NULL, GetModuleHandle(NULL), NULL);
        SendMessage(image, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)load_bitmap);
    return image;
}
void createSlideNumber(HWND parent, int start, int end, int x, int y, int w, int h, int jump, HFONT font)
{
     for (int i = start; i <= end; i++) 
     {
          char buffer[50];
          sprintf(buffer, "%d", i);
          int temp = x + i * jump;
          HWND hLabel_100 = CreateWindow("STATIC", buffer,
              WS_VISIBLE | WS_CHILD,
              temp, y, w, h,
              parent, NULL, NULL, NULL);
              SendMessage(hLabel_100, WM_SETFONT, (WPARAM)font, TRUE); 
     }
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
switch (msg) 
{
     case WM_CREATE:

     hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
     hFont13_NORMAL = createUIFont(13, FW_NORMAL, "Segeo UI");
     hFont13_BOLD = createUIFont(13, FW_BOLD, "Segeo UI");
     hFont16_NORMAL = createUIFont(16, FW_NORMAL, "Segeo UI");
     hFont16_BOLD = createUIFont(16, FW_BOLD, "Segeo UI");

     //Group_Box
     createButton(hwnd, "", 10, 10, 230, 150, BS_GROUPBOX, ID_BUTTON, hFont13_NORMAL);
     createButton(hwnd, "", 10, 180, 230, 150, BS_GROUPBOX, ID_BUTTON, hFont13_NORMAL); // setting group box

     createButton(hwnd, "", 260, 10, 150, 150, BS_GROUPBOX, ID_BUTTON, hFont13_NORMAL); // image group box
     createButton(hwnd, "", 260, 180, 150, 150, BS_GROUPBOX, ID_BUTTON, hFont13_NORMAL);
     
     //Label_Buffer
     createLabel(hwnd, "Left Click Buffer \0", 20, 10, 110, 15, 0, ID_LABEL, hFont16_BOLD);
     createLabel(hwnd, "Right Click Buffer \0", 20, 180, 115, 15, 0, ID_LABEL, hFont16_BOLD);

     createLabel(hwnd, "Left Click Buffer Rate\0", 20, 30, 150, 15, 0, ID_LABEL, hFont13_NORMAL);
     createLabel(hwnd, "Right Click Buffer Rate\0", 20, 200, 150, 15, 0, ID_LABEL, hFont13_NORMAL);

     //Tracks
     hTrack_left_click = createSlide(hwnd, "", 55, 55, 150, 30, TBS_AUTOTICKS, ID_TRACKBAR, 0, 5, 1);
     hTrack_right_click = createSlide(hwnd, "", 55, 225, 150, 30, TBS_AUTOTICKS, ID_TRACKBAR, 0, 5, 1);

     //Label_Levels
     createLabel(hwnd, "Level", 22, 60, 30, 15, 0, ID_LABEL, hFont16_NORMAL);
     createLabel(hwnd, "Level", 22, 230, 30, 15, 0, ID_LABEL, hFont16_NORMAL);
     
     //Tracks_Slides_Options
     createSlideNumber(hwnd, 0, 5, 65, 85, 20, 20, 25, hFont13_NORMAL);
     createSlideNumber(hwnd, 0, 5, 65, 255, 20, 20, 25, hFont13_NORMAL);

     //Label_Current_CPS
     sprintf(current_leftclick_cps, "Your Average 7 CPS Is Now %d CPS", level_cps[LEFT_CLICK_BUFFER_RATE]);
     sprintf(current_rightclick_cps, "Your Average 7 CPS Is Now %d CPS", level_cps[RIGHT_CLICK_BUFFER_RATE]);
     hLabel_Current_LeftClick_CPS = createLabel(hwnd, current_leftclick_cps, 20, 140, 200, 15, 0 ,ID_LABEL, hFont16_BOLD);
     hLabel_Current_RightClick_CPS = createLabel(hwnd, current_rightclick_cps, 20, 310, 200, 15, 0 ,ID_LABEL, hFont16_BOLD);
     
     //Label_Desciption
     createLabel(hwnd, "Left Click", 310, 143, 50, 15, 0 , ID_LABEL, hFont13_NORMAL);
     createLabel(hwnd, "Right Click", 310, 313, 55, 15, 0 , ID_LABEL, hFont13_NORMAL);
     
     //Buttons
     createButton(hwnd, "GitHub", 260, 340, 70, 20, BS_FLAT, ID_BUTTON_GITHUB, hFont13_NORMAL);
     createButton(hwnd, "Exit", 340, 340, 70, 20, BS_FLAT, ID_BUTTON_EXIT, hFont13_NORMAL);

     //Display BitMaps
     displayBitMap(hwnd, 270, 30, 100, 100, IDB_LEFT_CLICK);
     displayBitMap(hwnd, 270, 200, 100, 100, IDB_RIGHT_CLICK);

     break;
     case WM_HSCROLL:
     {
          if ((HWND)lParam == hTrack_left_click)
          {
               LEFT_CLICK_BUFFER_RATE = SendMessage(hTrack_left_click, TBM_GETPOS, 0, 0);                           //modify left click slide
               sprintf(current_leftclick_cps, "Your Average 7 CPS Is Now %d CPS", level_cps[LEFT_CLICK_BUFFER_RATE]);
               SetWindowText(hLabel_Current_LeftClick_CPS, current_leftclick_cps);
          }
          if ((HWND)lParam == hTrack_right_click) 
          {
               RIGHT_CLICK_BUFFER_RATE = SendMessage(hTrack_right_click, TBM_GETPOS, 0, 0);
               sprintf(current_rightclick_cps, "Your Average 7 CPS Is Now %d CPS", level_cps[RIGHT_CLICK_BUFFER_RATE]);         //modify right click slide
               SetWindowText(hLabel_Current_RightClick_CPS, current_rightclick_cps);
          }
          break;
     }
             
     case WM_CTLCOLORSTATIC:
          HDC hdcStatic = (HDC)wParam;
          SetBkMode(hdcStatic, TRANSPARENT); 
          SetTextColor(hdcStatic, RGB(0, 0, 0));
          return (INT_PTR)hWhiteBrush;
          
     case WM_COMMAND:

          if (LOWORD(wParam) == ID_BUTTON_GITHUB) ShellExecute(hwnd, "open", "https://github.com/HarpCheemse/Click-Buffer-Wind32-GUI-", NULL, NULL, SW_SHOWNORMAL);
          if (LOWORD(wParam) == ID_BUTTON_EXIT) PostQuitMessage(0);
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
