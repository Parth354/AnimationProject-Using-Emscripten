#include <stdio.h>
#include <stdlib.h> // as we use max
#include <emscripten.h>
#include <time.h>

// Number of Circles
#define NUM_CIRCLES 200

// Circle struct
struct Circle
{
    int x;
    int y;
    int r;
    int cr;
    int cg;
    int cb;
};

// Circle Animation Struct
struct CircleAnimationData
{
    int x;
    int y;
    int r;
    int xv;
    int yv;
    int xd;
    int yd;
};
// variable to hold circles
struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];
// Random Number Generator
int getRand(int max)
{
    return (rand() % max);
}

// Init circle data and start render -JS
int main()
{
    // seed random number generator
    srand(time(NULL));
    // create circles
    for (int i = 0; i < NUM_CIRCLES; i++)
    {
        int radius = getRand(50);
        // Coordinates
        int x = getRand(1000) + radius;
        int y = getRand(1000) + radius;

        // Fill Animation Data
        animationData[i].x = x;
        animationData[i].y = y;
        animationData[i].r = radius;
        animationData[i].xv = getRand(10);
        animationData[i].yv = getRand(10);
        animationData[i].xd = 1;
        animationData[i].yd = 1;

        // fill circles struct
        circles[i].x = x;
        circles[i].y = y;
        circles[i].r = radius;
        circles[i].cr = getRand(255);
        circles[i].cg = getRand(255);
        circles[i].cb = getRand(255);
    }

    printf("Init Circles \n");
    // emscripten_run_script("render()"); as we need to pass an argument so we will use
    EM_ASM({render($0, $1)}, NUM_CIRCLES * 6, 6);
    return 0;
}

// return animated circles to js
struct Circle *getCircles(int canvasWidth, int canvasHeight)
{
    // update circle Data
    for (int i = 0; i < NUM_CIRCLES; i++)
    {
        // Collision Right
        if (animationData[i].x + animationData[i].r >= canvasWidth)
            animationData[i].xd = 0;

        // Collision Left
        if (animationData[i].x - animationData[i].r <= 0)
            animationData[i].xd = 1;

        // Collision Top
        if (animationData[i].y - animationData[i].r <= 0)
            animationData[i].yd = 1;
        // Collision Bottom
        if (animationData[i].y + animationData[i].r >= canvasHeight)
        {
            animationData[i].yd = 0;
        }

        // Move the circles

        if (animationData[i].xd == 1)
        {
            animationData[i].x += animationData[i].xv;
        }else{
            animationData[i].x -= animationData[i].xv;
        }

        if (animationData[i].yd == 1){
            animationData[i].y += animationData[i].yv;
        }else{
            animationData[i].y -= animationData[i].yv;
        }

        circles[i].x=animationData[i].x;
        circles[i].y=animationData[i].y;
    }
    return circles;
}