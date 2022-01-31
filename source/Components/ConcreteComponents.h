#ifndef CONCRETECOMPONENTS_H
#define CONCRETECOMPONENTS_H

#include <string>
#include <vector>

#include "Component.h"
#include "GameObject.h"

class IntComponent : public Component {
    COMPONENT(IntComponent)

public:
    int value = 228;
};

class DoubleComponent : public Component {
    COMPONENT(DoubleComponent)

public:
    int value = 1;
};

class StringComponent : public Component {
    COMPONENT(StringComponent)

public:
    std::string value = "fuck you";
};

class PositionComponent : public Component {
    COMPONENT(PositionComponent)

public:
    int position = 0;
};

#endif //CONCRETECOMPONENTS_H
