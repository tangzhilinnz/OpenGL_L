#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "../glframework/core.h"
#include "../glframework/geometry.h"


class Screen
{
private:
    Geometry* pScreenGeo = nullptr;
public:
    Screen(const Screen&) = delete;
    Screen(Screen&&) = delete;
    Screen& operator=(const Screen&) & = delete;
    Screen& operator=(Screen&&) & = delete;
    ~Screen();

    static void draw();
    static void init();

private:
    static Screen& privGetInstance()
    {
        static Screen mInstance;
        return mInstance;
    }

    void privDraw();

private:
    Screen();
};

#endif // _SCREEN_H_