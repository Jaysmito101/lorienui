#include "lorien/lorien.h"

// TODO: Maybe implemente a better unique ID generator
size_t lorGenerateUniqueId() {
    static size_t uniqueId = 0;
    return uniqueId++;
}