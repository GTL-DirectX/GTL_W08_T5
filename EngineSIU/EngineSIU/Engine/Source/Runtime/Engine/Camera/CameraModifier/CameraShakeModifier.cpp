#include "CameraShakeModifier.h"
#include "Camera/CameraTypes.h"

UCameraShakeModifier::UCameraShakeModifier()
{
    // 0 ~ 2π 사이에서 랜덤하게 위상 설정
    PhaseX = FMath::FRandRange(0.f, 2 * PI);
    PhaseY = FMath::FRandRange(0.f, 2 * PI);
    PhaseZ = FMath::FRandRange(0.f, 2 * PI);

    PhasePitch = FMath::FRandRange(0.f, 2 * PI);
    PhaseYaw = FMath::FRandRange(0.f, 2 * PI);
    PhaseRoll = FMath::FRandRange(0.f, 2 * PI);

    // 초기 시간은 0
    ElapsedTime = 0.f;
}

bool UCameraShakeModifier::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
    ElapsedTime = FMath::Min(ElapsedTime + DeltaTime, Duration);
    if (ElapsedTime >= Duration)
        return false;  // Modifier 제거

    // 0~1 정규화
    float inT = FMath::Clamp(ElapsedTime / BlendInTime, 0.f, 1.f);
    float outT = FMath::Clamp((Duration - ElapsedTime) / BlendOutTime, 0.f, 1.f);
    float blendAlpha = InCurve ? InCurve->Evaluate(inT) : inT;
    blendAlpha *= OutCurve ? OutCurve->Evaluate(outT) : outT;
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
