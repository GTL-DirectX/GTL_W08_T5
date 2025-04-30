#pragma once
#include "Components/SceneComponent.h"

#define MIN_ORTHOZOOM (1.0)  // 2D ortho viewport zoom >= MIN_ORTHOZOOM
#define MAX_ORTHOZOOM (1e25)

enum class CameraProjectionMode : uint8
{
    Perspective,
    Orthographic,
};

class UCameraComponent : public USceneComponent
{
    DECLARE_CLASS(UCameraComponent, USceneComponent)

public:
    UCameraComponent() = default; 

    // 카메라의 위치와 회전 설정
    float GetFieldOfView() const { return ViewFOV; }
    float GetAspectRatio() const { return AspectRatio; }
    float GetNearClip() const { return NearClip; }
    float GetFarClip() const { return FarClip; }
    void SetFOV(float InFOV) { ViewFOV = InFOV; }
    void SetAspectRatio(float InAspectRatio) { AspectRatio = InAspectRatio; }
    void SetNearClip(float InNearClip) { NearClip = InNearClip; }
    void SetFarClip(float InFarClip) { FarClip = InFarClip; }

    float GetOrthoZoom() const { return OrthoZoom; }
    void SetOrthoZoom(float InOrthoZoom) { assert(InOrthoZoom >= MIN_ORTHOZOOM && InOrthoZoom <= MAX_ORTHOZOOM); OrthoZoom = InOrthoZoom; }

    // 카메라의 프로젝션 모드 설정
    void SetProjectionMode(CameraProjectionMode InProjectionMode){ ProjectionMode = InProjectionMode; }
    CameraProjectionMode GetProjectionMode() const { return ProjectionMode; }

    float GetOrthoSize() { return OrthoSize; }
    void SetOrthoSize(float InOrthoSize)
    {
        OrthoSize = InOrthoSize;
        OrthoSize = FMath::Max(OrthoSize, 0.1f);
    }

private:
    // 카메라 정보 
    float ViewFOV = 90.0f;
    float AspectRatio;
    float NearClip = 0.1f;
    float FarClip = 1000.0f;
    float OrthoZoom;
    float OrthoSize = 0.1;

    CameraProjectionMode ProjectionMode = CameraProjectionMode::Perspective;
};
