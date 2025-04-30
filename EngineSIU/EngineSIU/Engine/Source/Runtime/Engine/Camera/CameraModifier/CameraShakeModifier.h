#pragma once
#include "Camera/CameraModifier.h"

struct FMinimalViewInfo;
struct Oscillator
{
    float Amplitude = 0.0f;
    float Frequency = 0.0f;
    float Phase;
};

class UCameraShakeModifier : public UCameraModifier
{
    DECLARE_CLASS(UCameraShakeModifier, UCameraModifier)

public:
    UCameraShakeModifier() = default;

    bool ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV);
private:
    float Duration = 0.5f;
    float BlendInTime = 0.1f;
    float BlendOutTime = 0.1f;
    float ElapsedTime = 0.0f;
    float Scale = 1.0f;

    // 축별 위치/회전 Oscillation
    Oscillator LocX, LocY, LocZ;
    Oscillator RotPitch, RotYaw, RotRoll;

    // 랜덤 위상(Phase) 부여
    float PhaseX, PhaseY, PhaseZ;
    float PhasePitch, PhaseYaw, PhaseRoll;

    std::function<float(float)> InCurve;
    std::function<float(float)> OutCurve;

};

