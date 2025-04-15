#include "lorien/lorien.h"
#include "ui.h"

int main() {
    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;
    
    lor_ApplicationConfig applicationConfig = { LOR_STRUCT_APPLICATION_CONFIG };
    // applicationConfig.fBuildUI = buildImageViewerUI;
    applicationConfig.pAllocator = NULL; // use default allocator
    applicationConfig.pUserData = NULL;
    lor_ApplicationPtr pApplication = NULL;
    if ((result = lorApplicationBuild(&applicationConfig, &pApplication)) != LOR_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to build application: %d\n", result);
        return EXIT_FAILURE;
    }

    // struct lor_PlatformConfig platformConfig = { LOR_STRUCT_PLATFORM_CONFIG };
    // platformConfig.pApplication = pApplication; // takes ownership of the application

    // struct lor_Platform* pPlatform = lorBuildPlatform(&platformConfig, NULL);
    // if (pPlatform == NULL) {
    //     fprintf(stderr, "Failed to build platform.\n");
    //     return EXIT_FAILURE;
    // }

    // lorPlatformRun(pPlatform);
    // lorPlatformDestroy(pPlatform);

    return 0;
}