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

    // build a test tree mimicking a window
    lor_RenderablePrimitivePtr root = pApplication->pRootRenderablePrimitive;
    lor_RenderablePrimitivePtr titleBar = NULL;
    lor_RenderablePrimitivePtr closeIcon = NULL;
    lor_RenderablePrimitivePtr button1 = NULL;
    lor_RenderablePrimitivePtr button2 = NULL;
    lor_RenderablePrimitivePtr button3 = NULL;

    // 1) Root Window Background (RECT) - Initial size, will be centered and resized in lorApplicationUpdate
    lorRenderablePrimitiveAllocate(&allocator, &root, LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&root->sRect, 0, 0, 800, 600); // Example initial size, origin (0,0) is top-left
    pApplication->pRootRenderablePrimitive = root; // Re-assign root as it was reallocated

    // 2) Title Bar (RECT) - child of root, positioned at the bottom
    lorRenderablePrimitiveAllocate(&allocator, &titleBar, LOR_PRIM_TYPE_RECT);
    // Position near bottom of 800x600 initial area: Y from 565 to 595
    lorRectFillPosSize(&titleBar->sRect, 5, 565, 790, 30);
    root->pChildren[0] = titleBar;

    // 3) Close Icon (RECT) - child of title bar, positioned bottom-right within title bar
    lorRenderablePrimitiveAllocate(&allocator, &closeIcon, LOR_PRIM_TYPE_RECT);
    // Position within title bar's coords (5, 565) to (795, 595) -> Icon at X=765, Y=570
    lorRectFillPosSize(&closeIcon->sRect, 765, 570, 20, 20);
    titleBar->pChildren[0] = closeIcon;
    titleBar->sChildrenCount = 1;

    // 4) Button 1 (RECT) - child of root, positioned above title bar
    lorRenderablePrimitiveAllocate(&allocator, &button1, LOR_PRIM_TYPE_RECT);
    // Position above title bar (which starts at Y=565)
    lorRectFillPosSize(&button1->sRect, 50, 500, 100, 40);
    root->pChildren[1] = button1;

    // 5) Button 2 (RECT) - child of root
    lorRenderablePrimitiveAllocate(&allocator, &button2, LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&button2->sRect, 50, 450, 100, 40);
    root->pChildren[2] = button2;

    // 6) Button 3 (RECT) - child of root
    lorRenderablePrimitiveAllocate(&allocator, &button3, LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&button3->sRect, 50, 400, 100, 40);
    root->pChildren[3] = button3;

    // Root has 4 children: Title Bar, Button1, Button2, Button3
    root->sChildrenCount = 4;

    // log the new tree structure
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

    // --- Center the root primitive ---
    int fbW = pApplication->sCurrentInputState.sFramebufferSize.sWidth;
    int fbH = pApplication->sCurrentInputState.sFramebufferSize.sHeight;
    lorRectFillMinMax(&pApplication->pRootRenderablePrimitive->sRect, 0.0, 0.0, fbW, fbH);

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