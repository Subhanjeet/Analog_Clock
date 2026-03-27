#include <graphics.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <dos.h>

#define PI 3.14159265358979
#define CX 320
#define CY 240
#define R  180

void drawHand(int angle_deg, int length, int color, int thickness)
{
    double angle;
    int x, y;

    angle = (angle_deg - 90) * PI / 180.0;
    x = CX + (int)(length * cos(angle));
    y = CY + (int)(length * sin(angle));

    setcolor(color);
    setlinestyle(SOLID_LINE, 0, thickness);
    line(CX, CY, x, y);
}

void drawFace()
{
    int i;
    double angle;
    int x1, y1, x2, y2, x, y;

    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, 2);
    circle(CX, CY, R);
    circle(CX, CY, R - 5);

    for (i = 0; i < 12; i++)
    {
        angle = (i * 30 - 90) * PI / 180.0;
        x1 = CX + (int)((R - 10) * cos(angle));
        y1 = CY + (int)((R - 10) * sin(angle));
        x2 = CX + (int)((R - 25) * cos(angle));
        y2 = CY + (int)((R - 25) * sin(angle));
        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 3);
        line(x1, y1, x2, y2);
    }

    for (i = 0; i < 60; i++)
    {
        if (i % 5 == 0) continue;
        angle = (i * 6 - 90) * PI / 180.0;
        x = CX + (int)((R - 12) * cos(angle));
        y = CY + (int)((R - 12) * sin(angle));
        putpixel(x, y, LIGHTGRAY);
        putpixel(x+1, y, LIGHTGRAY);
    }

    setfillstyle(SOLID_FILL, RED);
    fillellipse(CX, CY, 6, 6);
}

int main()
{
    int gd = DETECT, gm;
    int hr, min, sec, hrAngle, minAngle, secAngle;
    char timeStr[10];
    time_t t;
    struct tm *now;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    while (1)
    {
	cleardevice();

	t   = time(NULL);
	now = localtime(&t);

	hr  = now->tm_hour % 12;
	min = now->tm_min;
	sec = now->tm_sec;

	secAngle = sec * 6;
	minAngle = min * 6 + sec / 10;
	hrAngle  = hr  * 30 + min / 2;

	drawFace();

	drawHand(hrAngle,  (int)(R * 0.50), WHITE, 3);
	drawHand(minAngle, (int)(R * 0.75), WHITE, 2);
	drawHand(secAngle, (int)(R * 0.85), RED,   1);

	sprintf(timeStr, "%02d:%02d:%02d", now->tm_hour, min, sec);
	setcolor(YELLOW);
	outtextxy(CX - 30, CY + R/2, timeStr);

	delay(1000);
    }
}