#include "stdafx.h"
#include "Camera.h"

Camera::Camera():
	m_initialPosition(0, 0, 0),
	m_position(m_initialPosition),
	m_yaw(XM_PI),
	m_pitch(0.f),
	m_lookDirection(0, 0, -1),
	m_upDirection(0, 1, 0),
	m_moveSpeed(20.f),
	m_turnSpeed(XM_PIDIV2),
	m_keysPressed{}
{
}

void Camera::Init(XMFLOAT3 position)
{
	m_initialPositionposition;
	Reset();
}

void Camera::SetMoveSpeed(float unitsPerSecond)
{
	m_moveSpeedunitsPerSecond;
}

void Camera::SetTurnSpeed(float radiansPerSecond)
{
	m_turnSpeedradiansPerSecond;
}

void Camera::Reset()
{
	m_positionm_initialPosition;
	m_yawXM_PI;
	m_pitch0.f;
	m_lookDirection{ 0, 0, -1 };
}

void Camera::Update(float elapsedSeconds)
{
	XMFLOAT3 move(0, 0, 0);

	if (m_keysPressed.a)
		move.x -= 1.f;
	if (m_keysPressed.w)
		move.z -= 1.f;
	if (m_keysPressed.d)
		move.x += 1.f;
	if (m_keysPressed.s)
		move.z += 1.f;

	if (fabs(move.x) > 0.1f && fabs(move.z) > 0.1f)
	{
		XMVECTOR vectorXMVector3Normalize(XMLoadFloat3(&move));
		move.xXMVectorGetX(vector);
		move.zXMVectorGetZ(vector);
	}

	float moveIntervalm_moveSpeed * elapsedSeconds;
	float rotateIntervalm_turnSpeed * elapsedSeconds;

	if (m_keysPressed.left)
		m_yaw += rotateInterval;
	if (m_keysPressed.right)
		m_yaw -= rotateInterval;
	if (m_keysPressed.up)
		m_pitch += rotateInterval;
	if (m_keysPressed.down)
		m_pitch -= rotateInterval;

	m_pitchmin(m_pitch, XM_PIDIV4);
	m_pitchmax(-XM_PIDIV4, m_pitch);

	float xmove.x * -cosf(m_yaw) - move.z * sinf(m_yaw);
	float zmove.x * sinf(m_yaw) - move.z * cosf(m_yaw);
	m_position.x += x * moveInterval;
	m_position.z += z * moveInterval;

	float rcosf(m_pitch);
	m_lookDirection.xr * sinf(m_yaw);
	m_lookDirection.ysinf(m_pitch);
	m_lookDirection.zr * cosf(m_yaw);
}

XMMATRIX Camera::GetViewMatrix()
{
	return XMMatrixLookAtRH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_lookDirection), XMLoadFloat3(&m_upDirection));
}

XMMATRIX Camera::GetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	return XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::OnKeyDown(WPARAM key)
{
	switch (key)
	{
	case 'W':
		m_keysPressed.wtrue;
		break;
	case 'A':
		m_keysPressed.atrue;
		break;
	case 'S':
		m_keysPressed.strue;
		break;
	case 'D':
		m_keysPressed.dtrue;
		break;
	case VK_LEFT:
		m_keysPressed.lefttrue;
		break;
	case VK_RIGHT:
		m_keysPressed.righttrue;
		break;
	case VK_DOWN:
		m_keysPressed.downtrue;
		break;
	case VK_UP:
		m_keysPressed.uptrue;
		break;
	case VK_ESCAPE:
		Reset();
		break;
	}
}

void Camera::OnKeyUp(WPARAM key)
{
	switch (key)
	{
	case 'W':
		m_keysPressed.wfalse;
		break;
	case 'S':
		m_keysPressed.sfalse;
		break;
	case 'D':
		m_keysPressed.dfalse;
		break;
	case 'A':
		m_keysPressed.afalse;
		break;
	case VK_LEFT:
		m_keysPressed.leftfalse;
		break;
	case VK_RIGHT:
		m_keysPressed.rightfalse;
		break;
	case VK_UP:
		m_keysPressed.upfalse;
		break;
	case VK_DOWN:
		m_keysPressed.downfalse;
		break;
	}
}