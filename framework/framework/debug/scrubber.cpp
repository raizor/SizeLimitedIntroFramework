#include "global.h"
#include "scrubber.h"

#ifdef ENABLE_SCRUBBER

#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../gl/gl8k.h"
const float Scrubber::kPadding = 6.0f;
const float Scrubber::kBorder = 1.0f;
const float Scrubber::kHeight = 18.0f;
const float Scrubber::kScrubberPadding = 2.0f;
const float Scrubber::kScrubberWidth = 30;

Scrubber* g_pScrubber = NULL;

extern void SetNormalizedTime(float timeNormalized);

extern void TogglePause(bool pause);
extern bool paused;

Scrubber::Scrubber(HWND hwnd) 
{
	this->hwnd = hwnd;
	m_bIsDragging = false;
	paused = false;
}

void Scrubber::MouseDown()
{
	SetCapture(hwnd);
	if (((float)m_iMouseY >= m_fTop) && ((float)m_iMouseY <= m_fBottom) && ((float)m_iMouseX >= NormalizedPosToScrubberPos(m_fLastNormalizedScrubberPos)) && ((float)m_iMouseX <= NormalizedPosToScrubberPos(m_fLastNormalizedScrubberPos) + kScrubberWidth))
	{
		m_bIsDragging = true;

		m_fDraggingOffset = (float)m_iMouseX - NormalizedPosToScrubberPos(m_fLastNormalizedScrubberPos);
	}
	else if (((float)m_iMouseY >= m_fTop) && ((float)m_iMouseY <= m_fBottom) && ((float)m_iMouseX >= m_fLeft && ((float)m_iMouseX <= m_fRight)))
	{
		float newNormalizedPos = ScrubberPosToNormalizedPos(m_iMouseX - kScrubberWidth/2);

		if (newNormalizedPos < 0.0f)
			newNormalizedPos = 0.0f;
		if (newNormalizedPos > 1.0f)
			newNormalizedPos = 1.0f;

		SetNormalizedTime(newNormalizedPos);
	}
	else if (((float)m_iMouseY >= m_fPauseTop) && ((float)m_iMouseY <= m_fPauseBottom) && ((float)m_iMouseX >= m_fPauseLeft && ((float)m_iMouseX <= m_fPauseRight)))
	{
		TogglePause(!paused);
		//	g_pDemo->Unpause();
		//else
		//	g_pDemo->Pause();
	}

}

void Scrubber::MouseUp()
{
	ReleaseCapture();
	m_bIsDragging = false;
}

void Scrubber::MouseMove(int x, int y)
{
	m_iMouseX = x;
	m_iMouseY = y;

	if (m_bIsDragging)
	{
		float newNormalizedPos = ScrubberPosToNormalizedPos(m_iMouseX - m_fDraggingOffset);

		if (newNormalizedPos < 0.0f)
			newNormalizedPos = 0.0f;
		if (newNormalizedPos > 1.0f)
			newNormalizedPos = 1.0f;


		SetNormalizedTime(newNormalizedPos);
	}
}

float Scrubber::NormalizedPosToScrubberPos(float fracPos)
{
	return m_fLeft + kBorder + fracPos*m_fAvailableScrubberSpace + kScrubberPadding;
}

float Scrubber::ScrubberPosToNormalizedPos(float pos)
{
	return (pos - m_fLeft + kBorder + kScrubberPadding) / m_fAvailableScrubberSpace;
}

void Scrubber::SetWindowSize(int iWidth, int iHeight)
{
	m_iWindowWidth = iWidth;
	m_iWindowHeight = iHeight;

	m_fLeft = kPadding*1.5f + kHeight;
	m_fRight = m_iWindowWidth - kPadding;
	m_fTop = m_iWindowHeight - kHeight - kPadding;
	m_fBottom = m_iWindowHeight - kPadding;

	m_fAvailableScrubberSpace = (m_fRight - m_fLeft) - 2*kBorder - 2*kScrubberPadding - kScrubberWidth;

	m_fPauseLeft = kPadding;
	m_fPauseRight = m_fPauseLeft + kHeight;
	m_fPauseTop = m_iWindowHeight - kHeight - kPadding;
	m_fPauseBottom = m_iWindowHeight - kPadding;
}

void Scrubber::DrawQuad(rect r)
{
	glBegin(GL_QUADS);
	glVertex3f(r.x1, r.y1, 0);
	glVertex3f(r.x1, r.y2, 0);
	glVertex3f(r.x2, r.y2, 0);
	glVertex3f(r.x2, r.y1, 0);
	glEnd();
}

void Scrubber::DrawTriangle(float x1, float y1, float x2,  float y2, float x3, float y3)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glVertex3f(x3, y3, 0);
	glEnd();
}

void Scrubber::DrawTime(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x1, y2, 0);
	//glVertex3f(x2, y2, 0);
	//glVertex3f(x2, y1, 0);
	glEnd();
}

void Scrubber::DrawEdgedHollowRect(rect r)
{
	float left = r.x1;
	float right = r.x2;
	float top = r.y1;
	float bottom = r.y2;

	rect rect1 (left-kBorder, top, right, top + kBorder ); // top 
	rect rect2 (left, bottom - kBorder, right, bottom); // bottom
	rect rect3 (left - kBorder, top + kBorder, left, bottom); // left
	rect rect4 (right - kBorder, top + kBorder, right, bottom - kBorder); // right

	glColor4f(0.f, 0.f, 0.f, 0.5f);
	rect1.expand(1.0f);
	rect2.expand(1.0f);
	rect3.expand(1.0f);
	rect4.expand(1.0f);
	DrawQuad(rect1);
	DrawQuad(rect2);
	DrawQuad(rect3);
	DrawQuad(rect4);

	glColor4f(1.f, 1.f, 1.f, 0.5f);
	rect1.contract(1.0f);
	rect2.contract(1.0f);
	rect3.contract(1.0f);
	rect4.contract(1.0f);
	DrawQuad(rect1);
	DrawQuad(rect2);
	DrawQuad(rect3);
	DrawQuad(rect4);

	//rect main2 (r.x1, r.y2 - kBorder, r.x2, r.y2 );
	//rect main3 (r.x1, r.y1, r.x2, r.y2 );
	//rect main4 (r.x1, r.y1, r.x2, r.y2 );
}

void Scrubber::DrawEdgedRect(rect r)
{
	r.expand(1.0f);
	glColor4f(0.f, 0.f, 0.f, 0.5f);
	DrawQuad(r);

	r.contract(1.0f);
	glColor4f(1.f, 1.f, 1.f, 0.5f);
	DrawQuad(r);
}

void Scrubber::Draw(float fNormalizedScrubberPos)
{
	float time = 1.0f;
	// Init normalized view
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, m_iWindowWidth, m_iWindowHeight, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );

	// TODO: deactivate all textures and program

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );

	// Draw black rects
	glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

	// scrubber track		
	rect rectMain (m_fLeft, m_fTop, m_fRight, m_fBottom);
	DrawEdgedHollowRect(rectMain);

	// draw play/pause button
	rect rectButton (m_fPauseLeft, m_fTop, m_fPauseRight, m_fBottom);
	DrawEdgedHollowRect(rectButton);
	
	// Draw scrubber
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

	float scrubberPos= NormalizedPosToScrubberPos(fNormalizedScrubberPos);	
	rect scrubRect (scrubberPos, m_fTop + kBorder + kScrubberPadding, scrubberPos + kScrubberWidth, m_fBottom - kBorder - kScrubberPadding);
	glColor4f(1.f, 1.f, 1.f, 0.5f);
	scrubRect.contract(1.0f);
	DrawEdgedRect(scrubRect);

	float fFifth = (m_fPauseRight - m_fPauseLeft - 2*kBorder) / 5.0f;
		
	if (paused)
	{
		DrawTriangle(m_fPauseLeft + kBorder + fFifth, m_fPauseTop + kBorder + fFifth, m_fPauseLeft + kBorder + fFifth, m_fPauseBottom - kBorder - fFifth, m_fPauseRight - kBorder - fFifth, (m_fPauseTop + m_fPauseBottom)/2.0f);
	}
	else
	{
		// pause 1
		rect rectPause (m_fPauseLeft + kBorder + fFifth, m_fPauseTop + kBorder + fFifth, m_fPauseLeft + kBorder + 2.0f*fFifth, m_fPauseBottom - kBorder - fFifth);
		DrawEdgedRect(rectPause);

		// pause 2
		rect rectPause2 (m_fPauseLeft + kBorder + 3.0f*fFifth, m_fPauseTop + kBorder + fFifth, m_fPauseLeft + kBorder + 4.0f*fFifth, m_fPauseBottom - kBorder - fFifth);
		DrawEdgedRect(rectPause2);
	}	

	// draw time
	//glColor4f(1.f, 1.f, 1.f, 1.f);
	//DrawTime(scrubberPos, m_fTop + kBorder + kScrubberPadding, scrubberPos + kScrubberWidth, m_fBottom - kBorder - kScrubberPadding);

	// Restore state
	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glColor4f(1.f, 1.f, 1.f, 1.f);

	m_fLastNormalizedScrubberPos = fNormalizedScrubberPos;
}
#endif