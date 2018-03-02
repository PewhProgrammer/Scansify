//------------------------------------------------------------------------------
// <copyright file="svghelper.cpp">
//     Copyright (c) 2018.  All rights reserved. 
//	   Author: Thinh Tran
// </copyright>
//------------------------------------------------------------------------------

#include "svghelper.h"
#include <algorithm>

SvgHelper::~SvgHelper(){}

void SvgHelper::addData(float x, float y)
{
	this->m_vSvgData.push_back(pair<float,float>(x,y));
	if (m_uWidth.first < x) m_uWidth.first = (int16_t)std::ceil(x);
	if (m_uWidth.second > x) m_uWidth.second = (int16_t)std::ceil(x);

	if (m_uHeight.first < y) m_uHeight.first = (int16_t)std::ceil(y);
	if (m_uHeight.second > y) m_uHeight.second = (int16_t)std::ceil(y);

	if (x < 0) m_bOffsetFlagX = true;
	if (y < 0) m_bOffsetFlagY = true;
}

uint2 SvgHelper::getDimensions()
{
	//first is low
	return uint2(
		std::max(m_uWidth.first,	std::abs((int)m_uWidth.first)	+		std::abs((int)m_uWidth.second)	),
		std::max(m_uHeight.first,	std::abs((int)m_uHeight.first)	+		std::abs((int)m_uHeight.second) ));
}

pair<int16_2, int16_2> SvgHelper::getDimensionsRange()
{
	return pair<int16_2, int16_2>(this->m_uWidth,this->m_uHeight);
}

vector<f2> SvgHelper::getData()
{
	return this->m_vSvgData;
}

string SvgHelper::getHeader()
{
	/*
	return 
		"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
		"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n"
		" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
		"<!-- ##################################\n     # SVG file created by Scansify   "
		"#\n     # Author: Thinh Tran             #\n     ################################## -->\n\n"
		"<svg width=\" " + std::to_string(getDimensions().first) +
		" \" height=\" " + std::to_string(getDimensions().second) + " \" version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\" "
		" >\n<title>Annotated Pattern</title>\n\n";
	*/

	return
		"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
		"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n"
		" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
		"<!-- ##################################\n     # SVG file created by Scansify   "
		"#\n     # Author: Thinh Tran             #\n     ################################## -->\n\n"
		"<svg width=\" 100%"
		" \" height=\" 100% \" version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\" "
		" >\n<title>Annotated Pattern</title>\n\n";
}

string SvgHelper::getFooter()
{
	return this->footer;
}

string SvgHelper::getStyles()
{
	m_style.stroke;
	string result = "style=\"stroke:rgb(" 
		+ std::to_string(get<0>(m_style.stroke)) + ", "
		+ std::to_string(get<1>(m_style.stroke))+", "
		+ std::to_string(get<2>(m_style.stroke)) + "); "
		"stroke-width:"+ std::to_string(m_style.stroke_width)  +"; "
		"fill:"+ m_style.fill + " "
		"\"";
	return result;
}

float SvgHelper::getX(unsigned int index)
{
	int offset = 100;
	float result = this->m_vSvgData[index].first + offset;

	if (!m_bOffsetFlagX)
		result += +std::abs(this->m_uWidth.second);

	return result * 2.5f;
}

float SvgHelper::getY(unsigned int index)
{
	int offset = 100;
	float result = this->m_vSvgData[index].second + offset;
	if (!m_bOffsetFlagY)
		result += std::abs(this->m_uHeight.second);

	return result * 2.5f;
}

bool signX = true;
bool dir_result = true;

/// <summary>
/// Getter for DirectionFlag of how to flatten the 3d shape into 2d plane
/// <param name="angle">change of angle represents turn of momentum</param>
/// <param name="dirX">the current direction the path is facing at</param>
/// </summary>
/// <returns>return the appropriate case; true indicates direction in positive axis</returns>
bool SvgHelper::getDirectionX(float angle, float dirX)
{
	// Basically has to know when the angle turn point is and adjust

	m_bPosX = dirX < 0 ? false : true;

	if ((dirX < 0 && !signX) || (dirX > 0 && signX)) {
		m_bDirectionFlagX = false;
	}

	// toggle change of momentum; check for momentum change
	if (!m_bDirectionFlagX && (angle < 0 || angle > 0)) {
		m_bDirectionFlagX = true;
		if (angle < 0) signX = false;
		if (angle > 0) signX = true;
	}

	if (m_bDirectionFlagX) {
		// if angle is positive before
		if (signX) {
			if (angle < 0) {
				// momentum changes, change state => become normal state
				signX = false;
				dir_result = !dir_result;
			}
		}
		else {
			if (angle > 0) {
				// momentum changes from negative to positive, change state => become normal state
				signX = true;
				dir_result = !dir_result;
			}
		}
	}

	return dir_result;

}
