#pragma once

#pragma warning(disable: 4244)

#include <windows.h>
#include <stdio.h>
#include <cmath>
#include <list>
#include <vector>
#include <map>
#include <string>

// �ܼ��� �׸��� �׸��� ���� �ʿ��� ����
#include <d2d1.h>						
#include <d2d1_1.h>						
#include <d2d1helper.h>					// ����
#include <dwrite.h>						// �ؽ�Ʈ ����
#include <wincodec.h>					// WIC (�̹��� �ε� ����)

// ����Ʈ�� ���̱� ���� �ʿ��� ����
#include <d3d11.h>						// ���̷�Ʈ 11
#include <dxgi1_2.h>					// DXGI(Direct X Graphic Infrastructure)
										// ���̷�Ʈ 11 ���� ��ɵ��� ��Ƶ� ���
#include <d2d1effects_2.h>				// Effect


#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "windowscodecs")

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

using namespace D2D1;
using namespace std;

struct ParticleInfo
{
	ID2D1Bitmap* m_Bitmap;		// ��ƼŬ ��Ʈ��

	D2D1_RECT_F m_SourceRect;	// ��ƼŬ ��������Ʈ ��Ʈ���� �ҷ��� ��

	float m_LifeTime;			// ��ƼŬ �ڽ��� ������ Ÿ��
	BOOL m_IsPlay;				// �������ΰ�?


	D2D1_POINT_2F m_Pos;		// ��ƼŬ�� �����Ǿ� ����� ��ġ
	float m_Scale;				// ��ƼŬ�� ũ��
	float m_RocalRotation;		// ��ƼŬ�� �ڱ� �������� ȸ�� ���� �ȴ�.
	float m_Opacity;			// ��ƼŬ�� ������ ��������Ѵ�.

};

struct TileInfo
{
	int index;
	int x;
	int y;
	int attribute;
};

struct CA_Animation
{
	ID2D1Bitmap* m_pBitmap;

	D2D1_RECT_F m_SourceRect;	// ��������Ʈ ��Ʈ���� �ҷ��� ��
	D2D1_VECTOR_2F m_DrawPos;	// ��������Ʈ�� �׸� ��
	float m_Opacity;			// ����

	int m_Index;
	int m_MaxIndex;
	int m_FrameRate;			// �� �����Ӹ��� �� �ƾ� �ѱ���
	int m_FrameCount;			// �������� ������ ������ ����

	BOOL m_IsLoop;				// ���� �����ΰ�?

	int m_Width;
	int m_Height;
};

