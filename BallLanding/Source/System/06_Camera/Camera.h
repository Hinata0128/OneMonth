#pragma once
#include <cstdint>

class Camera 
{
public:
    enum class CameraMode : uint8_t
    {
        ThirdPerson,
        FirstPerson,
        MouseCamera
    };
public:
    Camera();
    ~Camera() ;

    void Clear() ;

    void SetMode(CameraMode mode) { m_mode = mode; }
    CameraMode GetMode() const { return m_mode; }

    //ターゲット座標.
    void Update(const D3DXVECTOR3& targetPos);

    //指定した座標がカメラの正面(視界)にあるかのチェック.
    //弾の発射制限角度の取得をしている.
    //第一引数ではターゲットの座標を取得する.
    //第二引数では正面カメラの有効範囲を実装している.
    bool IsInView(const D3DXVECTOR3& targetPos,
        float forThreshold = 0.7) const;

public:
    //行列取得.
    const D3DXMATRIX& GetViewMatrix() const { return m_viewMatrix; }
    D3DXVECTOR3 GetForward() const;
    D3DXVECTOR3 GetRight() const;

    D3DXVECTOR3 GetOffset() const { return m_offset; }

    // 設定・取得用関数（Setter / Getter）
    void SetOffset(const D3DXVECTOR3& offset) { m_offset = offset; }
    void SetAngleX(float angle) { m_angleX = angle; } // これでGameMainから操作可能にする
    float GetAngleX() const { return m_angleX; }

    void SetActive(bool active) { m_isActive = active; }
    bool IsActive() const { return m_isActive; }
    D3DXVECTOR3 GetPosition() const { return m_Position; }
    void SetPosition(const D3DXVECTOR3& pos) { m_Position = pos; }

    float GetAngleY() const { return m_angleY; }
    void SetAngleY(float angle) { m_angleY = angle; }

    void ResetInitFlg() { m_isMouseInitialized = false; }

    //カメラの揺れ実装用の関数.
    void SetShake(float intensity, float duration)
    {
        m_shakeIntensity = intensity;
        m_shakeTimer = duration;
    }
    float m_shakeIntensity = 0.0f; // 揺れの強さ
    float m_shakeTimer = 0.0f;  //残り時間.
private:
    void UpdateThirdPerson(const D3DXVECTOR3& targetPos);
    void UpdateFirstPerson(const D3DXVECTOR3& targetPos);
    void UpdateMouseView(const D3DXVECTOR3& targetPos);
private:

    D3DXMATRIX  m_viewMatrix;
    D3DXVECTOR3 m_offset;
    float       m_angleX;
    float       m_angleY;
    float       m_sensitivity;

    bool m_isActive;

    CameraMode m_mode;
    D3DXVECTOR3 m_Position;
    //マウス位置.
    POINT m_PrevMousePos;
    bool m_isMouseInitialized;
};