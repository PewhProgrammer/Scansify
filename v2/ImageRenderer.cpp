//------------------------------------------------------------------------------
// <copyright file="ImageRenderer.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved. Additional modification by Thinh Tran
// </copyright>
//------------------------------------------------------------------------------

#include "KinectFusion/stdafx.h"
#include "ImageRenderer.h"

/// <summary>
/// Constructor
/// </summary>
ImageRenderer::ImageRenderer() : 
    m_hWnd(0),
    m_sourceWidth(0),
    m_sourceHeight(0),
    m_sourceStride(0),
    m_pD2DFactory(NULL), 
    m_pRenderTarget(NULL),
    m_pBitmap(0)
{
}

/// <summary>
/// Destructor
/// </summary>
ImageRenderer::~ImageRenderer()
{
    DiscardResources();
    SafeRelease(m_pD2DFactory);
}

/// <summary>
/// Ensure necessary Direct2d resources are created
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT ImageRenderer::EnsureResources()
{
    HRESULT hr = S_OK;

    if (NULL == m_pRenderTarget)
    {
        D2D1_SIZE_U size = D2D1::SizeU(m_sourceWidth, m_sourceHeight);

        D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
        rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
        rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

        // Create a hWnd render target, in order to render to the window set in initialize
        hr = m_pD2DFactory->CreateHwndRenderTarget(
            rtProps,
            D2D1::HwndRenderTargetProperties(m_hWnd, size),
            &m_pRenderTarget
            );

        if ( FAILED(hr) )
        {
            return hr;
        }

        // Create a bitmap that we can copy image data into and then render to the target
        hr = m_pRenderTarget->CreateBitmap(
            size, 
            D2D1::BitmapProperties( D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE) ),
            &m_pBitmap 
            );

        if ( FAILED(hr) )
        {
            SafeRelease(m_pRenderTarget);
            return hr;
        }
    }

    return hr;
}

/// <summary>
/// Dispose of Direct2d resources 
/// </summary>
void ImageRenderer::DiscardResources()
{
    SafeRelease(m_pRenderTarget);
    SafeRelease(m_pBitmap);
}

/// <summary>
/// Set the window to draw to as well as the video format
/// Implied bits per pixel is 32
/// </summary>
/// <param name="hWnd">window to draw to</param>
/// <param name="pD2DFactory">already created D2D factory object</param>
/// <param name="sourceWidth">width (in pixels) of image data to be drawn</param>
/// <param name="sourceHeight">height (in pixels) of image data to be drawn</param>
/// <param name="sourceStride">length (in bytes) of a single scanline</param>
/// <returns>indicates success or failure</returns>
HRESULT ImageRenderer::Initialize(HWND hWnd, ID2D1Factory* pD2DFactory, int sourceWidth, int sourceHeight, int sourceStride)
{
    if (NULL == pD2DFactory)
    {
        return E_INVALIDARG;
    }

    m_hWnd = hWnd;

    // One factory for the entire application so save a pointer here
    m_pD2DFactory = pD2DFactory;

    m_pD2DFactory->AddRef();

    // Get the frame size
    m_sourceWidth  = sourceWidth;
    m_sourceHeight = sourceHeight;
    m_sourceStride = sourceStride;

    return S_OK;
}

/// <summary>
/// Draws a 32 bit per pixel image of previously specified width, height, and stride to the associated hwnd
/// </summary>
/// <param name="pImage">image data in RGBX format</param>
/// <param name="cbImage">size of image data in bytes</param>
/// <returns>indicates success or failure</returns>
HRESULT ImageRenderer::Draw(BYTE* pImage, unsigned long cbImage)
{
    // incorrectly sized image data passed in
    if ( cbImage < ((m_sourceHeight - 1) * m_sourceStride) + (m_sourceWidth * 4) )
    {
        return E_INVALIDARG;
    }

    // create the resources for this draw device
    // they will be recreated if previously lost
    HRESULT hr = EnsureResources();

    if ( FAILED(hr) )
    {
        return hr;
    }
    
    // Copy the image that was passed in into the direct2d bitmap
    hr = m_pBitmap->CopyFromMemory(NULL, pImage, m_sourceStride);

    if ( FAILED(hr) )
    {
        return hr;
    }
       
    m_pRenderTarget->BeginDraw();

    // Draw the bitmap stretched to the size of the window
    m_pRenderTarget->DrawBitmap(m_pBitmap);

	ID2D1SolidColorBrush *pBrush;
	float radius = 5.5f;
	float x = m_sourceWidth * 0.5f;
	D2D1_ELLIPSE e = D2D1::Ellipse(D2D1::Point2F(x, x), radius, radius);
	const D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::SkyBlue);
	hr = m_pRenderTarget->CreateSolidColorBrush(color, &pBrush);
	m_pRenderTarget->FillEllipse(e, pBrush); // could also be DrawEllipse to draw outlier

	// m_pRenderTarget->DrawLine(D2D1::Point2F(100,100),D2D1::Point2F(100,100), pBrush);

    hr = m_pRenderTarget->EndDraw();

    // Device lost, need to recreate the render target
    // We'll dispose it now and retry drawing
    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardResources();
    }

    return hr;
}


/// <summary>
/// Draws a 32 bit per pixel image of previously specified width, height, and stride to the associated hwnd
/// </summary>
/// <param name="pImage">image data in RGBX format</param>
/// <param name="cbImage">size of image data in bytes</param>
/// <returns>indicates success or failure</returns>
HRESULT ImageRenderer::DrawSVG(SvgHelper* svg)
{
	auto dim = svg->getDimensions();
	auto data = svg->getData();
	size_t dataSize = data.size();
	
	// incorrectly sized image data passed in
	if (dim.first == INT_FAST16_MIN || dim.second == INT_FAST16_MAX)
	{
		return E_INVALIDARG;
	}

	// create the resources for this draw device
	// they will be recreated if previously lost
	HRESULT hr = EnsureResources();

	if (FAILED(hr)){return hr;}

	m_pRenderTarget->BeginDraw();

	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	ID2D1SolidColorBrush *pBrush;
	float radius = 5.5f;
	float x = m_sourceWidth * 0.5f;
	float scale = 5.f;
	D2D1_ELLIPSE e = D2D1::Ellipse(D2D1::Point2F(x, x), radius, radius);
	D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::SkyBlue);
	hr = m_pRenderTarget->CreateSolidColorBrush(color, &pBrush);
	if (FAILED(hr)) { return hr; }

	// interpolate svg data into frame dimensions
	// base formula for range interpolation: Result := ((Input - InputLow) / (InputHigh - InputLow)) * (OutputHigh - OutputLow) + OutputLow;
	
	auto dimRange = svg->getDimensionsRange();
	UINT offsetY = 65;
	UINT offsetX = 35;

	// iterate over svg data points and draw them
	for (int i = 1; i < dataSize; i++) {

		auto x1 = (data[i - 1].first + offsetX )	* scale;
		auto y1 = (data[i - 1].second + offsetY)	* scale;
		auto x2 = (data[i].first + offsetX)			* scale;
		auto y2 = (data[i].second + offsetY)		* scale;

		m_pRenderTarget->DrawLine(
			D2D1::Point2F(x1,y1),
			D2D1::Point2F(x2,y2),
			pBrush);
	}

	// only if prev exists
	if (dataSize > 1) {
		color = D2D1::ColorF(D2D1::ColorF::IndianRed);
		hr = m_pRenderTarget->CreateSolidColorBrush(color, &pBrush);
		if (FAILED(hr)) { return hr; }
		m_pRenderTarget->DrawLine(D2D1::Point2F((data[dataSize - 1].first + offsetX) * scale, (data[dataSize - 1].second + offsetY) * scale),
			D2D1::Point2F((data[0].first + offsetX) * scale, (data[0].second + offsetY) * scale), pBrush);
	}

	hr = m_pRenderTarget->EndDraw();

	// Device lost, need to recreate the render target
	// We'll dispose it now and retry drawing
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardResources();
	}

	return hr;
}