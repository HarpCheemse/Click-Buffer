#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define INPUT_ERROR -1
#define MAX_SIZE 255

#define DETECT_LEFT_CLICK  (GetAsyncKeyState(VK_LBUTTON) & 0x8000) 
#define DETECT_RIGHT_CLICK (GetAsyncKeyState(VK_RBUTTON) & 0x8000) 

char VERSION[20] = "1.0";

int LEFT_CLICK_BUFFER_RATE = 1;
int RIGHT_CLICK_BUFFER_RATE = 1;

/*                     Photopyes                    */
void changeCPS();
void leftClick();
void rightClick();

/*                     Input Function                */

/*                    Functionality                  */
float randomNumberGenerator() // generate number between 0.9 - 1.1
{
    int min = 90;
    int max = 110;
    int random_number = min + rand() % (max - min + 1);
    return random_number/100.0;
}
int isHoldingLeftClick()
{
    const DWORD HOLD_THRESHOLD = 200;
    DWORD start_timer;
    DWORD current_timer;
    start_timer = GetTickCount();
    while(DETECT_LEFT_CLICK)     //detect left click and start the timer. if timer > threshold => holding key
    {
        current_timer = GetTickCount();
        if( current_timer - start_timer> HOLD_THRESHOLD)
        {
            return 1;  // no break to consume leftover hold LBUTTON
        }
        Sleep(1);
    }
    return 0;
}
int isHoldingRightClick()
{
    const DWORD HOLD_THRESHOLD = 200;
    DWORD start_timer;
    DWORD current_timer;
    start_timer = GetTickCount();
    while(DETECT_RIGHT_CLICK)   //detect right click and start the timer. if timer > threshold => holding key
    {
        current_timer = GetTickCount();
        if( current_timer - start_timer> HOLD_THRESHOLD)
        {
            return 1;  // no break to consume leftover hold LBUTTON
        }
        Sleep(1);
    }
    return 0;
}
int isLeftClickInWaitThreshold(DWORD start_timer) //Wait for another left click
{
    const DWORD WAIT_THRESHOLD = 200;
    DWORD current_timer;
    while(1)
    {
        current_timer = GetTickCount();
        if ((DETECT_LEFT_CLICK && LEFT_CLICK_BUFFER_RATE != 0)) return 1;
        if(current_timer - start_timer > WAIT_THRESHOLD) return 0;
    }
}
int isRightClickInWaitThreshold(DWORD start_timer) //Wait for another left click
{
    const DWORD WAIT_THRESHOLD = 200;
    DWORD current_timer;
    while(1)
    {
        current_timer = GetTickCount();
        if ((DETECT_RIGHT_CLICK && RIGHT_CLICK_BUFFER_RATE != 0)) return 1;
        if(current_timer - start_timer > WAIT_THRESHOLD) return 0;
    }
}
void performLeftClick(int LEFT_CLICK_BUFFER_RATE)
{
    switch(LEFT_CLICK_BUFFER_RATE)
    {
        case 1:
            leftClick();
            Sleep((int)(100.0 * randomNumberGenerator()));
            break;
        case 2:
            leftClick();
            Sleep((int)(40.0 * randomNumberGenerator()));
            leftClick();
            

            break;
        case 3:
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(20.0 * randomNumberGenerator()));
            break;
        case 4:
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            break;
        case 5:
            leftClick();
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            break;
    }
}
void performRightClick(int RIGHT_CLICK_BUFFER_RATE)
{
    switch(RIGHT_CLICK_BUFFER_RATE)
    {
        case 1:
            rightClick();
            Sleep((int)(100.0 * randomNumberGenerator()));
            break;
        case 2:
            rightClick();
            Sleep((int)(40.0 * randomNumberGenerator()));
            rightClick();
            

            break;
        case 3:
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(20.0 * randomNumberGenerator()));
            break;
        case 4:
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            break;
        case 5:
            rightClick();
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            break;
    }
}
void leftClick()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
void rightClick()
{
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}
void* leftClickEvent(void*)
{
    DWORD start_timer;
    while(1)
    {
        int is_valid_left_click = 1;
        if(DETECT_LEFT_CLICK && LEFT_CLICK_BUFFER_RATE != 0)  //detect the first left click
        {
            if(isHoldingLeftClick()) continue;  //track if it's a click or a hold
            while(is_valid_left_click)
            {
                start_timer = GetTickCount();
                if(!isLeftClickInWaitThreshold(start_timer)) is_valid_left_click = 0;  //detect if user is clicking fast
                if(isHoldingLeftClick())
                {
                    is_valid_left_click = 0; 
                    printf("STOPING ");
                }     //go back to loop one if user is clicking not fast enough / holding left click
                if(is_valid_left_click)
                {
                    performLeftClick(LEFT_CLICK_BUFFER_RATE);
                }
            }
        }    
        Sleep(1); // reduce CPS usage
    }
    return NULL;
}
void* rightClickEvent(void*)
{
    DWORD start_timer;
    while(1)
    {
        int is_valid_right_click = 1;
        if(DETECT_RIGHT_CLICK && RIGHT_CLICK_BUFFER_RATE != 0)  //detect the first left click
        {
            if(isHoldingRightClick()) continue;  //track if it's a click or a hold
            while(is_valid_right_click)
            {
                start_timer = GetTickCount();
                if(!isRightClickInWaitThreshold(start_timer)) is_valid_right_click = 0;  //detect if user is clicking fast
                if(isHoldingRightClick()) is_valid_right_click = 0;      //go back to loop one if user is clicking not fast enough / holding left click
                if(is_valid_right_click)
                {
                    performRightClick(RIGHT_CLICK_BUFFER_RATE);
                }
            }
        }    
        Sleep(1); // reduce CPS usage
    }
    return NULL;
}
void startClickThreads()
{
    srand(time(NULL));

    pthread_t background_left_click_event;
    pthread_t background_right_click_event;

    pthread_create(&background_left_click_event, NULL, leftClickEvent, NULL);
    pthread_create(&background_right_click_event, NULL, rightClickEvent, NULL);

    pthread_detach(background_left_click_event);             //run both thread at the same time
    pthread_detach(background_right_click_event);            //if run 1 thread only, the code has to wait for left click buffer to finish to move on to right click buffer

}