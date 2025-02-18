#include "screen.h"
#include "../wrapper/checkError.h"

Screen::Screen()
{
    this->pScreenGeo = Geometry::createScreenPlane();
}

Screen::~Screen()
{
    printf("---- ~Screen ----\n");
}

void Screen::draw()
{
    Screen& pMan = Screen::privGetInstance();
    pMan.privDraw();
}

//--------------------private impl methods --------------------

void Screen::privDraw()
{
    GL_CALL(glBindVertexArray(this->pScreenGeo->getVao()));
    GL_CALL(glDrawElements(GL_TRIANGLES, this->pScreenGeo->getIndicesCount(), GL_UNSIGNED_INT, 0));
}
