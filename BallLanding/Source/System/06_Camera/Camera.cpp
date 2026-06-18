#include "Camera.h"
#include <cmath>

#include "SceneManager/SceneManager.h"

//定数宣言用.
namespace Numbers
{
    const float Zero = 0.0f;
    const float One = 1.0f;
    const float OnePointFive = 1.5f;
}

Camera::Camera()
    : m_angleX(0.0f)
    , m_angleY(0.0f)
    , m_sensitivity(0.0006f)
    , m_isActive(true)

    , m_mode(CameraMode::ThirdPerson)
    , m_Position()

    , m_PrevMousePos()
    , m_isMouseInitialized(false)
{
    m_offset = D3DXVECTOR3(0.0f, 3.0f, -12.0f);    // Xを0にする（真後ろ）
    D3DXMatrixIdentity(&m_viewMatrix);
}

Camera::~Camera()
{
}

void Camera::Clear()
{
}

void Camera::Update(const D3DXVECTOR3& targetPos)
{
    if (!m_isActive) return;

    if (SceneManager::GetInstance()->IsPause())
    {
        //ImGuiを触れるように.
        return;
    }

    switch (m_mode)
    {
    case CameraMode::ThirdPerson:
        UpdateThirdPerson(targetPos);
        break;

    case CameraMode::FirstPerson:
        UpdateFirstPerson(targetPos);
        break;
    case CameraMode::MouseCamera:
        UpdateMouseView(targetPos);
        break;
    }

    if (m_shakeTimer > 0.0f)
    {
        // タイマー減少（Timer::GetInstance().DeltaTime() などがあればそれを使う）
        // ここでは仮に 1/60秒 ずつ減らしますが、お使いのDeltaTimeに差し替えてください
        float dt = 0.016f;
        m_shakeTimer -= dt;

        // ランダムに座標をずらす (-1.0 ～ 1.0 の範囲 * 強さ)
        float offsetX = ((rand() % 2001) - 1000) / 1000.0f * m_shakeIntensity;
        float offsetY = ((rand() % 2001) - 1000) / 1000.0f * m_shakeIntensity;
        float offsetZ = ((rand() % 2001) - 1000) / 1000.0f * m_shakeIntensity;

        m_Position += D3DXVECTOR3(offsetX, offsetY, offsetZ);
    }
}

#if 0
bool Camera::IsInView(const D3DXVECTOR3& targetPos, float forThreshold) const
{
    //カメラからターゲットへの方向ベクトル.
    D3DXVECTOR3 ToTarget = targetPos - m_Position;
    //向きを求める.
    D3DXVec3Normalize(&ToTarget, &ToTarget);

    //カメラの現在の正面方向を取得[UpdateMouseView()でのカメラの計算に合わせる].
    //回転情報を格納するための空の箱を用意する[4 * 4].
    D3DXMATRIX MatRot;
    D3DXMatrixRotationYawPitchRoll(
        &MatRot,    //計算結果を格納する場所.
        m_angleX,   //Yaw : Y軸回りの回転.
        m_angleY,   //Pitch : X軸周りの回転.
        0.0f);      //Roll : Z軸回りの回転.
    D3DXVECTOR3 ForWard(0, 0, 1);
    D3DXVec3TransformNormal(&ForWard, &ForWard, &MatRot);

    //内角で角度の差を計算[1.0なら完全正面, 0.0なら真横].
    float Dot = D3DXVec3Dot(&ForWard, &ToTarget);
    //画面の広さで調整させる.
    return forThreshold < Dot;
}
#else
bool Camera::IsInView(const D3DXVECTOR3& targetPos, float cosThreshold) const
{
    // カメラ → ターゲット
    D3DXVECTOR3 toTarget = targetPos - m_Position;
    D3DXVec3Normalize(&toTarget, &toTarget);

    // カメラの前方向（Yaw + Pitch）
    D3DXMATRIX rot;
    D3DXMatrixRotationYawPitchRoll(
        &rot,
        m_angleY, // Yaw
        m_angleX, // Pitch
        0.0f
    );

    D3DXVECTOR3 forward(0, 0, 1);
    D3DXVec3TransformNormal(&forward, &forward, &rot);
    D3DXVec3Normalize(&forward, &forward);

    float dot = D3DXVec3Dot(&forward, &toTarget);

    return dot >= cosThreshold;
}

#endif
D3DXVECTOR3 Camera::GetForward() const
{
    D3DXMATRIX rot;
    D3DXMatrixRotationYawPitchRoll(
        &rot,
        m_angleY, // Yaw（左右）
        m_angleX, // Pitch（上下）
        0.0f
    );

    D3DXVECTOR3 forward(0.0f, 0.0f, 1.0f);
    D3DXVec3TransformNormal(&forward, &forward, &rot);
    D3DXVec3Normalize(&forward, &forward);

    return forward;
}

D3DXVECTOR3 Camera::GetRight() const
{
    D3DXMATRIX rot;
    D3DXMatrixRotationYawPitchRoll(
        &rot,
        m_angleY,
        m_angleX,
        0.0f
    );

    D3DXVECTOR3 right(1.0f, 0.0f, 0.0f);
    D3DXVec3TransformNormal(&right, &right, &rot);
    D3DXVec3Normalize(&right, &right);

    return right;
}

void Camera::UpdateThirdPerson(const D3DXVECTOR3& targetPos)
{
    // プレイヤーから受け取った Yaw(m_angleY) と Pitch(m_angleX) を使う
        // Z（ロール）の回転は入れないのがエスコン風の安定したカメラ
    D3DXMATRIX matRot;
    D3DXMatrixRotationYawPitchRoll(&matRot, m_angleY, m_angleX, 0.0f);

    // オフセット（0, 3, -12など）を現在の角度で回転させる
    D3DXVECTOR3 transformedOffset;
    D3DXVec3TransformCoord(&transformedOffset, &m_offset, &matRot);

    // カメラの座標 = プレイヤー位置 + 回転したオフセット
    m_Position = targetPos + transformedOffset;

    // 注視点：プレイヤーの少し上を見るように設定
    D3DXVECTOR3 lookAt = targetPos + D3DXVECTOR3(0.0f, 1.5f, 0.0f);

    // 上方向ベクトル：常に世界の上(Y軸)を「上」とする
    D3DXVECTOR3 up(0, 1, 0);

    D3DXMatrixLookAtLH(&m_viewMatrix, &m_Position, &lookAt, &up);

    if (m_shakeTimer > 0.0f)
    {
        float shakeX = ((rand() % 2001) - 1000) / 1000.0f * m_shakeIntensity;
        float shakeY = ((rand() % 2001) - 1000) / 1000.0f * m_shakeIntensity;

        // ビュー行列の移動成分に直接足す（カメラのローカル移動に見える）
        m_viewMatrix._41 += shakeX;
        m_viewMatrix._42 += shakeY;
    }
}

void Camera::UpdateFirstPerson(const D3DXVECTOR3& targetPos)
{
    D3DXMATRIX matRot;
    D3DXMatrixRotationYawPitchRoll(&matRot, m_angleY, m_angleX, Numbers::Zero);

    // 目線の位置.
    m_Position = targetPos + D3DXVECTOR3(Numbers::Zero, Numbers::OnePointFive, Numbers::Zero);

    //正面方向ベクトルを回転させる.
    D3DXVECTOR3 forward(0, 0, 1);
    D3DXVec3TransformNormal(&forward, &forward, &matRot);
    //注視点 = 現在地 + 向いている方向.
    D3DXVECTOR3 lookAt = m_Position + forward;
    D3DXVECTOR3 up(0, 1, 0);
    //上方向ベクトルも回転に合わせる.
    D3DXVec3TransformNormal(&up, &up, &matRot);
    
    D3DXMatrixLookAtLH(&m_viewMatrix, &m_Position, &lookAt, &up);
}

void Camera::UpdateMouseView(const D3DXVECTOR3& targetPos)
{
    //マウス座標の取得.
    POINT currentPos;
    GetCursorPos(&currentPos);

    //カメラの飛び跳ねをなくすよう.
   if (!m_isMouseInitialized) 
   {
        m_PrevMousePos = currentPos;
        m_isMouseInitialized = true; 
        return;
    }

    //X,Y方向のマウスの感度.
    float dx = static_cast<float>(currentPos.x - m_PrevMousePos.x);
    float dy = static_cast<float>(currentPos.y - m_PrevMousePos.y);

    m_angleY += dx * m_sensitivity;
    m_angleX += dy * m_sensitivity;

    //角度制限.
    const float limit = D3DX_PI * 0.45f;
    if (m_angleX > limit)  m_angleX = limit;
    if (m_angleX < -limit) m_angleX = -limit;

    //マウスカーソルをパソコンの画面中央に設定.
    int centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
    int centerY = GetSystemMetrics(SM_CYSCREEN) / 2;
    SetCursorPos(centerX, centerY);

    //中央の座標を固定させて保存している.
    m_PrevMousePos.x = centerX;
    m_PrevMousePos.y = centerY;

    D3DXMATRIX matRot;
    D3DXMatrixRotationYawPitchRoll(&matRot, m_angleY, m_angleX, Numbers::Zero);

    D3DXVECTOR3 transformedOffset;
    D3DXVec3TransformCoord(&transformedOffset, &m_offset, &matRot);

    m_Position = targetPos + transformedOffset;
    D3DXVECTOR3 lookAt = targetPos + D3DXVECTOR3(Numbers::Zero, Numbers::One, Numbers::Zero);
    D3DXVECTOR3 up(0, 1, 0);

    D3DXMatrixLookAtLH(&m_viewMatrix, &m_Position, &lookAt, &up);

    DirectX11::GetInstance()->SetViewMatrix(m_viewMatrix);
}