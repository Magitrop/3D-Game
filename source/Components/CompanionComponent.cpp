#include "CompanionComponent.h"

#include "NotAloneComponent.h"

std::string CompanionComponent::toString() {
    return "im " + value + ", my not alone is " + notAlone->value;
}