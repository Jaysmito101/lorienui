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

    static lor_Vertex sDummyMeshesBuffer[1024];
    static uint32_t sDummyIndicesBuffer[1024];

    // 1) Root Window Background (RECT) - Initial size, will be centered and resized in lorApplicationUpdate
    lorRenderablePrimitiveAllocate(&allocator, &root, LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&root->sRect, 0, 0, 800, 600); // Example initial size, origin (0,0) is top-left
    root->sColor = lorColor(220, 220, 220, 255); // Light gray background
    pApplication->pRootRenderablePrimitive = root; // Re-assign root as it was reallocated

    // 2) Title Bar (RECT) - child of root, positioned at the bottom
    lorRenderablePrimitiveAllocate(&allocator, &titleBar, LOR_PRIM_TYPE_RECT);
    // Position near bottom of 800x600 initial area: Y from 565 to 595
    lorRectFillPosSize(&titleBar->sRect, 5, 565, 790, 30);
    titleBar->sColor = lorColor(100, 100, 180, 255); // Blue-ish
    root->pChildren[0] = titleBar;

    // 3) Close Icon (RECT) - child of title bar, positioned bottom-right within title bar
    lorRenderablePrimitiveAllocate(&allocator, &closeIcon, LOR_PRIM_TYPE_RECT);
    // Position within title bar's coords (5, 565) to (795, 595) -> Icon at X=765, Y=570
    lorRectFillPosSize(&closeIcon->sRect, 765, 570, 20, 20);
    closeIcon->sColor = lorColor(220, 80, 80, 255); // Red-ish
    titleBar->pChildren[0] = closeIcon;
    titleBar->sChildrenCount = 1;

    // 4) Button 1 (RECT) - child of root, positioned above title bar
    lorRenderablePrimitiveAllocate(&allocator, &button1, LOR_PRIM_TYPE_RECT);
    // Position above title bar (which starts at Y=565)
    lorRectFillPosSize(&button1->sRect, 50, 500, 100, 40);
    button1->sColor = lorColor(180, 180, 180, 255); // Gray
    root->pChildren[1] = button1;

    // 5) Button 2 (RECT) - child of root
    lorRenderablePrimitiveAllocate(&allocator, &button2, LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&button2->sRect, 50, 450, 100, 40);
    button2->sColor = lorColor(180, 180, 180, 255); // Gray
    root->pChildren[2] = button2;

    // 6) Button 3 (RECT) - child of root
    lorRenderablePrimitiveAllocate(&allocator, &button3, LOR_PRIM_TYPE_RECT);
    lorRectFillPosSize(&button3->sRect, 50, 400, 100, 40);
    button3->sColor = lorColor(180, 180, 180, 255); // Gray
    root->pChildren[3] = button3;

    // Add mesh primitives using parts of the dummy buffers
    
    // 7) Triangle mesh
    lor_RenderablePrimitivePtr triangleMesh = NULL;
    lorRenderablePrimitiveAllocate(&allocator, &triangleMesh, LOR_PRIM_TYPE_MESH);
    lorRectFillPosSize(&triangleMesh->sRect, 200, 400, 150, 150);
    triangleMesh->sColor = lorColor(255, 100, 100, 255); // Red-ish
    
    // Set up triangle mesh data
    lor_Vertex* triangleVertices = &sDummyMeshesBuffer[0];
    triangleVertices[0] = (lor_Vertex){250, 400, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0}; // Top
    triangleVertices[1] = (lor_Vertex){200, 550, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0}; // Bottom-left
    triangleVertices[2] = (lor_Vertex){350, 550, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}; // Bottom-right
    
    uint32_t* triangleIndices = &sDummyIndicesBuffer[0];
    triangleIndices[0] = 0;
    triangleIndices[1] = 1;
    triangleIndices[2] = 2;
    
    triangleMesh->sData.sMesh.pVertices = triangleVertices;
    triangleMesh->sData.sMesh.pIndices = triangleIndices;
    triangleMesh->sData.sMesh.sVertexCount = 3;
    triangleMesh->sData.sMesh.sIndexCount = 3;
    
    root->pChildren[4] = triangleMesh;
    
    // 8) Diamond mesh
    lor_RenderablePrimitivePtr diamondMesh = NULL;
    lorRenderablePrimitiveAllocate(&allocator, &diamondMesh, LOR_PRIM_TYPE_MESH);
    lorRectFillPosSize(&diamondMesh->sRect, 400, 400, 150, 150);
    diamondMesh->sColor = lorColor(100, 255, 100, 255); // Green-ish
    
    // Set up diamond mesh data
    lor_Vertex* diamondVertices = &sDummyMeshesBuffer[10];
    diamondVertices[0] = (lor_Vertex){475, 400, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0}; // Top
    diamondVertices[1] = (lor_Vertex){400, 475, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0}; // Left
    diamondVertices[2] = (lor_Vertex){475, 550, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}; // Bottom
    diamondVertices[3] = (lor_Vertex){550, 475, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0}; // Right
    
    uint32_t* diamondIndices = &sDummyIndicesBuffer[10];
    // First triangle
    diamondIndices[0] = 0;
    diamondIndices[1] = 1;
    diamondIndices[2] = 3;
    // Second triangle
    diamondIndices[3] = 1;
    diamondIndices[4] = 2;
    diamondIndices[5] = 3;
    
    diamondMesh->sData.sMesh.pVertices = diamondVertices;
    diamondMesh->sData.sMesh.pIndices = diamondIndices;
    diamondMesh->sData.sMesh.sVertexCount = 4;
    diamondMesh->sData.sMesh.sIndexCount = 6;
    
    root->pChildren[5] = diamondMesh;
    
    // 9) Star mesh
    lor_RenderablePrimitivePtr starMesh = NULL;
    lorRenderablePrimitiveAllocate(&allocator, &starMesh, LOR_PRIM_TYPE_MESH);
    lorRectFillPosSize(&starMesh->sRect, 600, 400, 150, 150);
    starMesh->sColor = lorColor(255, 255, 100, 255); // Yellow
    
    // Set up star mesh data
    lor_Vertex* starVertices = &sDummyMeshesBuffer[20];
    float cx = 675, cy = 475, outerRadius = 75, innerRadius = 30;
    
    // Center point
    starVertices[0] = (lor_Vertex){cx, cy, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0};
    
    // 5 outer points
    for (int i = 0; i < 5; i++) {
        float angle = (float)i * 2 * 3.14159f / 5 - 3.14159f/2;
        starVertices[i+1] = (lor_Vertex){
            cx + outerRadius * cosf(angle),
            cy + outerRadius * sinf(angle),
            0, 0, 0, 1, 1, 0, 1, 0, 0, 0
        };
    }
    
    // 5 inner points
    for (int i = 0; i < 5; i++) {
        float angle = (float)i * 2 * 3.14159f / 5 - 3.14159f/2 + 3.14159f/5;
        starVertices[i+6] = (lor_Vertex){
            cx + innerRadius * cosf(angle),
            cy + innerRadius * sinf(angle),
            0, 0, 0, 1, 0.5f, 0, 1, 0, 0, 0
        };
    }
    
    uint32_t* starIndices = &sDummyIndicesBuffer[20];
    for (int i = 0; i < 5; i++) {
        // Outer triangle
        starIndices[i*6] = 0;
        starIndices[i*6+1] = i+1;
        starIndices[i*6+2] = (i+1)%5+1;
        
        // Inner triangle
        starIndices[i*6+3] = 0;
        starIndices[i*6+4] = i+6;
        starIndices[i*6+5] = (i+1)%5+6;
    }
    
    starMesh->sData.sMesh.pVertices = starVertices;
    starMesh->sData.sMesh.pIndices = starIndices;
    starMesh->sData.sMesh.sVertexCount = 11;
    starMesh->sData.sMesh.sIndexCount = 30;
    
    root->pChildren[6] = starMesh;
    
    // Root now has 7 children: Title Bar, Button1, Button2, Button3, Triangle, Diamond, Star
    root->sChildrenCount = 7;

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