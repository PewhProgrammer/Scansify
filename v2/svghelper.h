//------------------------------------------------------------------------------
// <copyright file="svghelper.h">
//     Copyright (c) 2018.  All rights reserved. 
//	   Author: Thinh Tran
// </copyright>
//------------------------------------------------------------------------------

#ifndef SVGHELPER_HEADER
#define SVGHELPER_HEADER

// datastructure Includes
#include <iostream>
#include <vector>
#include <string>
#include <tuple>

using namespace std;
typedef pair<float, float> f2;
typedef pair<uint16_t, uint16_t> uint2;
typedef pair<int_fast16_t, int_fast16_t> int16_2;
typedef std::tuple<int, int, int> triple;


/// <summary>
/// Defined styles for lines which can be changed here
/// Possible extensional approach can be added in this class
/// </summary>
struct Styles {
	triple stroke = make_tuple(255, 0, 0);
	uint16_t stroke_width = 1;
	std::string fill = "none";
};

class SvgHelper
{
private:
	vector<f2> m_vSvgData;
	int16_2 m_uWidth;
	int16_2 m_uHeight;

	string footer;
	bool m_bOffsetFlagX;
	bool m_bOffsetFlagY;

	Styles m_style;
public:
	bool m_bDirectionFlagX;
	bool m_bPosX;

	SvgHelper():m_uWidth(INT_FAST16_MIN, INT_FAST16_MAX),m_uHeight(INT_FAST16_MIN, INT_FAST16_MAX), m_bOffsetFlagY(false), m_bOffsetFlagX(false),
	m_bDirectionFlagX(true){

		// prereserve 10k spots for coordinates
		m_vSvgData.reserve(10000);

		// Construct footer for svg file
		footer = "\n</svg>";

		m_style = Styles();
		
	}
	~SvgHelper();

	/// <summary>
	/// adds data into structure. additionally enlargens width and height properties
	/// </summary>
	/// <returns>nothing</returns>
	void						addData(float x, float y);

	/// <summary>
	/// Getter for width and height of svg
	/// </summary>
	/// <returns>first-> width; second -> height</returns>
	uint2						getDimensions();


	/// <summary>
	/// Getter for width and height of svg
	/// </summary>
	/// <returns>first-> width; second -> height</returns>
	pair<int16_2, int16_2>			getDimensionsRange();


	/// <summary>
	/// Getter for complete line information
	/// </summary>
	/// <returns>preserves order; first is x coordinate</returns>
	vector<f2>					getData();


	/// <summary>
	/// Getter for header string. Construct it inside header because of dynamic variable width,height
	/// </summary>
	/// <returns>complete string to insert into svg file</returns>
	string						getHeader();


	/// <summary>
	/// Getter for footer string
	/// </summary>
	/// <returns>complete string to insert into svg file</returns>
	string						getFooter();

	/// <summary>
	/// Getter for styles string for to draw lines
	/// </summary>
	/// <returns>complete string to insert into svg file</returns>
	string						getStyles();

	/// <summary>
	/// Getter for coordinate X
	/// </summary>
	/// <returns>coordinate with appropriate offset</returns>
	float						getX(unsigned int index);

	/// <summary>
	/// Getter for coordinate Y
	/// </summary>
	/// <returns>coordinate with appropriate offset</returns>
	float						getY(unsigned int index);

	/// <summary>
	/// Getter for DirectionFlag of how to flatten the 3d shape into 2d plane
	/// </summary>
	/// <returns>appriopriate case</returns>
	bool						getDirectionX(float angle, float dirX);
};

#endif