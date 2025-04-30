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

lor_Color lorColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return LOR_PACK_RGBA(r, g, b, a);
}

uint8_t lorColorGetChannel(lor_Color color, uint8_t channel)
{
    switch (channel)
    {
    case 0:
        return (color >> 24) & 0xFF; // Red channel
    case 1:
        return (color >> 16) & 0xFF; // Green channel
    case 2:
        return (color >> 8) & 0xFF;  // Blue channel
    case 3:
        return color & 0xFF;         // Alpha channel
    default:
        return 0; // Invalid channel
    }
}

float lorColorGetChannelF32(lor_Color color, uint8_t channel) {
    uint16_t channelValue = lorColorGetChannel(color, channel);
    return (float)channelValue / 255.0f; // Convert to float in range [0, 1]
}

void lorColorSetChannel(lor_Color* color, uint8_t channel, uint8_t value) {
    switch (channel)
    {
    case 0:
        *color = (*color & 0x00FFFFFF) | ((uint32_t)value << 24); // Set Red channel
        break;
    case 1:
        *color = (*color & 0xFF00FFFF) | ((uint32_t)value << 16); // Set Green channel
        break;
    case 2:
        *color = (*color & 0xFFFF00FF) | ((uint32_t)value << 8);  // Set Blue channel
        break;
    case 3:
        *color = (*color & 0xFFFFFF00) | (uint32_t)value;         // Set Alpha channel
        break;
    default:
        break; // Invalid channel, do nothing
    }
}

void lorColorSetChannelF32(lor_Color* color, uint8_t channel, float value) {
    uint8_t channelValue = (uint8_t)(value * 255.0f); // Convert float in range [0, 1] to uint8_t
    lorColorSetChannel(color, channel, channelValue);
}