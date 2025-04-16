#include <lorien/platforms/any-glfw-opengl3.h>

lor_Result lorPlatformAnyGLFWOpengl3(lor_PlatformConfigPtr pPlatformConfig, lor_PlatformAnyGLFWOpengl3Ptr* ppPlatform) {
#ifdef LOR_DEBUG
    if (pPlatformConfig == NULL || ppPlatform == NULL) {
        LOR_ERROR("Invalid arguments provided.");
        return LOR_RESULT_INVALID_ARGUMENT;
    }
#endif
    
    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;
    if (pPlatformConfig->sType != LOR_STRUCT_PLATFORM_CONFIG) {
        LOR_ERROR("Invalid struct type provided.");
        return LOR_RESULT_INVALID_ARGUMENT;
    }

    lor_Allocator allocator = { 0 };
    if (pPlatformConfig->pAllocator == NULL) {
        if ((result = lorGetDefaultAllocator(&allocator)) != LOR_RESULT_SUCCESS) {
            LOR_ERROR("Failed to get default allocator.");
            return result;
        }
    } else {
        allocator = *pPlatformConfig->pAllocator; // copy the allocator
    }

    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = NULL;
    if ((result = lorAllocatorAllocate(&allocator, sizeof(lor_PlatformAnyGLFWOpengl3), LOR_ALLOCATION_TYPE_GENERAL, (void**)&pPlatform)) != LOR_RESULT_SUCCESS) {
        LOR_ERROR("Failed to allocate platform.");
        return result;
    }

    if (pPlatformConfig->fPreloadPlatform != NULL) {
        pPlatformConfig->fPreloadPlatform(pPlatformConfig->pUserData, &allocator);
    }

    pPlatform->sAllocator = allocator;
    pPlatform->sIsAllocatorOwned = (pPlatformConfig->pAllocator == NULL);
    pPlatform->pUserData = pPlatformConfig->pUserData;
    pPlatform->fRenderPlatform = pPlatformConfig->fRenderPlatform;
    pPlatform->fUpdatePlatform = pPlatformConfig->fUpdatePlatform;
    pPlatform->fErrorPlatform = pPlatformConfig->fErrorPlatform;
    pPlatform->fDestroyPlatform = pPlatformConfig->fDestroyPlatform;
    pPlatform->sIsRunning = true;
    pPlatform->pApplication = NULL;
    assert(pPlatformConfig->fBuildApplication != NULL);
    if ((result = pPlatformConfig->fBuildApplication(pPlatformConfig->pUserData, &allocator, &pPlatform->pApplication)) != LOR_RESULT_SUCCESS) {
        LOR_ERROR("Failed to build application.");
        lorAllocatorFastFree(&allocator, LOR_ALLOCATION_TYPE_GENERAL, pPlatform);
        return result;
    }


    *ppPlatform = pPlatform;
    return LOR_RESULT_SUCCESS;
}

void lorPlatformAnyGLFWOpengl3Destroy(lor_PlatformAnyGLFWOpengl3Ptr pPlatform) {
#ifdef LOR_DEBUG
    if (pPlatform == NULL) {
        LOR_ERROR("Invalid platform provided.");
        return;
    }
#endif
    lor_Allocator allocator = pPlatform->sAllocator;
    lorApplicationDestroy(pPlatform->pApplication);
    if (pPlatform->fDestroyPlatform != NULL) {
        pPlatform->fDestroyPlatform(pPlatform->pUserData, &allocator);
    }
	bool isAllocatorOwned = pPlatform->sIsAllocatorOwned;
    lorAllocatorFastFree(&allocator, LOR_ALLOCATION_TYPE_GENERAL, pPlatform);
    if (isAllocatorOwned) { 
        lorAllocatorDestroy(&allocator);
    }
}

void lorPlatformAnyGLFWOpengl3Run(lor_PlatformAnyGLFWOpengl3Ptr pPlatform) {
    while (pPlatform->sIsRunning) {
        if (pPlatform->fUpdatePlatform != NULL) {
            pPlatform->sIsRunning = pPlatform->fUpdatePlatform(pPlatform->pUserData);
        }
        if (pPlatform->fRenderPlatform != NULL) {
            pPlatform->fRenderPlatform(pPlatform->pUserData);
        }
    }
}