#include "CameraShakeModifier.h"
#include "Camera/PlayerCameraManager.h"

bool UCameraShakeModifier::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
    ElapsedTime = FMath::Min(ElapsedTime + DeltaTime, Duration);
    if (ElapsedTime >= Duration)
        return false;  // Modifier 제거

    // 0~1 정규화
    float inT = FMath::Clamp(ElapsedTime / BlendInTime, 0.f, 1.f);
    float outT = FMath::Clamp((Duration - ElapsedTime) / BlendOutTime, 0.f, 1.f);
    float blendAlpha = InCurve ? InCurve(inT) : inT;
    blendAlpha *= OutCurve ? OutCurve(outT) : outT;
    blendAlpha *= Scale;

    auto calc = [&](const Oscillator& O, float phase)
        {
            return O.Amplitude * blendAlpha
                * FMath::Sin(phase + ElapsedTime * O.Frequency);
        };

    InOutPOV.Location += FVector(calc(LocX, PhaseX),
        calc(LocY, PhaseY),
        calc(LocZ, PhaseZ));

    InOutPOV.Rotation.Pitch += calc(RotPitch, PhasePitch);
    InOutPOV.Rotation.Yaw += calc(RotYaw, PhaseYaw);
    InOutPOV.Rotation.Roll += calc(RotRoll, PhaseRoll);

    return true;

}
