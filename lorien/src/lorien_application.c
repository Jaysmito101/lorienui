#include "lorien/lorien.h"

lor_Result lorApplicationBuild(lor_ApplicationConfigPtr pConfig, lor_ApplicationPtr *ppApplication)
{
    LOR_ASSERT_MSG(pConfig != NULL, "Invalid application config provided.");
    LOR_ASSERT_MSG(ppApplication != NULL, "Invalid application pointer provided.");

    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;

    if (pConfig->sType != LOR_STRUCT_APPLICATION_CONFIG)
    {
        LOR_ERROR("Invalid struct type provided.");
        return LOR_RESULT_INVALID_ARGUMENT;
    }

    lor_Allocator allocator = {0};
    if (pConfig->pAllocator == NULL)
    {
        if ((result = lorGetDefaultAllocator(&allocator)) != LOR_RESULT_SUCCESS)
        {
            LOR_ERROR("Failed to get default allocator.");
            return result;
        }
    }
    else
    {
        allocator = *pConfig->pAllocator; // copy the allocator
    }

    lor_ApplicationPtr pApplication = NULL;
    if ((result = lorAllocatorAllocate(&allocator, sizeof(struct lor_Application), LOR_ALLOCATION_TYPE_GENERAL, (void **)&pApplication)) != LOR_RESULT_SUCCESS)
    {
        LOR_ERROR("Failed to allocate application.");
        return result;
    }
    memset(pApplication, 0, sizeof(struct lor_Application)); // Initialize to zero
    pApplication->sType = LOR_STRUCT_APPLICATION;
    pApplication->sAllocator = allocator;
    pApplication->pUserData = pConfig->pUserData;
    pApplication->fShouldClose = pConfig->fShouldClose;
    pApplication->sIsAllocatorOwned = (pConfig->pAllocator == NULL);
    pApplication->pRootRenderablePrimitive = NULL;
    lorInputStateReset(&pApplication->sCurrentInputState);
    lorInputStateReset(&pApplication->sPreviousInputState);

    *ppApplication = pApplication;

    // create a basic primitive for testing the platform renderer
    // (no validations done here as its just a test)
    lorRenderablePrimitiveAllocate(&allocator, &pApplication->pRootRenderablePrimitive, LOR_PRIM_TYPE_VOID);

    // build a 10-node test tree
    lor_RenderablePrimitivePtr root = pApplication->pRootRenderablePrimitive;

    // 1) three direct children of root
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[0], LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&root->pChildren[0]->sRect, 0,   0, 100, 100);
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[1], LOR_PRIM_TYPE_CIRCLE);
    lorRectFillPosSize(&root->pChildren[1]->sRect, 50,  50, 150, 150);
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[2], LOR_PRIM_TYPE_MESH);
    lorRectFillPosSize(&root->pChildren[2]->sRect,100, 100, 200, 200);
    root->sChildrenCount = 3;

    // 2) two grandchildren under child 0 (now node count = 1+3+2 = 6)
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[0]->pChildren[0], LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&root->pChildren[0]->pChildren[0]->sRect, 10, 10,  60,  60);
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[0]->pChildren[1], LOR_PRIM_TYPE_CIRCLE);
    lorRectFillPosSize(&root->pChildren[0]->pChildren[1]->sRect, 20, 20,  70,  70);
    root->pChildren[0]->sChildrenCount = 2;

    // 3) three grandchildren under child 1 (now node count = 6+3 = 9)
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[1]->pChildren[0], LOR_PRIM_TYPE_MESH);
    lorRectFillPosSize(&root->pChildren[1]->pChildren[0]->sRect, 55, 55,  80,  80);
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[1]->pChildren[1], LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&root->pChildren[1]->pChildren[1]->sRect, 65, 65,  85,  85);
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[1]->pChildren[2], LOR_PRIM_TYPE_CIRCLE);
    lorRectFillPosSize(&root->pChildren[1]->pChildren[2]->sRect, 75, 75,  95,  95);
    root->pChildren[1]->sChildrenCount = 3;

    // 4) one grandchild under child 2 (now node count = 9+1 = 10)
    lorRenderablePrimitiveAllocate(&allocator, &root->pChildren[2]->pChildren[0], LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&root->pChildren[2]->pChildren[0]->sRect,100,100,120,120);
    root->pChildren[2]->sChildrenCount = 1;

    // log the full 10-node tree
    lorRenderablePrimitiveLogTree(root);

    return LOR_RESULT_SUCCESS;
}

void lorApplicationDestroy(lor_ApplicationPtr pApplication)
{
    LOR_ASSERT_MSG(pApplication != NULL, "Invalid application provided.");
    lor_Allocator allocator = pApplication->sAllocator;
    bool isAllocatorOwned = pApplication->sIsAllocatorOwned;

    if (pApplication->pRootRenderablePrimitive != NULL)
    {
        lorRenderablePrimitiveDestroyRecursive(&allocator, pApplication->pRootRenderablePrimitive);
    }

    lorAllocatorFastFree(&allocator, LOR_ALLOCATION_TYPE_GENERAL, pApplication);
    if (isAllocatorOwned)
    {
        lorAllocatorDestroy(&allocator);
    }
}

bool lorApplicationUpdate(lor_ApplicationPtr pApplication)
{
    LOR_ASSERT_MSG(pApplication != NULL, "Invalid application provided.");

    bool shouldContinue = !pApplication->sCurrentInputState.sShouldWindowClose;
    if (pApplication->sCurrentInputState.sShouldWindowClose && pApplication->fShouldClose != NULL)
    {
        shouldContinue = !pApplication->fShouldClose(pApplication->pUserData);
    }

    lorRectFillMinMax(&pApplication->pRootRenderablePrimitive->sRect, 0, 0, pApplication->sCurrentInputState.sFramebufferSize.sWidth, pApplication->sCurrentInputState.sFramebufferSize.sHeight);

    return shouldContinue;
}

void lorApplicationNewFrame(lor_ApplicationPtr pApplication)
{
    LOR_ASSERT_MSG(pApplication != NULL, "Invalid application provided.");

    lorInputStateNewFrame(&pApplication->sCurrentInputState);
    lorInputStateSwap(&pApplication->sCurrentInputState, &pApplication->sPreviousInputState);
}

lor_InputStatePtr lorApplicationGetInputState(lor_ApplicationPtr pApplication)
{
    LOR_ASSERT_MSG(pApplication != NULL, "Invalid application provided.");
    return &pApplication->sCurrentInputState;
}