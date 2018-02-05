//------------------------------------------------------------------------------
// <copyright file="svghelper.cpp">
//     Copyright (c) 2018.  All rights reserved. 
//	   Author: Thinh Tran
// </copyright>
//------------------------------------------------------------------------------

#include "svghelper.h"

SvgHelper::~SvgHelper(){}

void SvgHelper::addData(float x, float y)
{
	this->m_vSvgData.push_back(pair<float,float>(x,y));
	if (m_uWidth.first < x) m_uWidth.first = x;
	if (m_uWidth.second > x) m_uWidth.second = x;

	if (m_uHeight.first < y) m_uHeight.first = y;
	if (m_uHeight.second > y) m_uHeight.second = y;
}

uint2 SvgHelper::getDimensions()
{
	return uint2(m_uWidth.first, m_uHeight.first);
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
	return 
		"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
		"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n"
		" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
		"<!-- ##################################\n     # SVG file created by Scansify   "
		"#\n     # Author: Thinh Tran             #\n     ################################## -->\n\n"
		"<svg width=\" " + std::to_string(m_uWidth.first) +
		" \" height=\" " + std::to_string(m_uHeight.first) + "\" version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\" "
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
		"stroke-width:"+ std::to_string(m_style.stroke_width)  +" \"" ;
	return result;
}
