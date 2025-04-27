#include "lorien/lorien.h"

lor_Result lorRenderablePrimitiveAllocate(lor_AllocatorPtr pAllocator, lor_RenderablePrimitivePtr *ppRenderablePrimitive, lor_PrimTypes sType)
{
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");
    LOR_ASSERT_MSG(ppRenderablePrimitive != NULL, "Invalid renderable primitive pointer provided.");

    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;
    lor_RenderablePrimitivePtr pRenderablePrimitive = NULL;

    if ((result = lorAllocatorAllocate(pAllocator, sizeof(struct lor_RenderablePrimitive), LOR_ALLOCATION_TYPE_RENDERABLE_PRIMITIVE, (void **)&pRenderablePrimitive)) != LOR_RESULT_SUCCESS)
    {
        LOR_ERROR("Failed to allocate renderable primitive.");
        return result;
    }
    memset(pRenderablePrimitive, 0, sizeof(struct lor_RenderablePrimitive)); // Initialize to zero
    pRenderablePrimitive->sType = LOR_STRUCT_RENDERABLE_PRIMITIVE;
    pRenderablePrimitive->sId = lorGenerateUniqueId(); 
    pRenderablePrimitive->sData.sType = sType;
    pRenderablePrimitive->pParent = NULL;
    pRenderablePrimitive->sChildrenCount = 0;
    *ppRenderablePrimitive = pRenderablePrimitive;
    return LOR_RESULT_SUCCESS;
}

void lorRenderablePrimitiveDestroyRecursive(lor_AllocatorPtr pAllocator, lor_RenderablePrimitivePtr pRenderablePrimitive)
{
    LOR_ASSERT_MSG(pRenderablePrimitive != NULL, "Invalid renderable primitive provided.");
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");

    
    // loop through all the children and destroy them recursively
    
    if (pRenderablePrimitive->sChildrenCount > 0)
    {
        for (size_t i = 0; i < LOR_MAX_CHILDREN; i++)
        {
            lor_RenderablePrimitivePtr pChild = pRenderablePrimitive->pChildren[i];
            if (pChild != NULL)
            {
                lorRenderablePrimitiveDestroyRecursive(pAllocator, pChild);
            }
        }
    }    
    lorRenderablePrimitiveDestroy(pAllocator, pRenderablePrimitive); // destroy the current primitive
}

void lorRenderablePrimitiveDestroy(lor_AllocatorPtr pAllocator, lor_RenderablePrimitivePtr pRenderablePrimitive)
{
    LOR_ASSERT_MSG(pRenderablePrimitive != NULL, "Invalid renderable primitive provided.");
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");
    lorRenderablePrimitiveDetach(pRenderablePrimitive);
    lorAllocatorFastFree(pAllocator, LOR_ALLOCATION_TYPE_RENDERABLE_PRIMITIVE, pRenderablePrimitive);
}

void lorRenderablePrimitiveDetach(lor_RenderablePrimitivePtr pRenderablePrimitive)
{
    LOR_ASSERT_MSG(pRenderablePrimitive != NULL, "Invalid renderable primitive provided.");

    if (pRenderablePrimitive->pParent != NULL)
    {
        // unlink the child from the parent
        for (size_t i = 0; i < LOR_MAX_CHILDREN; i++)
        {
            if (pRenderablePrimitive->pParent->pChildren[i] == pRenderablePrimitive)
            {
                pRenderablePrimitive->pParent->pChildren[i] = NULL;
                pRenderablePrimitive->pParent->sChildrenCount--;
                break;
            }
        }
        pRenderablePrimitive->pParent = NULL;
    }

    // detach all the children from the parent
    if (pRenderablePrimitive->sChildrenCount > 0)
    {
        for (size_t i = 0; i < LOR_MAX_CHILDREN; i++)
        {
            lor_RenderablePrimitivePtr pChild = pRenderablePrimitive->pChildren[i];
            if (pChild != NULL)
            {
                pRenderablePrimitive->pChildren[i] = NULL; 
                pChild->pParent = NULL;
            }
        }
        pRenderablePrimitive->sChildrenCount = 0; 
    }
}


const char* lorRenderablePrimitiveGetTypeString(lor_PrimTypes sType) {
    switch (sType)
    {
    case LOR_PRIM_TYPE_VOID:
        return "LOR_PRIM_TYPE_VOID";
    case LOR_PRIM_TYPE_RECT:
        return "LOR_PRIM_TYPE_RECT";
    case LOR_PRIM_TYPE_CIRCLE:
        return "LOR_PRIM_TYPE_CIRCLE";
    case LOR_PRIM_TYPE_MESH:
        return "LOR_PRIM_TYPE_MESH";
    default:
        return "Unknown Primitive Type";
    }
}

void __lorRenderablePrimitiveLogTreeWithDepth(lor_RenderablePrimitivePtr pRenderablePrimitive, int32_t depth) {
    LOR_ASSERT_MSG(pRenderablePrimitive != NULL, "Invalid renderable primitive provided.");
    if (depth < 0) {
        depth = 0;
    }

    for (int32_t i = 0; i < depth + 1; i++) {
        printf("--");
    }

    printf("> [%zu] [%s] [%zu children]\n", pRenderablePrimitive->sId, lorRenderablePrimitiveGetTypeString(pRenderablePrimitive->sData.sType), pRenderablePrimitive->sChildrenCount);

    for (size_t i = 0; i < LOR_MAX_CHILDREN; i++) {
        lor_RenderablePrimitivePtr pChild = pRenderablePrimitive->pChildren[i];
        if (pChild != NULL) {
            __lorRenderablePrimitiveLogTreeWithDepth(pChild, depth + 1);
        }
    }
}

void lorRenderablePrimitiveLogTree(lor_RenderablePrimitivePtr pRenderablePrimitive) {
    LOR_ASSERT_MSG(pRenderablePrimitive != NULL, "Invalid renderable primitive provided.");
    __lorRenderablePrimitiveLogTreeWithDepth(pRenderablePrimitive, 0);
}