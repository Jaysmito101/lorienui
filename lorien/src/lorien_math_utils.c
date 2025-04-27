#include "lorien/lorien.h"

void lorRectFillMinMax(lor_Rect *pRect, float minX, float minY, float maxX, float maxY)
{
    pRect->sMinX = minX;
    pRect->sMinY = minY;
    pRect->sMaxX = maxX;
    pRect->sMaxY = maxY;
}

void lorRectFillPosSize(lor_Rect *pRect, float posX, float posY, float sizeX, float sizeY)
{
    pRect->sMinX = posX;
    pRect->sMinY = posY;
    pRect->sMaxX = posX + sizeX;
    pRect->sMaxY = posY + sizeY;
}

lor_Rect lorRectMinMax(float minX, float minY, float maxX, float maxY)
{
    lor_Rect rect = {0};
    lorRectFillMinMax(&rect, minX, minY, maxX, maxY);
    return rect;
}

lor_Rect lorRectPosSize(float posX, float posY, float sizeX, float sizeY)
{
    lor_Rect rect = {0};
    lorRectFillPosSize(&rect, posX, posY, sizeX, sizeY);
    return rect;
}

lor_Point lorPoint(float x, float y)
{
    lor_Point point = {0};
    point.sX = x;
    point.sY = y;
    return point;
}

lor_ISize lorISize(int32_t width, int32_t height)
{
    lor_ISize size = {0};
    size.sWidth = width;
    size.sHeight = height;
    return size;
}

lor_Size lorSize(float width, float height)
{
    lor_Size size = {0};
    size.sWidth = width;
    size.sHeight = height;
    return size;
}
