#pragma once

#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

class APlayerCameraManager;

class UCameraModifier : public UObject
{
    DECLARE_CLASS(UCameraModifier, UObject)

public:
    UCameraModifier() = default;

private:
    APlayerCameraManager* CameraOwner;
    float AlphaInTime;
    float AlphaOutTime;
    float Alpha;
    uint32 bDisabled;
    uint8 Priority;


};

