#include "lorien/lorien.h"

#ifndef LORIEN_IMAGE_VIEWER_APPLICATION_H
#define LORIEN_IMAGE_VIEWER_APPLICATION_H

lor_Result buildImageViewerApplication(void* pUserData, lor_AllocatorPtr pAllocator, lor_ApplicationPtr* ppApplication);
void imageViewerPlatformError(void* pUserData, lor_PlatformErrorPtr pError);
bool imageViewerPlatformUpdate(void* pUserData);
void imageViewerPlatformDestroy(void* pUserData, lor_AllocatorPtr pAllocator);
bool imageViewerShouldClose(void* pUserData);


#endif // LORIEN_IMAGE_VIEWER_APPLICATION_H