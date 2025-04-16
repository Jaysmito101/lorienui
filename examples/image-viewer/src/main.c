#include <lorien/lorien.h>
#include "lorien/platforms/any-glfw-opengl3.h"

#include "application.h"


int main() {
    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;

    lor_PlatformAnyGLFWOpengl3Config customPlatformConfig = { 0 };
	customPlatformConfig.windowStartingWidth = 1200;
	customPlatformConfig.windowStartingHeight = 800;
	customPlatformConfig.windowTitle = "LorienUI/Samples/Image Viewer - Jaysmito Mukherjee";
   
    lor_PlatformConfig platformConfig = { LOR_STRUCT_PLATFORM_CONFIG };
    platformConfig.pUserData = NULL;
    platformConfig.fPreloadPlatform = NULL; // No preload function for now
    platformConfig.fBuildApplication = buildImageViewerApplication;
    platformConfig.fErrorPlatform = imageViewerPlatformError;
    platformConfig.fUpdatePlatform = imageViewerPlatformUpdate;
    platformConfig.fDestroyPlatform = imageViewerPlatformDestroy;
    platformConfig.pCustomPlatformConfig = &customPlatformConfig;

    lor_PlatformAnyGLFWOpengl3Ptr platform = NULL;
    if ((result = lorPlatformAnyGLFWOpengl3(&platformConfig, &platform)) != LOR_RESULT_SUCCESS) {
        fprintf(stderr, "Failed to create platform: %d\n", result);
        return EXIT_FAILURE;
    }
    lorPlatformAnyGLFWOpengl3Run(platform);
    lorPlatformAnyGLFWOpengl3Destroy(platform);
    return 0;
}