#pragma once

#pragma warning(disable: 4244)

#include <windows.h>
#include <stdio.h>
#include <cmath>
#include <list>
#include <vector>
#include <map>
#include <string>

// 단순히 그림을 그리기 위해 필요한 스펙
#include <d2d1.h>						
#include <d2d1_1.h>						
#include <d2d1helper.h>					// 헬퍼
#include <dwrite.h>						// 텍스트 관련
#include <wincodec.h>					// WIC (이미지 로딩 관련)

// 이펙트를 붙이기 위해 필요한 스펙
#include <d3d11.h>						// 다이렉트 11
#include <dxgi1_2.h>					// DXGI(Direct X Graphic Infrastructure)
										// 다이렉트 11 관련 기능들을 모아둔 헤더
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
	ID2D1Bitmap* m_Bitmap;		// 파티클 비트맵

	D2D1_RECT_F m_SourceRect;	// 파티클 스프라이트 시트에서 불러올 곳

	float m_LifeTime;			// 파티클 자신의 라이프 타임
	BOOL m_IsPlay;				// 실행중인가?


	D2D1_POINT_2F m_Pos;		// 파티클이 생성되어 재생될 위치
	float m_Scale;				// 파티클의 크기
	float m_RocalRotation;		// 파티클은 자기 스스로의 회전 값을 안다.
	float m_Opacity;			// 파티클은 서서히 사라져야한다.

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

	D2D1_RECT_F m_SourceRect;	// 스프라이트 시트에서 불러올 곳
	D2D1_VECTOR_2F m_DrawPos;	// 스프라이트를 그릴 곳
	float m_Opacity;			// 투명도

	int m_Index;
	int m_MaxIndex;
	int m_FrameRate;			// 몇 프레임마다 한 컷씩 넘길지
	int m_FrameCount;			// 프레임이 도달할 때마다 증가

	BOOL m_IsLoop;				// 루프 형태인가?

	int m_Width;
	int m_Height;
};

