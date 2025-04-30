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

void lorRectUnionInplace(lor_Rect* pRect, const lor_Rect* pOtherRect) {
    if (pOtherRect->sMinX < pRect->sMinX) {
        pRect->sMinX = pOtherRect->sMinX;
    }
    if (pOtherRect->sMinY < pRect->sMinY) {
        pRect->sMinY = pOtherRect->sMinY;
    }
    if (pOtherRect->sMaxX > pRect->sMaxX) {
        pRect->sMaxX = pOtherRect->sMaxX;
    }
    if (pOtherRect->sMaxY > pRect->sMaxY) {
        pRect->sMaxY = pOtherRect->sMaxY;
    }
}

lor_Size lorRectGetSize(const lor_Rect* pRect) {
    lor_Size size = {0};
    size.sWidth = pRect->sMaxX - pRect->sMinX;
    size.sHeight = pRect->sMaxY - pRect->sMinY;
    return size;
}

lor_Point lorRectGetCenter(const lor_Rect* pRect) {
    lor_Point center = {0};
    center.sX = (pRect->sMinX + pRect->sMaxX) / 2.0f;
    center.sY = (pRect->sMinY + pRect->sMaxY) / 2.0f;
    return center;
}

lor_Point lorRectGetMin(const lor_Rect* pRect) {
    lor_Point min = {0};
    min.sX = pRect->sMinX;
    min.sY = pRect->sMinY;
    return min;
}

lor_Rect lorRectUnion(const lor_Rect* pRect, const lor_Rect* pOtherRect) {
    lor_Rect result = *pRect;
    lorRectUnionInplace(&result, pOtherRect);
    return result;
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
