#include "application.h"
#include "ui.h"


// bool imageViewerPlatformUpdate(void* pUserData);

lor_Result buildImageViewerApplication(void* pUserData, lor_AllocatorPtr pAllocator, lor_ApplicationPtr* ppApplication) {
    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;
    lor_ApplicationConfig applicationConfig = { LOR_STRUCT_APPLICATION_CONFIG };
    // applicationConfig.fBuildUI = buildImageViewerUI;
    applicationConfig.pAllocator = pAllocator; // use the same allocator as the platform
    applicationConfig.pUserData = pUserData;
    applicationConfig.fShouldClose = imageViewerShouldClose;
    if ((result = lorApplicationBuild(&applicationConfig, ppApplication)) != LOR_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to build application: %d\n", result);
        return result;
    }
    return LOR_RESULT_SUCCESS;
}

void imageViewerPlatformError(void* pUserData, lor_PlatformErrorPtr pError) {
    if (pError == NULL) {
        fprintf(stderr, "Error: NULL error provided.\n");
        return;
    }
    fprintf(stderr, "Error: %d - %s\n", pError->sErrorCode, pError->sErrorMessage);
}

bool imageViewerPlatformUpdate(void* pUserData) {
    // printf("Updating application...\n");
    return true;
}

void imageViewerPlatformDestroy(void* pUserData, lor_AllocatorPtr pAllocator) {
	// Free any resources associated with the platform
	printf("Destroying platform...\n");
}

bool imageViewerShouldClose(void* pUserData) {
    printf("Should close application...?? YEA!\n");
    return true;
}