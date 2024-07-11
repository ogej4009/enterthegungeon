#pragma once
#include <Windows.h>
#include <crtdbg.h>
#include <functional>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>

using namespace std;

#include "Flag.h"
#include "Math.h"
/*
	�ڵ�� �������̺귯�� �����ϴ� ��� : �ִϸ��̼�
	#pragma comment(lib, "msimg32.lib") : msimg32 : win32���� �����ϴ� ���̺귯��
	�ؽ����� BitBlt�� ������� �ʴ� ���
*/
#pragma comment(lib, "msimg32.lib")

typedef struct _tagResolution
{
	int	iWidth;
	int	iHeight;
}Resolution, * PResolution;

typedef struct _tagPosition
{
	float		x, y;

	_tagPosition()
	{
		x = y = 0.f;
	}

	_tagPosition(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	_tagPosition(int _x, int _y)
	{
		x = (float)_x;
		y = (float)_y;
	}

	_tagPosition(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	_tagPosition(const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}

	void operator = (const _tagPosition& tPos)
	{
		x = tPos.x;
		y = tPos.y;
	}

	void operator = (const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}

	void operator = (float f)
	{
		x = f;
		y = f;
	}

	void operator = (int i)
	{
		x = (float)i;
		y = (float)i;
	}

	// +
	_tagPosition operator + (const _tagPosition& tPos)	const
	{
		_tagPosition	t;
		t.x = x + tPos.x;
		t.y = y + tPos.y;
		return t;
	}

	_tagPosition operator + (const POINT& tPos)	const
	{
		_tagPosition	t;
		t.x = x + (float)tPos.x;
		t.y = y + (float)tPos.y;
		return t;
	}

	_tagPosition operator + (float f)	const
	{
		_tagPosition	t;
		t.x = x + f;
		t.y = y + f;
		return t;
	}

	_tagPosition operator + (int i)	const
	{
		_tagPosition	t;
		t.x = x + (float)i;
		t.y = y + (float)i;
		return t;
	}

	void operator += (const _tagPosition& tPos)
	{
		x += tPos.x;
		y += tPos.y;
	}

	_tagPosition operator += (const POINT& tPos)
	{
		x += (float)tPos.x;
		y += (float)tPos.y;
	}

	_tagPosition operator += (float f)
	{
		x += f;
		y += f;
	}

	_tagPosition operator += (int i)
	{
		x += (float)i;
		y += (float)i;
	}

	// -
	_tagPosition operator - (const _tagPosition& tPos)	const
	{
		_tagPosition	t;
		t.x = x - tPos.x;
		t.y = y - tPos.y;
		return t;
	}

	_tagPosition operator - (const POINT& tPos)	const
	{
		_tagPosition	t;
		t.x = x - (float)tPos.x;
		t.y = y - (float)tPos.y;
		return t;
	}

	_tagPosition operator - (float f)	const
	{
		_tagPosition	t;
		t.x = x - f;
		t.y = y - f;
		return t;
	}

	_tagPosition operator - (int i)	const
	{
		_tagPosition	t;
		t.x = x - (float)i;
		t.y = y - (float)i;
		return t;
	}

	void operator -= (const _tagPosition& tPos)
	{
		x -= tPos.x;
		y -= tPos.y;
	}

	_tagPosition operator -= (const POINT& tPos)
	{
		x -= (float)tPos.x;
		y -= (float)tPos.y;
	}

	_tagPosition operator -= (float f)
	{
		x -= f;
		y -= f;
	}

	_tagPosition operator -= (int i)
	{
		x -= (float)i;
		y -= (float)i;
	}

	// *
	_tagPosition operator * (const _tagPosition& tPos)	const
	{
		_tagPosition	t;
		t.x = x * tPos.x;
		t.y = y * tPos.y;
		return t;
	}

	_tagPosition operator * (const POINT& tPos)	const
	{
		_tagPosition	t;
		t.x = x * (float)tPos.x;
		t.y = y * (float)tPos.y;
		return t;
	}

	_tagPosition operator * (float f)	const
	{
		_tagPosition	t;
		t.x = x * f;
		t.y = y * f;
		return t;
	}

	_tagPosition operator * (int i)	const
	{
		_tagPosition	t;
		t.x = x * (float)i;
		t.y = y * (float)i;
		return t;
	}

	void operator *= (const _tagPosition& tPos)
	{
		x *= tPos.x;
		y *= tPos.y;
	}

	_tagPosition operator *= (const POINT& tPos)
	{
		x *= (float)tPos.x;
		y *= (float)tPos.y;
	}

	_tagPosition operator *= (float f)
	{
		x *= f;
		y *= f;
	}

	_tagPosition operator *= (int i)
	{
		x *= (float)i;
		y *= (float)i;
	}

	// /
	_tagPosition operator / (const _tagPosition& tPos)	const
	{
		_tagPosition	t;
		t.x = x / tPos.x;
		t.y = y / tPos.y;
		return t;
	}

	_tagPosition operator / (const POINT& tPos)	const
	{
		_tagPosition	t;
		t.x = x / (float)tPos.x;
		t.y = y / (float)tPos.y;
		return t;
	}

	_tagPosition operator / (float f)	const
	{
		_tagPosition	t;
		t.x = x / f;
		t.y = y / f;
		return t;
	}

	_tagPosition operator / (int i)	const
	{
		_tagPosition	t;
		t.x = x / (float)i;
		t.y = y / (float)i;
		return t;
	}

	void operator /= (const _tagPosition& tPos)
	{
		x /= tPos.x;
		y /= tPos.y;
	}

	_tagPosition operator /= (const POINT& tPos)
	{
		x /= (float)tPos.x;
		y /= (float)tPos.y;
	}

	_tagPosition operator /= (float f)
	{
		x /= f;
		y /= f;
	}

	_tagPosition operator /= (int i)
	{
		x /= (float)i;
		y /= (float)i;
	}

	void operator ++ ()
	{
		x += 1.f;
		y += 1.f;
	}

	void operator ++ (int)
	{
		x += 1.f;
		y += 1.f;
	}

	void operator -- ()
	{
		x -= 1.f;
		y -= 1.f;
	}

	void operator -- (int)
	{
		x -= 1.f;
		y -= 1.f;
	}

	float Distance(const _tagPosition& tPos)	const
	{
		float	fWidth = x - tPos.x;
		float	fHeight = y - tPos.y;
		// sqrtf : ��Ʈ�� ���ϴ� �Լ�
		return sqrtf(fWidth * fWidth + fHeight * fHeight);
	}
}Position, * PPosition, Size, * PSize;

typedef struct _tagRectInfo
{
	float	l;
	float	t;
	float	r;
	float	b;

	_tagRectInfo() :
		l(0.f),
		t(0.f),
		r(0.f),
		b(0.f)
	{
	}
}RectInfo, * PRectInfo;

template <typename T>
struct ObjectArray
{
	T* pArray;
	unsigned int	iCapacity;
	unsigned int	iSize;

	ObjectArray()
	{
		iSize = 0;
		iCapacity = 2;

		pArray = new T[iCapacity];
	}

	~ObjectArray()
	{
		SAFE_DELETE_ARRAY(pArray);
	}

	void Add(const T& pObj)
	{
		if (iCapacity == iSize)
		{
			iCapacity *= 2;

			T* pArr = new T[iCapacity];

			for (int i = 0; i < iSize; ++i)
			{
				pArr[i] = pArray[i];
			}

			SAFE_DELETE_ARRAY(pArray);

			pArray = pArr;
		}

		pArray[iSize] = pObj;
		++iSize;
	}
};

// �浹�� ä���� ���¸� ���� 
typedef struct _tagCollisionChannel
{
	string	strName;
	COLLISION_CHANNEL_STATE	eState;

	_tagCollisionChannel()
	{
		eState = CCS_BLOCK;// ����Ʈ�� ��� 
	}
}CollisionChannel, * PCollisionChannel;

// ä�θ�ϰ� ũ�⸦ ���� 
typedef struct _tagCollisionProfile
{
	string	strName;
	PCollisionChannel	pChannelList;
	int					iChannelSize;

	_tagCollisionProfile()
	{
		pChannelList = nullptr;
		iChannelSize = 0;
	}

	~_tagCollisionProfile()
	{
		SAFE_DELETE_ARRAY(pChannelList);
	}
}CollisionProfile, * PCollisionProfile;

typedef struct _tagSphereInfo // �߽����� ���� 
{
	Position	tCenter;
	float		fRadius;

	_tagSphereInfo()
	{
		fRadius = 0.f;
	}

	_tagSphereInfo operator + (const Position& tPos)	const
	{
		_tagSphereInfo	result;
		result.tCenter = tCenter + tPos;
		result.fRadius = fRadius;

		return result;
	}

	void operator += (const Position& tPos)
	{
		tCenter += tPos;
	}

	_tagSphereInfo operator - (const Position& tPos)	const
	{
		_tagSphereInfo	result;
		result.tCenter = tCenter - tPos;
		result.fRadius = fRadius;

		return result;
	}

	void operator -= (const Position& tPos)
	{
		tCenter -= tPos;
	}
}SphereInfo, * PSphereInfo;

typedef struct _tagClipCoord
{
	Position	tStart;
	Position	tEnd;
}ClipCoord, * PClipCoord;

typedef struct _tagAnimationClip
{
	string	strName;
	ANIMATION_TYPE		eType;
	ANIMATION_OPTION	eOption;
	class CTexture* pTexture;
	unsigned int	iColorKey;
	bool			bColorKey;
	int			iFrame;
	int			iStopFrame; // ��
	vector<ClipCoord>	vecCoord;
	float		fTime;		// �ð� ������ �������Ѽ� üũ�Ѵ�.
	float		fPlayTime;	// �� Ŭ���� ���ư� �ð�
	float		fFrameTime;	// 1�������� ���ư� �ð�
	float		fOptionTime;// �ɼ� �����ð� 
	float		fOptionPlayTime; // �ɼ� ����Ǵ� �ð� 

	_tagAnimationClip()
	{
		eType = AT_ATLAS;
		eOption = AO_LOOP;
		pTexture = nullptr;
		iFrame = 0;
		fTime = 0.f;
		fPlayTime = 0.f;
		fFrameTime = 0.f;
		fPlayTime = 0.f;
		fOptionTime = 0.f;
		fOptionPlayTime = 0.f;
		iColorKey = 0;
		iStopFrame = 0; // ���߰� 
		bColorKey = false;
	}
}AnimationClip, * PAnimationClip;
