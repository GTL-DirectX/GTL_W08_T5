#include "CameraShakeModifier.h"
#include "Camera/CameraTypes.h"

UCameraShakeModifier::UCameraShakeModifier()
{
    // 기본 진폭·빈도 세팅
    LocX.Amplitude = 0.0f;   LocX.Frequency = 5.0f;
    LocY.Amplitude = 0.1f;   LocY.Frequency = 5.0f;
    LocZ.Amplitude = 0.1f;   LocZ.Frequency = 5.0f;

    RotPitch.Amplitude = 0.01f;  RotPitch.Frequency = 5.0f;
    RotYaw.Amplitude = 0.01f;  RotYaw.Frequency = 5.0f;
    RotRoll.Amplitude = 0.0f;  RotRoll.Frequency = 5.0f;

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
    // 1) 경과 시간 누적
    ElapsedTime += DeltaTime;
    if (ElapsedTime >= Duration)
        return false;  // 완전 종료

    // 2) Alpha 결정
    float alpha = 1.0f;
    // 페이드인 중
    if (ElapsedTime < AlphaInTime)
    {
        alpha = ElapsedTime / AlphaInTime;
    }
    // 페이드아웃 시작 전 (유지 구간)
    else if (ElapsedTime < Duration - AlphaOutTime)
    {
        alpha = 1.0f;
    }
    // 페이드아웃 중
    else
    {
        float tOut = (ElapsedTime - (Duration - AlphaOutTime)) / AlphaOutTime;
        alpha = 1.0f - tOut;
    }

    // 3) ShakeScale 계산
    const float ShakeScale = alpha * Scale;

    // 4) 시간 누적 → 오프셋 계산
    auto CalcOffset = [&](const Oscillator& O, float Phase)->float
        {
            return O.Amplitude * ShakeScale
                * FMath::Sin(Phase + ElapsedTime * O.Frequency);
        };

    // 5) ViewTarget.POV 에 적용
    InOutPOV.Location += FVector(
        CalcOffset(LocX, PhaseX),
        CalcOffset(LocY, PhaseY),
        CalcOffset(LocZ, PhaseZ)
    );
    InOutPOV.Rotation += FRotator(
        CalcOffset(RotPitch, PhasePitch),
        CalcOffset(RotYaw, PhaseYaw),
        CalcOffset(RotRoll, PhaseRoll)
    );

    return true;
}
