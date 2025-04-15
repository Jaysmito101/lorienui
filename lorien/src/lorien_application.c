#include "lorien/lorien.h"


lor_Result lorApplicationBuild(lor_ApplicationConfigPtr pConfig, lor_ApplicationPtr* ppApplication) {
#ifdef LOR_DEBUG
    if (pConfig == NULL || ppApplication == NULL) {
        LOR_ERROR("Invalid arguments provided.");
        return LOR_RESULT_INVALID_ARGUMENT;
    }
#endif

    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;

    if (pConfig->sType != LOR_STRUCT_APPLICATION_CONFIG) {
        LOR_ERROR("Invalid struct type provided.");
        return LOR_RESULT_INVALID_ARGUMENT;
    }

    lor_Allocator allocator = { 0 };
    if (pConfig->pAllocator == NULL) {
        if ((result = lorGetDefaultAllocator(&allocator)) != LOR_RESULT_SUCCESS) {
            LOR_ERROR("Failed to get default allocator.");
            return result;
        }
    } else {
        allocator = *pConfig->pAllocator; // copy the allocator
    }

    lor_ApplicationPtr pApplication = NULL;
    if ((result = lorAllocatorAllocate(&allocator, sizeof(struct lor_Application), LOR_ALLOCATION_TYPE_GENERAL, (void**)&pApplication)) != LOR_RESULT_SUCCESS) {
        LOR_ERROR("Failed to allocate application.");
        return result;
    }

    pApplication->sType = LOR_STRUCT_APPLICATION;
    pApplication->sAllocator = allocator;
    pApplication->pUserData = pConfig->pUserData;

    *ppApplication = pApplication;
    return LOR_RESULT_SUCCESS;
}

void lorApplicationDestroy(lor_ApplicationPtr pApplication) {
#ifdef LOR_DEBUG
    if (pApplication == NULL) {
        LOR_ERROR("Invalid application provided.");
        return;
    }   
#endif
    lor_Allocator allocator = pApplication->sAllocator;
    lorAllocatorFastFree(&allocator, LOR_ALLOCATION_TYPE_GENERAL, pApplication);
    lorAllocatorDestroy(&allocator);
}