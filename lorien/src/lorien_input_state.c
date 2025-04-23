#include "lorien/lorien.h"

void lorInputStateNewFrame(lor_InputStatePtr pInputState) {
    LOR_ASSERT_MSG(pInputState != NULL, "Invalid input state provided.");

    // Reset items that is supposed to be present for only one frame
    pInputState->sShouldWindowClose = false;
}

void lorInputStateSwap(lor_InputStatePtr pCurrentState, lor_InputStatePtr pPreviousState) {
    LOR_ASSERT_MSG(pCurrentState != NULL, "Invalid current input state provided.");
    LOR_ASSERT_MSG(pPreviousState != NULL, "Invalid previous input state provided.");
    
    lor_InputState temp = *pCurrentState;
    *pCurrentState = *pPreviousState;
    *pPreviousState = temp;
}

void lorInputStateReset(lor_InputStatePtr pInputState) {
    LOR_ASSERT_MSG(pInputState != NULL, "Invalid input state provided.");

    // Reset all items in the input state
    pInputState->sShouldWindowClose = false;

}