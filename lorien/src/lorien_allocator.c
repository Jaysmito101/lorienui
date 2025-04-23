#include "lorien/lorien.h"


struct __lor_DefaultAllocator {
    size_t sSize;
};
typedef struct __lor_DefaultAllocator __lor_DefaultAllocator;
typedef struct __lor_DefaultAllocator* __lor_DefaultAllocatorPtr;

static __lor_DefaultAllocatorPtr __lorDefaultAllocatorBuild() {
    __lor_DefaultAllocatorPtr pAllocator = (__lor_DefaultAllocatorPtr)LOR_MALLOC(sizeof(struct __lor_DefaultAllocator));
    if (pAllocator == NULL) {
        LOR_ERROR("Failed to allocate memory for default allocator.");
        return NULL;
    }
    pAllocator->sSize = 0;
    return pAllocator;
}

static void __lorDefaultAllocatorDestroy(void* pUserData) {
    __lor_DefaultAllocatorPtr pAllocator = (__lor_DefaultAllocatorPtr)pUserData;
    if (pAllocator != NULL) {
        LOR_FREE(pAllocator);
    }
}

static lor_Result __lorDefaultAllocatorAllocate(void* pUserData, size_t size, lor_AllocationType type, void** ppOut) {
    LOR_ASSERT_MSG(pUserData != NULL, "Invalid user data provided.");
    LOR_ASSERT_MSG(ppOut != NULL, "Invalid output pointer provided.");
    
    __lor_DefaultAllocatorPtr pAllocator = (__lor_DefaultAllocatorPtr)pUserData;

    // TODO: For now default allocator is just a proxy to malloc/free, improve this later
    (void)pAllocator; 
    (void)type;

    if (size == 0) {
        LOR_ERROR("Invalid size provided for allocation.");
        return LOR_RESULT_INVALID_ARGUMENT;
    }

    void* pMemory = LOR_MALLOC(size);
    if (pMemory == NULL) {
        LOR_ERROR("Failed to allocate memory.");
        return LOR_RESULT_OBJECT_CREATION_FAILED;
    }

    *ppOut = pMemory;
    return LOR_RESULT_SUCCESS;
}

static void __lorDefaultAllocatorAutoFree(void* pUserData, void* pObject) {
    LOR_ASSERT_MSG(pUserData != NULL, "Invalid user data provided.");
    LOR_ASSERT_MSG(pObject != NULL, "Invalid object provided.");

    __lor_DefaultAllocatorPtr pAllocator = (__lor_DefaultAllocatorPtr)pUserData;
    (void)pAllocator; // Unused for now

    LOR_FREE(pObject);
}

static void __lorDefaultAllocatorFastFree(void* pUserData, lor_AllocationType type, void* pObject) {
    LOR_ASSERT_MSG(pUserData != NULL, "Invalid user data provided.");
    LOR_ASSERT_MSG(pObject != NULL, "Invalid object provided.");

    __lor_DefaultAllocatorPtr pAllocator = (__lor_DefaultAllocatorPtr)pUserData;
    (void)pAllocator; // Unused for now
    (void)type; // Unused for now

    LOR_FREE(pObject);
}

static void __lorDefaultAllocatorDefragment(void* pUserData) {
    // No-op for default allocator
    (void)pUserData;
}

lor_Result lorGetDefaultAllocator(lor_AllocatorPtr pAllocator) {
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");    

    pAllocator->sType = LOR_STRUCT_ALLOCATOR;
    // Initialize the default allocator
    pAllocator->pUserData = __lorDefaultAllocatorBuild();
    if (pAllocator->pUserData == NULL) {
        LOR_ERROR("Failed to build default allocator.");
        return LOR_RESULT_OBJECT_CREATION_FAILED;
    }
    pAllocator->fDestroyAllocator = __lorDefaultAllocatorDestroy;
    pAllocator->fAllocate = __lorDefaultAllocatorAllocate;
    pAllocator->fAutoFree = __lorDefaultAllocatorAutoFree;
    pAllocator->fFastFree = __lorDefaultAllocatorFastFree;
    pAllocator->fDefragment = __lorDefaultAllocatorDefragment;

    return LOR_RESULT_SUCCESS;
}


void lorAllocatorDestroy(lor_AllocatorPtr pAllocator) {
    if (pAllocator == NULL) {
        LOR_ERROR("Invalid allocator provided.");
        return;
    }

    if (pAllocator->fDestroyAllocator != NULL) {
        pAllocator->fDestroyAllocator(pAllocator->pUserData);
    }
}

lor_Result lorAllocatorAllocate(lor_AllocatorPtr pAllocator, size_t size, lor_AllocationType type, void** ppOut) {  
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");
    LOR_ASSERT_MSG(pAllocator->fAllocate != NULL, "Allocator function not set.");
    
    lor_Result result = pAllocator->fAllocate(pAllocator->pUserData, size, type, ppOut);
    return result;    
}

void lorAllocatorAutoFree(lor_AllocatorPtr pAllocator, void* pObject) {
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");
    LOR_ASSERT_MSG(pAllocator->fAutoFree != NULL, "Allocator function not set.");

    pAllocator->fAutoFree(pAllocator->pUserData, pObject);
    if (pAllocator->fDefragment != NULL) {
        pAllocator->fDefragment(pAllocator->pUserData);
    }
}

void lorAllocatorFastFree(lor_AllocatorPtr pAllocator, lor_AllocationType type, void* pObject) {
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");
    LOR_ASSERT_MSG(pAllocator->fFastFree != NULL, "Allocator function not set.");

    pAllocator->fFastFree(pAllocator->pUserData, type, pObject);
    if (pAllocator->fDefragment != NULL) {
        pAllocator->fDefragment(pAllocator->pUserData);
    }
}

void lorAllocatorDefragment(lor_AllocatorPtr pAllocator) {
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");
    LOR_ASSERT_MSG(pAllocator->pUserData != NULL, "Invalid user data provided.");

    if (pAllocator->fDefragment != NULL) {
        pAllocator->fDefragment(pAllocator->pUserData);
    } else {
        LOR_WARN("Defragmentation function not set. Skipping defragmentation.");
    }
}