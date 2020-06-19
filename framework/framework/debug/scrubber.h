#pragma once

#include <Windows.h>
#include "framework_intro_config.h"

#ifdef ENABLE_SCRUBBER

typedef struct rect
{
	float x1, y1, x2, y2;

	rect(float x1, float y1, float x2, float y2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}

	void expand(float amount)
	{
		x1-=amount;
		y1-=amount;
		x2+=amount;
		y2+=amount;
	}

	void contract(float amount)
	{
		x1+=amount;
		y1+=amount;
		x2-=amount;
		y2-=amount;
	}

}rect;

class Scrubber
{
public:
	Scrubber(HWND hwnd);

	void SetWindowSize(int iWidth, int iHeight);
	void Draw(float normalizedScrubberPos);

	void MouseDown();
	void MouseUp();
	void MouseMove(int x, int y);

private:
	// Drawing constants
	static const float kPadding;
	static const float kBorder;
	static const float kHeight;
	static const float kScrubberPadding;
	static const float kScrubberWidth;

	void DrawQuad(rect r);
	void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void DrawTime(float x1, float y1, float x2, float y2);
	void DrawEdgedRect(rect r);
	void DrawEdgedHollowRect(rect r);
	int m_iWindowWidth, m_iWindowHeight;
	float m_fLeft, m_fTop, m_fRight, m_fBottom;
	float m_fPauseLeft, m_fPauseTop, m_fPauseRight, m_fPauseBottom;
	float m_fAvailableScrubberSpace;

	float NormalizedPosToScrubberPos(float fracPos);
	float ScrubberPosToNormalizedPos(float pos);

	float m_fLastNormalizedScrubberPos;

	int m_iMouseX, m_iMouseY;

	bool m_bIsDragging;
	float m_fDraggingOffset;

	HWND hwnd;
};

extern Scrubber* g_pScrubber;
#endif