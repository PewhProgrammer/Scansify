//------------------------------------------------------------------------------
// <copyright file="KinectFusionExplorer.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved. Additional modification by Thinh Tran
// </copyright>
//------------------------------------------------------------------------------

// System includes
#include "KinectFusion/stdafx.h"
#include <iostream>
#include <time.h>
//#include <afxdialogex.h>


// Project includes
#include "resource.h"
#include "main.h"
#include "KinectFusion/KinectFusionProcessorFrame.h"
#include "KinectFusion/KinectFusionHelper.h"

// RayTracing Includes
#include "rt\bvh.h"
#include "rt\cameras\perspective.h"





#define MIN_DEPTH_DISTANCE_MM 500   // Must be greater than 0
#define MAX_DEPTH_DISTANCE_MM 8000
#define MIN_INTEGRATION_WEIGHT 1    // Must be greater than 0
#define MAX_INTEGRATION_WEIGHT 1000

#define WM_FRAMEREADY           (WM_USER + 0)
#define WM_UPDATESENSORSTATUS   (WM_USER + 1)

/// <summary>
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

    Scansify application;
    application.Run(hInstance, nCmdShow);
}

/// <summary>
/// Constructor
/// </summary>
Scansify::Scansify() :
m_hWnd(nullptr),
    m_pD2DFactory(nullptr), 
    m_pDrawReconstruction(nullptr),
    m_pDrawTrackingResiduals(nullptr),
    m_pDrawDepth(nullptr),
    m_bSavingMesh(false),
    m_saveMeshFormat(Stl),
    m_bInitializeError(false),
    m_bColorCaptured(false),
    m_bUIUpdated(false)
{
}

/// <summary>
/// Destructor
/// </summary>
Scansify::~Scansify()
{
	// Release the mesh
	SafeRelease(m_params.m_pMesh);

    // clean up Direct2D renderer
    SAFE_DELETE(m_pDrawReconstruction);

    // clean up Direct2D renderer
    SAFE_DELETE(m_pDrawTrackingResiduals);

    // clean up Direct2D renderer
    SAFE_DELETE(m_pDrawDepth);

    // clean up Direct2D
    SafeRelease(m_pD2DFactory); 
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int Scansify::Run(HINSTANCE hInstance, int nCmdShow)
{
    MSG       msg = {0};
    WNDCLASS  wc  = {0};

    // Dialog custom window class
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbWndExtra    = DLGWINDOWEXTRA;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
    wc.lpfnWndProc   = DefDlgProcW;
    wc.lpszClassName = L"KinectFusionExplorerAppDlgWndClass";

    if (!RegisterClassW(&wc))
    {
        return 0;
    }

    // Create main application window
    HWND hWndApp = CreateDialogParamW(
        hInstance,
        MAKEINTRESOURCE(IDD_APP),
        nullptr,
        (DLGPROC)Scansify::MessageRouter, 
        reinterpret_cast<LPARAM>(this));

	HMENU menu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
	SetMenu(m_hWnd, menu);
	

	//ID_MENU_VIEW_MIRRORDEPTH

    // Show window
    ShowWindow(hWndApp, nCmdShow);
	ShowWindow(hWndApp, SW_SHOWMAXIMIZED);

    // Main message loop
    while (WM_QUIT != msg.message)
    {
        if (GetMessage(&msg, nullptr, 0, 0))
        {
            // If a dialog message will be taken care of by the dialog proc
            if ((hWndApp != nullptr) && IsDialogMessageW(hWndApp, &msg))
            {
                continue;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK Scansify::MessageRouter(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    Scansify* pThis = nullptr;


    if (WM_INITDIALOG == uMsg)
    {
        pThis = reinterpret_cast<Scansify*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    }
    else
    {
        pThis = reinterpret_cast<Scansify*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}


POINT pointPrev;

/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK Scansify::DlgProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{



	// revert latest change
	if (GetKeyState('Z') < 0) {
		if (intersectedNodes.size() > 0) {
			for (std::vector<rt::Node*>::iterator it = intersectedNodes.begin(); it != intersectedNodes.end(); ++it) {
				/* std::cout << *it; ... */
				(*it)->m_bAnnotated = false;
			}
			if (m_vAnnotatedObjects.size() > 0) {
				m_params.m_svgHelper->removeLatestData();
				m_vAnnotatedObjects.pop_back();
			}


			m_processor.SetParams(m_params);
			m_processor.RedrawRenderedImage();
		}
	}

    switch (message)
    {
	case WM_LBUTTONDOWN: {
		GetCursorPos(&pointPrev);
		RECT rect1;
		HWND reconstructionWindow1 = GetDlgItem(m_hWnd, IDC_RECONSTRUCTION_VIEW);
		GetWindowRect(reconstructionWindow1, &rect1);

		ScreenToClient(reconstructionWindow1, &pointPrev);
		}
		break;
	case WM_MOUSEWHEEL: {
		// If annotation mode disabled, disable camera steering
		//if (!m_params.m_bInitializeAnnotationMode) break;

		unsigned int processId = m_params.m_cReconstructionFrameProcessId++;
		while (m_params.m_bReconstructionFrameLock) {
			if (processId != m_params.m_cReconstructionFrameLockId); // sleep
			else break;
		}

		m_params.m_bReconstructionFrameLock = true;

		if ((short)GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			m_processor.ComputeRaytraceCamera(0, 0, -100); // move camera uniformly
		}
		else if ((short)GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			m_processor.ComputeRaytraceCamera(0, 0, 100); // move camera uniformly
		}
		else {
			break;
		}

		m_processor.RedrawRenderedImage();
		m_params.m_cReconstructionFrameLockId = processId + 1;
		m_params.m_bReconstructionFrameLock = false;
	}
	break;
	case WM_LBUTTONUP:{
		// If annotation mode disabled, disable camera steering
		if (!m_params.m_bInitializeAnnotationMode) break;

		POINT point;
		GetCursorPos(&point);
		RECT rect2;
		HWND reconstructionWindow2 = GetDlgItem(m_hWnd, IDC_RECONSTRUCTION_VIEW);
		GetWindowRect(reconstructionWindow2, &rect2);

		ScreenToClient(reconstructionWindow2, &point);

		int diffX = pointPrev.x - point.x;
		int diffY = point.y - pointPrev.y;
		//printf("Distance difference: (%d,%d)\n", diffX, diffY);

		// 1003 is wParam for window click; process lParam as it remains unused
		if (diffX == diffY && diffX == 0) {
			ProcessUI(1003, lParam); break;
		}

		auto key = GetKeyState(VK_LSHIFT); // key handler for left_shift
		//printf("\n");

		if (key < 0) {
			m_processor.ComputeRaytraceCamera(diffX * -1, diffY, 0); // move camera uniformly
		}
		else
			m_processor.ComputeRotationalRaytraceCamera(diffX, diffY);


		m_processor.RedrawRenderedImage();

		}
		break;
    case WM_INITDIALOG:
        {
            // Bind application window handle
            m_hWnd = hWnd;

            InitializeUIControls();

            // Init Direct2D
            D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

            int width = m_params.m_cDepthWidth;
            int height = m_params.m_cDepthHeight;

            // Create and initialize a new Direct2D image renderer (take a look at ImageRenderer.h)
            // We'll use this to draw the data we receive from the Kinect to the screen
            m_pDrawDepth = new ImageRenderer();
            HRESULT hr = m_pDrawDepth->Initialize(
                GetDlgItem(m_hWnd, IDC_DEPTH_VIEW), // retrieves child window to parent
                m_pD2DFactory,
                width,
                height,
                width * sizeof(long));

            if (FAILED(hr))
            {
                SetStatusMessage(L"Failed to initialize the Direct2D draw device for Depth.");
                m_bInitializeError = true;
            }

            m_pDrawReconstruction = new ImageRenderer();
            hr = m_pDrawReconstruction->Initialize(
                GetDlgItem(m_hWnd, IDC_RECONSTRUCTION_VIEW),
                m_pD2DFactory,
                width,
                height,
                width * sizeof(long));

            if (FAILED(hr))
            {
                SetStatusMessage(L"Failed to initialize the Direct2D draw device for Reconstruction.");
                m_bInitializeError = true;
            }

            m_pDrawTrackingResiduals = new ImageRenderer();
            hr = m_pDrawTrackingResiduals->Initialize(
                GetDlgItem(m_hWnd, IDC_TRACKING_RESIDUALS_VIEW),
                m_pD2DFactory,
                width,
                height,
                width * sizeof(long));

            if (FAILED(hr))
            {
                SetStatusMessage(L"Failed to initialize the Direct2D draw device for Tracking Residual.");
                m_bInitializeError = true;
            }

            if (FAILED(m_processor.SetWindow(m_hWnd, WM_FRAMEREADY, WM_UPDATESENSORSTATUS)) ||
                FAILED(m_processor.SetParams(m_params)) ||
                FAILED(m_processor.StartProcessing()))
            {
                m_bInitializeError = true;
            }

            m_saveMeshFormat = m_params.m_saveMeshType;
        }
        break;

        // If the title bar X is clicked, destroy app
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        // Quit the main message pump
        m_processor.StopProcessing();
        PostQuitMessage(0);
        break;

        // Handle button press
    case WM_COMMAND:
        ProcessUI(wParam, lParam);
        break;

        // Handle sliders
    case  WM_HSCROLL:
        UpdateHSliders();
        break;

    case WM_FRAMEREADY:
        HandleCompletedFrame();
        break;
	default:
		break;

    }

    return FALSE;
}

/// <summary>
/// Handle a completed frame from the Kinect Fusion processor.
/// </summary>
/// <returns>S_OK on success, otherwise failure code</returns>
void Scansify::HandleCompletedFrame()
{
    KinectFusionProcessorFrame const* pFrame = nullptr;

    // Flush any extra WM_FRAMEREADY messages from the queue
    MSG msg;
    while (PeekMessage(&msg, m_hWnd, WM_FRAMEREADY, WM_FRAMEREADY, PM_REMOVE)) {}

    m_processor.LockFrame(&pFrame);

    if (!m_bSavingMesh) // don't render while a mesh is being saved
    {
        if (m_processor.IsVolumeInitialized())
        {
			if (m_params.m_bInitializeAnnotationMode) {
				m_pDrawDepth->Draw(pFrame->m_pDepthRGBX, pFrame->m_cbImageSize);

				if (m_processor.ConsumeViewRendered()) {
					// Render the view if raytrace happened before
					m_pDrawReconstruction->Draw(pFrame->m_pTrackingDataRGBX, pFrame->m_cbImageSize);
					m_pDrawReconstruction->DrawAnnotationOnModel(m_processor.ConsumeAnnotationCoordinates()); // if no annotation happen, dont change the current output
				}	

				// TODO move this to the consumeViewRendered Condition. optionally
				m_pDrawTrackingResiduals->DrawSVG(m_params.m_svgHelper);

				// as comparisson
				//m_pDrawTrackingResiduals->Draw(pFrame->m_pReconstructionRGBX, pFrame->m_cbImageSize);
			}
			else {
				m_pDrawDepth->Draw(pFrame->m_pDepthRGBX, pFrame->m_cbImageSize);
				m_pDrawReconstruction->Draw(pFrame->m_pReconstructionRGBX, pFrame->m_cbImageSize);
				m_pDrawTrackingResiduals->Draw(pFrame->m_pTrackingDataRGBX, pFrame->m_cbImageSize);

				m_pDrawReconstruction->DrawScanArea(35, 6);
			}
        }

        SetStatusMessage(pFrame->m_statusMessage);
        SetFramesPerSecond(pFrame->m_fFramesPerSecond);
    }

    if (!m_bUIUpdated && m_processor.IsVolumeInitialized())
    {
        const int Mebi = 1024 * 1024;

        // We now create both a color and depth volume, doubling the required memory, so we restrict
        // which resolution settings the user can choose when the graphics card is limited in memory.
        if (pFrame->m_deviceMemory <= 1 * Mebi)  // 1GB
        {
            // Disable 640 voxel resolution in all axes - cards with only 1GB cannot handle this
            HWND hButton = GetDlgItem(m_hWnd, IDC_VOXELS_X_640);
            EnableWindow(hButton, FALSE);
            hButton = GetDlgItem(m_hWnd, IDC_VOXELS_Y_640);
            EnableWindow(hButton, FALSE);
            hButton = GetDlgItem(m_hWnd, IDC_VOXELS_Z_640);
            EnableWindow(hButton, FALSE);

            if (Is64BitApp() == FALSE)
            {
                // Also disable 512 voxel resolution in one arbitrary axis on 32bit machines
                hButton = GetDlgItem(m_hWnd, IDC_VOXELS_Y_512);
                EnableWindow(hButton, FALSE);
            }
        }
        else if (pFrame->m_deviceMemory <= 2 * Mebi)  // 2GB
        {
            if (Is64BitApp() == FALSE)
            {
                // Disable 640 voxel resolution in one arbitrary axis on 32bit machines
                HWND hButton = GetDlgItem(m_hWnd, IDC_VOXELS_Y_640);
                EnableWindow(hButton, FALSE);
            }
            // True 64 bit apps seem to be more able to cope with large volume sizes.
        }

        m_bUIUpdated = true;
    }

    m_bColorCaptured = pFrame->m_bColorCaptured;

    m_processor.UnlockFrame();
}

/// <summary>
/// Import mesh into tool
/// </summary>
/// <param name="saveMeshType">The mesh type to import.</param>
/// <returns>indicates success or failure</returns>
HRESULT Scansify::ImportMeshFile(KinectFusionMeshTypes saveMeshType) {
	HRESULT hr = S_OK;

	CComPtr<IFileOpenDialog> pSaveDlg;

	// Create the file save dialog object.
	hr = pSaveDlg.CoCreateInstance(__uuidof(FileOpenDialog));

	m_params.m_sceneStructure = new rt::BVH();


	if (FAILED(hr))
	{
		return hr;
	}

	// Set the dialog title
	hr = pSaveDlg->SetTitle(L"Import Kinect Fusion Mesh");
	if (SUCCEEDED(hr))
	{
		// Set the button text
		hr = pSaveDlg->SetOkButtonLabel(L"Import");
		if (SUCCEEDED(hr))
		{
			// Set a default filename
			if (Stl == saveMeshType)
			{
				hr = pSaveDlg->SetFileName(L"arm_model_.stl");
			}
			else if (Obj == saveMeshType)
			{
				hr = pSaveDlg->SetFileName(L"arm_model_.obj");
			}
			else if (Ply == saveMeshType)
			{
				hr = pSaveDlg->SetFileName(L"arm_model_.ply");
			}


			if (SUCCEEDED(hr))
			{
				// Set the file type extension
				if (Stl == saveMeshType)
				{
					hr = pSaveDlg->SetDefaultExtension(L"stl");
				}
				else if (Obj == saveMeshType)
				{
					//hr = pSaveDlg->SetDefaultExtension(L"obj");
				}
				else if (Ply == saveMeshType)
				{
					//hr = pSaveDlg->SetDefaultExtension(L"ply");
				}

				if (SUCCEEDED(hr))
				{
					// Set the file type filters
					if (Stl == saveMeshType)
					{
						COMDLG_FILTERSPEC allPossibleFileTypes[] = {
							{ L"Stl mesh files", L"*.stl" },
						{ L"All files", L"*.*" }
						};

						hr = pSaveDlg->SetFileTypes(
							ARRAYSIZE(allPossibleFileTypes),
							allPossibleFileTypes);
					}
					else if (Obj == saveMeshType)
					{
						COMDLG_FILTERSPEC allPossibleFileTypes[] = {
							{ L"Obj mesh files", L"*.obj" },
						{ L"All files", L"*.*" }
						};

						hr = pSaveDlg->SetFileTypes(
							ARRAYSIZE(allPossibleFileTypes),
							allPossibleFileTypes);
					}
					else if (Ply == saveMeshType)
					{
						COMDLG_FILTERSPEC allPossibleFileTypes[] = {
							{ L"Ply mesh files", L"*.ply" },
						{ L"All files", L"*.*" }
						};

						hr = pSaveDlg->SetFileTypes(
							ARRAYSIZE(allPossibleFileTypes),
							allPossibleFileTypes);
					}

					if (SUCCEEDED(hr))
					{
						// Show the file selection box
						hr = pSaveDlg->Show(m_hWnd);

						// Save the mesh to the chosen file.
						if (SUCCEEDED(hr))
						{
							CComPtr<IShellItem> pItem;
							hr = pSaveDlg->GetResult(&pItem);

							if (SUCCEEDED(hr))
							{
								LPOLESTR pwsz = nullptr;
								hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

								if (SUCCEEDED(hr))
								{
									SetStatusMessage(L"Importing mesh file, please wait...");
									SetCursor(LoadCursor(nullptr, MAKEINTRESOURCE(IDC_WAIT)));

									if (Stl == saveMeshType)
									{
										hr = LoadBinarySTLMeshFile(pwsz, m_params.m_sceneStructure,true);
										//hr = WriteBinarySTLMeshFile(pMesh, pwsz);
									}
									else if (Obj == saveMeshType)
									{
										//hr = WriteAsciiObjMeshFile(pMesh, pwsz);
									}
									else if (Ply == saveMeshType)
									{
										//hr = WriteAsciiPlyMeshFile(pMesh, pwsz, true, m_bColorCaptured);
									}

									CoTaskMemFree(pwsz);
								}
							}
						}
					}
				}
			}
		}
	}

	m_params.m_sceneStructure->buildIndex();
	UpdateMode(Scansify::Mode::Annotation);

	return hr;
}

/// <summary>
/// Save Mesh to disk.
/// </summary>
/// <param name="mesh">The mesh to save.</param>
/// <returns>indicates success or failure</returns>
HRESULT Scansify::SaveMeshFile(INuiFusionColorMesh* pMesh, KinectFusionMeshTypes saveMeshType, bool reconstruction)
{
    HRESULT hr = S_OK;

    if (nullptr == pMesh)
    {
        return E_INVALIDARG;
    }

    CComPtr<IFileSaveDialog> pSaveDlg;

    // Create the file save dialog object.
    hr = pSaveDlg.CoCreateInstance(__uuidof(FileSaveDialog));

    if (FAILED(hr))
    {
        return hr;
    }

    // Set the dialog title
    hr = pSaveDlg->SetTitle(L"Save Kinect Fusion Mesh");
    if (SUCCEEDED(hr))
    {
        // Set the button text
        hr = pSaveDlg->SetOkButtonLabel (L"Import");
        if (SUCCEEDED(hr))
        {
            // Set a default filename
            if (Stl == saveMeshType)
            {
                hr = pSaveDlg->SetFileName(L"arm_model_.stl");
            }
            else if (Obj == saveMeshType)
            {
                hr = pSaveDlg->SetFileName(L"arm_model_.obj");
            }
            else if (Ply == saveMeshType)
            {
                hr = pSaveDlg->SetFileName(L"arm_model_.ply");
            }
			else if (Svg == saveMeshType)
			{
				hr = pSaveDlg->SetFileName(L"AnnotationSample.svg");
			}



            if (SUCCEEDED(hr))
            {
                // Set the file type extension
                if (Stl == saveMeshType)
                {
                    hr = pSaveDlg->SetDefaultExtension(L"stl");
                }
                else if (Obj == saveMeshType)
                {
                    hr = pSaveDlg->SetDefaultExtension(L"obj");
                }
                else if (Ply == saveMeshType)
                {
                    hr = pSaveDlg->SetDefaultExtension(L"ply");
                }
				else if (Svg == saveMeshType)
				{
					hr = pSaveDlg->SetDefaultExtension(L"svg");
				}

                if (SUCCEEDED(hr))
                {
                    // Set the file type filters
                    if (Stl == saveMeshType)
                    {
                        COMDLG_FILTERSPEC allPossibleFileTypes[] = {
                            { L"Stl mesh files", L"*.stl" },
                            { L"All files", L"*.*" }
                        };

                        hr = pSaveDlg->SetFileTypes(
                            ARRAYSIZE(allPossibleFileTypes),
                            allPossibleFileTypes);
                    }
                    else if (Obj == saveMeshType)
                    {
                        COMDLG_FILTERSPEC allPossibleFileTypes[] = {
                            { L"Obj mesh files", L"*.obj" },
                            { L"All files", L"*.*" }
                        };

                        hr = pSaveDlg->SetFileTypes(
                            ARRAYSIZE(allPossibleFileTypes),
                            allPossibleFileTypes );
                    }
                    else if (Ply == saveMeshType)
                    {
                        COMDLG_FILTERSPEC allPossibleFileTypes[] = {
                            { L"Ply mesh files", L"*.ply" },
                            { L"All files", L"*.*" }
                        };

                        hr = pSaveDlg->SetFileTypes(
                            ARRAYSIZE(allPossibleFileTypes),
                            allPossibleFileTypes );
                    }
					else if (Svg == saveMeshType)
					{
						COMDLG_FILTERSPEC allPossibleFileTypes[] = {
							{ L"SVG files", L"*.svg" },
						{ L"All files", L"*.*" }
						};

						hr = pSaveDlg->SetFileTypes(
							ARRAYSIZE(allPossibleFileTypes),
							allPossibleFileTypes);
					}

                    if (SUCCEEDED(hr))
                    {
                        // Show the file selection box
                        hr = pSaveDlg->Show(m_hWnd);

                        // Save the mesh to the chosen file.
                        if (SUCCEEDED(hr))
                        {
                            CComPtr<IShellItem> pItem;
                            hr = pSaveDlg->GetResult(&pItem);

                            if (SUCCEEDED(hr))
                            {
                                LPOLESTR pwsz = nullptr;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

                                if (SUCCEEDED(hr))
                                {
                                    SetStatusMessage(L"Saving mesh file, please wait...");
                                    SetCursor(LoadCursor(nullptr, MAKEINTRESOURCE(IDC_WAIT)));

                                    if (Stl == saveMeshType)
                                    {
										hr = WriteBinarySTLMeshFile(pMesh, pwsz);
                                    }
                                    else if (Obj == saveMeshType)
                                    {
                                        hr = WriteAsciiObjMeshFile(pMesh, pwsz);
                                    }
                                    else if (Ply == saveMeshType)
                                    {
                                        hr = WriteAsciiPlyMeshFile(pMesh, pwsz, true, m_bColorCaptured);
                                    }
									else if (Svg == saveMeshType)
									{
										hr = WriteBinarySVGCanvasFile(m_params.m_svgHelper, pwsz);
									}

                                    CoTaskMemFree(pwsz);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return hr;
}

///////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Initialize the UI
/// </summary>
void Scansify::InitializeUIControls()
{
    // Create NuiSensorChooser UI control
    RECT rc;
    GetClientRect(m_hWnd, &rc);

    POINT ptCenterTop;
    ptCenterTop.x = (rc.right - rc.left)/2;
    ptCenterTop.y = 0;

	
    // Set slider ranges
    SendDlgItemMessage(
        m_hWnd,
        IDC_SLIDER_DEPTH_MIN,
        TBM_SETRANGE,
        TRUE,
        MAKELPARAM(MIN_DEPTH_DISTANCE_MM, MAX_DEPTH_DISTANCE_MM));

    SendDlgItemMessage(m_hWnd,
        IDC_SLIDER_DEPTH_MAX,
        TBM_SETRANGE,
        TRUE,
        MAKELPARAM(MIN_DEPTH_DISTANCE_MM, MAX_DEPTH_DISTANCE_MM));

    SendDlgItemMessage(
        m_hWnd,
        IDC_INTEGRATION_WEIGHT_SLIDER,
        TBM_SETRANGE,
        TRUE,
        MAKELPARAM(MIN_INTEGRATION_WEIGHT, MAX_INTEGRATION_WEIGHT));

    // Set slider positions
    SendDlgItemMessage(
        m_hWnd,
        IDC_SLIDER_DEPTH_MAX,
        TBM_SETPOS,
        TRUE,
        (UINT)m_params.m_fMaxDepthThreshold * 1000);

    SendDlgItemMessage(
        m_hWnd,
        IDC_SLIDER_DEPTH_MIN,
        TBM_SETPOS,
        TRUE,
        (UINT)m_params.m_fMinDepthThreshold * 1000);

    SendDlgItemMessage(
        m_hWnd,
        IDC_INTEGRATION_WEIGHT_SLIDER,
        TBM_SETPOS,
        TRUE,
        (UINT)m_params.m_cMaxIntegrationWeight);

    // Set intermediate slider tics at meter intervals
    for (int i=1; i<(MAX_DEPTH_DISTANCE_MM/1000); i++)
    {
        SendDlgItemMessage(m_hWnd, IDC_SLIDER_DEPTH_MAX, TBM_SETTIC, 0, i*1000);
        SendDlgItemMessage(m_hWnd, IDC_SLIDER_DEPTH_MIN, TBM_SETTIC, 0, i*1000);
    }

    // Update slider text
    WCHAR str[MAX_PATH];
    swprintf_s(str, ARRAYSIZE(str), L"%4.2fm", m_params.m_fMinDepthThreshold);
    SetDlgItemText(m_hWnd, IDC_MIN_DIST_TEXT, str);
    swprintf_s(str, ARRAYSIZE(str), L"%4.2fm", m_params.m_fMaxDepthThreshold);
    SetDlgItemText(m_hWnd, IDC_MAX_DIST_TEXT, str);

    swprintf_s(str, ARRAYSIZE(str), L"%u", m_params.m_cMaxIntegrationWeight);
    SetDlgItemText(m_hWnd, IDC_INTEGRATION_WEIGHT_TEXT, str);

	
	HWND comboVoxel = GetDlgItem(m_hWnd, IDC_COMBO_VOXELS);
	ComboBox_AddString(comboVoxel, L"768");
	ComboBox_AddString(comboVoxel, L"640");
	ComboBox_AddString(comboVoxel, L"512");
	ComboBox_AddString(comboVoxel, L"384");
	ComboBox_AddString(comboVoxel, L"256");
	ComboBox_AddString(comboVoxel, L"128");

	// Set the control box for Volume Parameters
	switch ((int)m_params.m_reconstructionParams.voxelsPerMeter)
	{
	case 768:
		ComboBox_SetText(comboVoxel, L"768");
		break;
	case 640:
		ComboBox_SetText(comboVoxel, L"640");
		break;
	case 512:
		ComboBox_SetText(comboVoxel, L"512");
		break;
	case 384:
		ComboBox_SetText(comboVoxel, L"384");
		break;
	case 256:
		ComboBox_SetText(comboVoxel, L"256");
		break;
	case 128:
		ComboBox_SetText(comboVoxel, L"128");
		break;
	default:
		m_params.m_reconstructionParams.voxelsPerMeter = 256.0f;	// set to medium default
		ComboBox_SetText(comboVoxel, L"256");
		break;
	}

	comboVoxel = GetDlgItem(m_hWnd, IDC_COMBO_ROOM_X);
	ComboBox_AddString(comboVoxel, L"640");
	ComboBox_AddString(comboVoxel, L"512");
	ComboBox_AddString(comboVoxel, L"384");
	ComboBox_AddString(comboVoxel, L"256");
	ComboBox_AddString(comboVoxel, L"128");

	switch ((int)m_params.m_reconstructionParams.voxelCountX)
	{
	case 640:
		ComboBox_SetText(comboVoxel, L"640");
		break;
	case 512:
		ComboBox_SetText(comboVoxel, L"512");
		break;
	case 384:
		ComboBox_SetText(comboVoxel, L"384");
		break;
	case 256:
		ComboBox_SetText(comboVoxel, L"256");
		break;
	case 128:
		ComboBox_SetText(comboVoxel, L"128");
		break;
	default:
		m_params.m_reconstructionParams.voxelCountX = 384;	// set to medium default
		ComboBox_SetText(comboVoxel, L"384");
		break;
	}

	comboVoxel = GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Y);
	ComboBox_AddString(comboVoxel, L"640");
	ComboBox_AddString(comboVoxel, L"512");
	ComboBox_AddString(comboVoxel, L"384");
	ComboBox_AddString(comboVoxel, L"256");
	ComboBox_AddString(comboVoxel, L"128");

	switch ((int)m_params.m_reconstructionParams.voxelCountY)
	{
	case 640:
		ComboBox_SetText(comboVoxel, L"640");
		break;
	case 512:
		ComboBox_SetText(comboVoxel, L"512");
		break;
	case 384:
		ComboBox_SetText(comboVoxel, L"384");
		break;
	case 256:
		ComboBox_SetText(comboVoxel, L"256");
		break;
	case 128:
		ComboBox_SetText(comboVoxel, L"128");
		break;
	default:
		m_params.m_reconstructionParams.voxelCountX = 384;	// set to medium default
		ComboBox_SetText(comboVoxel, L"384");
		break;
	}

	comboVoxel = GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Z);
	ComboBox_AddString(comboVoxel, L"640");
	ComboBox_AddString(comboVoxel, L"512");
	ComboBox_AddString(comboVoxel, L"384");
	ComboBox_AddString(comboVoxel, L"256");
	ComboBox_AddString(comboVoxel, L"128");
	

    switch((int)m_params.m_reconstructionParams.voxelCountZ)
    {
	case 640:
		ComboBox_SetText(comboVoxel, L"640");
		break;
	case 512:
		ComboBox_SetText(comboVoxel, L"512");
		break;
	case 384:
		ComboBox_SetText(comboVoxel, L"384");
		break;
	case 256:
		ComboBox_SetText(comboVoxel, L"256");
		break;
	case 128:
		ComboBox_SetText(comboVoxel, L"128");
		break;
    default:
        m_params.m_reconstructionParams.voxelCountX = 384;	// set to medium default
		ComboBox_SetText(comboVoxel, L"384");
        break;
    }

    if (Stl == m_saveMeshFormat)
    {
        CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_STL_RADIO, BST_CHECKED);
    }
    else if (Obj == m_saveMeshFormat)
    {
        CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_OBJ_RADIO, BST_CHECKED);
    }
    else if (Ply == m_saveMeshFormat)
    {
        CheckDlgButton(m_hWnd, IDC_MESH_FORMAT_PLY_RADIO, BST_CHECKED);
    }

    if (m_params.m_bCaptureColor)
    {
        CheckDlgButton(m_hWnd, IDC_CHECK_CAPTURE_COLOR, BST_CHECKED);
    }

    if (m_params.m_bAutoFindCameraPoseWhenLost)
    {
        CheckDlgButton(m_hWnd, IDC_CHECK_CAMERA_POSE_FINDER, BST_CHECKED);
    }
	

}

/// <summary>
/// Checks the Menu on given input
/// </summary>
/// <param name="wParam">message data</param>
/// <param name="check">if option should be checked or not</param>
void Scansify::CheckMenu(WPARAM id, bool check) {
	
	HMENU menu = GetMenu(m_hWnd);

	if (check)	CheckMenuItem(menu, LOWORD(id), MF_CHECKED);
	else 		CheckMenuItem(menu, LOWORD(id), MF_UNCHECKED);
}

void Scansify::SaveMesh(bool reconstruction) {
		// process saving mesh with reconstructed part

		SetStatusMessage(L"Creating and saving mesh of reconstruction, please wait...");
		m_bSavingMesh = true;

		// Pause integration while we're saving
		bool wasPaused = m_params.m_bPauseIntegration;
		m_params.m_bPauseIntegration = true;
		m_processor.SetParams(m_params);

		INuiFusionColorMesh *mesh = nullptr;
		HRESULT hr = m_processor.CalculateMesh(&mesh);

		if (SUCCEEDED(hr))
		{
			// Save mesh
			hr = SaveMeshFile(mesh, m_saveMeshFormat, reconstruction);

			if (SUCCEEDED(hr))
			{
				SetStatusMessage(L"Saved Kinect Fusion mesh.");
			}
			else if (HRESULT_FROM_WIN32(ERROR_CANCELLED) == hr)
			{
				SetStatusMessage(L"Mesh save canceled.");
			}
			else
			{
				SetStatusMessage(L"Error saving Kinect Fusion mesh!");
			}

			// Release the mesh
			SafeRelease(mesh);
		}
		else
		{
			SetStatusMessage(L"Failed to create mesh of reconstruction.");
		}

		// Restore pause state of integration
		m_params.m_bPauseIntegration = wasPaused;
		m_processor.SetParams(m_params);
		m_bSavingMesh = false;


		return;
}

void Scansify::ImportMesh() {
	// process saving mesh with reconstructed part

	SetStatusMessage(L"Importing existing mesh into design tool, please wait...");
	m_bSavingMesh = true;

	// Pause integration while we're saving
	bool wasPaused = m_params.m_bPauseIntegration;
	m_params.m_bPauseIntegration = true;
	m_processor.SetParams(m_params);

	INuiFusionColorMesh *mesh = nullptr;
	HRESULT hr = m_processor.CalculateMesh(&mesh);

	if (SUCCEEDED(hr))
	{
		// Save mesh
		hr = ImportMeshFile(m_saveMeshFormat);

		if (SUCCEEDED(hr))
		{
			SetStatusMessage(L"Imported Kinect Fusion mesh.");
		}
		else if (HRESULT_FROM_WIN32(ERROR_CANCELLED) == hr)
		{
			SetStatusMessage(L"Import canceled.");
		}
		else
		{
			SetStatusMessage(L"Error importing 3d mesh into design tool!");
		}

		// Release the mesh
		SafeRelease(mesh);
	}
	else SetStatusMessage(L"Error importing 3d mesh into design tool!");

	// Restore pause state of integration
	m_params.m_bPauseIntegration = wasPaused;
	m_processor.SetParams(m_params);
	m_bSavingMesh = false;


	return;
}


/// <summary>
/// Process the UI inputs
/// </summary>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
void Scansify::ProcessUI(WPARAM wParam, LPARAM)
{

	//printf("Received msg: %f\n", HIWORD(wParam));
    // If it was for the display surface normals toggle this variable
    if (ID_MENU_VIEW_CAPTURECOLOR == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        // Toggle capture color
        m_params.m_bCaptureColor = !m_params.m_bCaptureColor;

		CheckMenu(wParam, m_params.m_bCaptureColor);
    }
    // If it was for the display surface normals toggle this variable
	if (ID_MENU_VIEW_MIRRORDEPTH == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam)) {

		// Toggle depth mirroring
		m_params.m_bMirrorDepthFrame = !m_params.m_bMirrorDepthFrame;

		// Un-check pause
		CheckDlgButton(m_hWnd, IDC_CHECK_PAUSE_INTEGRATION, BST_UNCHECKED);
		m_processor.ResetReconstruction();

		CheckMenu(wParam, m_params.m_bMirrorDepthFrame);
	}
    if (ID_MENU_TOOLS_POSE_FINDER == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        m_params.m_bAutoFindCameraPoseWhenLost = !m_params.m_bAutoFindCameraPoseWhenLost;

		CheckMenu(wParam, m_params.m_bAutoFindCameraPoseWhenLost);
    }
	if (ID_MENU_DISPLAYFEATURES_FINGERS == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		// display finger tracking on depth frame
		SetStatusMessage(L"Toggle finger tracking display");
		m_params.m_bDisplayFingerTracking = !m_params.m_bDisplayFingerTracking;

		CheckMenu(wParam, m_params.m_bDisplayFingerTracking);
	}
	if (ID_MENU_DISPLAYFEATURES_HANDS == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		// display arms tracking on depth frame
		SetStatusMessage(L"Toggle arms tracking display");
		m_params.m_bDisplayArmTracking = !m_params.m_bDisplayArmTracking;

		CheckMenu(wParam, m_params.m_bDisplayArmTracking);
	}
	if (ID_MENU_DISPLAYFEATURES_ARMS == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		// display hands tracking on depth frame
		SetStatusMessage(L"Toggle hand tracking display");
		m_params.m_bDisplayHandTracking = !m_params.m_bDisplayHandTracking;

		CheckMenu(wParam, m_params.m_bDisplayHandTracking);
	}
	if (ID_MENU_DISPLAYFEATURES_RAY == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		// display finger pointing vector tracking on depth frame
		SetStatusMessage(L"Toggle finger pencil display");
		m_params.m_bDisplayRayTracking = !m_params.m_bDisplayRayTracking;

		CheckMenu(wParam, m_params.m_bDisplayRayTracking);
	}
    // If it was the reset button clicked, clear the volume
    if (IDC_BUTTON_RESET_RECONSTRUCTION == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
		if (m_params.m_bInitializeAnnotationMode) {
			// reset camera
			m_processor.ResetCamera();
			m_processor.RedrawRenderedImage();
		}
		else {
			// Un-check pause
			CheckDlgButton(m_hWnd, IDC_CHECK_PAUSE_INTEGRATION, BST_UNCHECKED);
			m_processor.ResetReconstruction();
		}
    }

	// If it was the reset button clicked, clear the annotations
	if (IDC_BUTTON_RESET_ANNOTATION == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		// only if in annotation mode
		if (m_params.m_bInitializeAnnotationMode && m_vAnnotatedObjects.size() > 0) {
			// reset camera
			m_vAnnotatedObjects.clear();
			delete m_params.m_svgHelper;
			m_params.m_svgHelper = new SvgHelper();
			m_params.m_sceneStructure->rebuildIndex();

			m_processor.SetParams(m_params);
			m_processor.RedrawRenderedImage();
		}
	}

	// If clicked on reconstruction window, compute screen coordinate
	if (IDC_RECONSTRUCTION_VIEW == LOWORD(wParam) && STN_CLICKED == HIWORD(wParam)) {
		POINT point;
		GetCursorPos(&point);
		RECT rect;
		HWND reconstructionWindow = GetDlgItem(m_hWnd, IDC_RECONSTRUCTION_VIEW);
		GetWindowRect(reconstructionWindow, &rect);
		double width = rect.right - rect.left;
		double height = rect.bottom - rect.top;

		// base formula for range interpolation: Result := ((Input - InputLow) / (InputHigh - InputLow)) * (OutputHigh - OutputLow) + OutputLow;
		// translate to screen coordinate [-1;1]

		double x, y; 
		if (ScreenToClient(reconstructionWindow, &point))
		{
			x = ((double)(point.x - 0.f) / (width - 0.f)) * (1.f + 1.f) - 1.f;
			y = ((double)(point.y - 0.f) / (height - 0.f)) * (1.f + 1.f) - 1.f;

			//p.x and p.y are now relative to hwnd's client area
			//printf("Clicked on: (%6.3f,%6.3f) \n", x,y);
		}


		// if structure has been built
		if (m_params.m_sceneStructure != nullptr && m_params.m_sceneStructure->built_flag) {
			rt::Ray r = (m_processor.GetRaytraceCamera())->getPrimaryRay((float)x, (float)y);
			r.m_bMousePicking = true;
			r.m_annotationID = m_vAnnotatedObjects.size();
			intersectedNodes.clear();
			rt::Intersection hit = m_params.m_sceneStructure->intersect(r, FLT_MAX, intersectedNodes);
			
			if (hit) {
				//printf("Hit detected at coordinate (%f, %f, %f) %f\n", hit.hitPoint().x, hit.hitPoint().y, hit.hitPoint().z, hit.m_nodeCounter);
				hit.solid->m_bAnnotated = true;
				m_vAnnotatedObjects.push_back(hit.solid);
				m_processor.SetParams(m_params);
				m_processor.RedrawRenderedImage();
			}
			//else return; // no processing needed
		}

		////////////// AUTOMATICALLY PROCESS HIT IN TEST PHASE /////////////////////
		///			   http://www.karldiab.com/3DPointPlotter/		to see tongue plot in 3D ///



		// TODO energy flow is wrongly calculated
		//auto marked = m_processor.GetAnnotatedObjects();
		auto marked = m_vAnnotatedObjects;
		printf("size: %d ", m_vAnnotatedObjects.size());
		auto annotatedCount = marked.size() - 1;
		if (marked.size() > 1) {
			rt::Point prev = marked[annotatedCount - 1]->sample(); // TODO sample might possible be responsible for the misaligned rawing on the model
			rt::Point curr = marked[annotatedCount]->sample();
			auto vec = curr - prev; // vector from previous to current node

			if (annotatedCount == 1) {
				//init Matrix
				m_params.m_svgHelper->addData(prev.x, prev.z);
				//printf("added svg data (%f, %f)\n", prev.x, prev.z);
			}

			// change direction flag if following vector would be crossing the z-axis because we are moving in x-axis 
			// it's important to evaluate how we add the y value to the x one
			auto dynamic = rt::dot(vec.normalize(), rt::Vector(0, -1, 0));
			if (dynamic < 0)	m_params.m_svgHelper->m_bDirectionFlagX = !m_params.m_svgHelper->m_bDirectionFlagX;

			//printf("dynamic: %f \n", dynamic);

			float len = vec.length();
			float diff = marked[0]->sample().y - curr.y;
			float storedY = vec.y;
			vec.y = 0;
			auto flag = m_params.m_svgHelper->getDirectionX(dynamic, vec.x);
			printf("direction is: %s\n", flag ? "positive" : "negative");
			if (flag) {
				vec.z += std::abs(storedY);
			}
			else {
				vec.z -= std::abs(storedY);
			}

			vec = vec.normalize() * len;

			rt::Point fixedPrevPoint(m_params.m_svgHelper->getData()[annotatedCount - 1].first, 0, m_params.m_svgHelper->getData()[annotatedCount - 1].second);
			curr = fixedPrevPoint + vec; // modified curr

			m_params.m_svgHelper->addData(curr.x, curr.z);
			//printf("added svg data (%f, %f)\n", curr.x, curr.z);
		}

	}

	// If starting annotation process
	if (IDC_BUTTON_MESH_DRAWING == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
	{
		// check if we are in annotation mode
		if (m_params.m_bInitializeAnnotationMode) {
			m_params.m_bInitializeAnnotationMode = false;

			UpdateMode(Scansify::Mode::Reconstruction);
			m_processor.ResetReconstruction();

			// clear annotations
			m_vAnnotatedObjects.clear();
		}
		else {
			// Initialize svghelper
			delete m_params.m_svgHelper;
			m_params.m_svgHelper = new SvgHelper();

			SetStatusMessage(L"Reconstructing the mesh. Please wait...");

			m_processor.ResetCamera();

			// Pause integration while we're saving
			bool wasPaused = m_params.m_bPauseIntegration;
			m_params.m_bPauseIntegration = true;
			m_processor.SetParams(m_params);

			// Release the mesh from previous reconstruction
			SafeRelease(m_params.m_pMesh);


			// Release the mesh in KinectFusionProcessor
			HRESULT hr = m_processor.CalculateMesh(&m_params.m_pMesh);

			SetStatusMessage(L"Building acceleration structure...");
			const clock_t begin_time = clock();

			if (SUCCEEDED(hr))
			{
				m_params.m_sceneStructure = new rt::BVH();

				INuiFusionColorMesh* mesh = m_params.m_pMesh;

				const Vector3 *vertices = nullptr;
				const Vector3 *normals = nullptr;

				unsigned int numVertices = mesh->VertexCount();
				unsigned int numTriangles = numVertices / 3;

				mesh->GetVertices(&vertices);
				mesh->GetNormals(&normals);



				// Iterate over generated mesh buffer and put data into vector
				float k = 0;
				for (unsigned int t = 0; t < numTriangles; ++t)
				{
					k++;
					rt::Point vertex[3];
					rt::Vector normal[3];

					// Sequentially write the 3 vertices and normals of the triangle, for each triangle
					for (unsigned int v = 0; v<3; v++)
					{
						vertex[v] = rt::Point(vertices[(t * 3) + v].x, vertices[(t * 3) + v].y, vertices[(t * 3) + v].z);
						normal[v] = rt::Vector(normals[(t * 3) + v].x, normals[(t * 3) + v].y, normals[(t * 3) + v].z);
					}
					rt::SmoothTriangle* smoothT = new rt::SmoothTriangle(vertex, normal);
					//smoothT->m_bAnnotated = true;
					m_params.m_sceneStructure->add(smoothT);
				}

				m_params.m_sceneStructure->buildIndex();
				UpdateMode(Scansify::Mode::Annotation);

				printf("\n\nAcceleration structure built in %.3f seconds with %.1f polygons.\n\n", float(clock() - begin_time) / CLOCKS_PER_SEC, k);
				m_processor.RedrawRenderedImage();
			}
			else SetStatusMessage(L"Failed to create mesh of reconstruction.");
		}

		
	}


	if (ID_MENU_IMPORT_RECONSTRUCTION_STL == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam)) {
		m_saveMeshFormat = Stl;
		ImportMesh();
	}
	if (ID_MENU_EXPORT_RECONSTRUCTION_STL == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam)) {
		m_saveMeshFormat = Stl;
		SaveMesh(true);
	}
	if (ID_MENU_EXPORT_RECONSTRUCTION_OBJ == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam)) {
		m_saveMeshFormat = Obj;
		SaveMesh(true);
	}
	if (ID_MENU_EXPORT_RECONSTRUCTION_PLY == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam)) {
		m_saveMeshFormat = Ply;
		SaveMesh(true);
	}

	if (ID_MENU_EXPORT_ANNOTATION_SVG == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam)) {
		m_saveMeshFormat = Svg;
		SaveMesh(false);
	}

	if (IDC_COMBO_VOXELS == LOWORD(wParam) && CBN_SELCHANGE == HIWORD(wParam)) {
		LPCTSTR k = new WCHAR; TCHAR buffer[60];

		auto sel = SendMessage(GetDlgItem(m_hWnd, IDC_COMBO_VOXELS), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		wsprintf(buffer, TEXT("&#37;d"), sel);
		ComboBox_GetLBText(GetDlgItem(m_hWnd, IDC_COMBO_VOXELS), sel, k);

		if (!wcscmp(k, L"768"))			m_params.m_reconstructionParams.voxelsPerMeter = 768.0f;
		else if (!wcscmp(k, L"640"))		m_params.m_reconstructionParams.voxelsPerMeter = 640.0f;
		else if (!wcscmp(k, L"512"))	m_params.m_reconstructionParams.voxelsPerMeter = 512.0f;
		else if (!wcscmp(k, L"384"))	m_params.m_reconstructionParams.voxelsPerMeter = 384.0f;
		else if (!wcscmp(k, L"256"))	m_params.m_reconstructionParams.voxelsPerMeter = 256.0f;
		else if (!wcscmp(k, L"128"))	m_params.m_reconstructionParams.voxelsPerMeter = 128.0f;
		else SetStatusMessage(L"Failed to set voxels per meter");

		//delete k;
	}


    if (IDC_CHECK_PAUSE_INTEGRATION == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
    {
        // Toggle the pause state of the reconstruction
        m_params.m_bPauseIntegration = !m_params.m_bPauseIntegration;
    }

	if (IDC_COMBO_ROOM_X == LOWORD(wParam) && CBN_SELCHANGE == HIWORD(wParam)) {
		LPCTSTR k = new WCHAR(); TCHAR buffer[60];

		auto sel = SendMessage(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_X), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		wsprintf(buffer, TEXT("&#37;d"), sel);
		ComboBox_GetLBText(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_X), sel, k);
		
		if (!wcscmp(k, L"640"))			m_params.m_reconstructionParams.voxelCountX = 640;
		else if (!wcscmp(k, L"512"))	m_params.m_reconstructionParams.voxelCountX = 512;
		else if (!wcscmp(k, L"384"))	m_params.m_reconstructionParams.voxelCountX = 384;
		else if (!wcscmp(k, L"256"))	m_params.m_reconstructionParams.voxelCountX = 256;
		else if (!wcscmp(k, L"128"))	m_params.m_reconstructionParams.voxelCountX = 128;
		else SetStatusMessage(L"Failed to set x-axis resolution");

		//delete k;
	}

	if (IDC_COMBO_ROOM_Y == LOWORD(wParam) && CBN_SELCHANGE == HIWORD(wParam)) {
		LPCTSTR k = new WCHAR(); TCHAR buffer[60];

		auto sel = SendMessage(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Y), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		wsprintf(buffer, TEXT("&#37;d"), sel);
		ComboBox_GetLBText(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Y), sel, k);

		if (!wcscmp(k, L"640"))			m_params.m_reconstructionParams.voxelCountY = 640;
		else if (!wcscmp(k, L"512"))	m_params.m_reconstructionParams.voxelCountY = 512;
		else if (!wcscmp(k, L"384"))	m_params.m_reconstructionParams.voxelCountY = 384;
		else if (!wcscmp(k, L"256"))	m_params.m_reconstructionParams.voxelCountY = 256;
		else if (!wcscmp(k, L"128"))	m_params.m_reconstructionParams.voxelCountY = 128;
		else SetStatusMessage(L"Failed to set y-axis resolution");

		//delete k;

	}

	if (IDC_COMBO_ROOM_Z == LOWORD(wParam) && CBN_SELCHANGE == HIWORD(wParam)) {
		LPCTSTR k = new WCHAR(); TCHAR buffer[60];

		auto sel = SendMessage(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Z), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		wsprintf(buffer, TEXT("&#37;d"), sel);
		ComboBox_GetLBText(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Z), sel, k);

		if (!wcscmp(k, L"640"))			m_params.m_reconstructionParams.voxelCountZ = 640;
		else if (!wcscmp(k, L"512"))	m_params.m_reconstructionParams.voxelCountZ = 512;
		else if (!wcscmp(k, L"384"))	m_params.m_reconstructionParams.voxelCountZ = 384;
		else if (!wcscmp(k, L"256"))	m_params.m_reconstructionParams.voxelCountZ = 256;
		else if (!wcscmp(k, L"128"))	m_params.m_reconstructionParams.voxelCountZ = 128;
		else SetStatusMessage(L"Failed to set z-axis resolution");

		//delete k;
	}

    m_processor.SetParams(m_params);
}

/// <summary>
/// Update mode
/// </summary>
void Scansify::UpdateMode(Scansify::Mode mode) {

	switch (mode) {
	case Reconstruction:
		// Show all unecessary window components //
		ShowWindow(GetDlgItem(m_hWnd, IDC_RECON_VOLUME_SETTINGS_BOX), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_VOXELS_PER_METER_BOX), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_INTEGRATION_WEIGHT_BOX), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_VOLUME_RESOLUTION_BOX), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_DEPTH_THRESHOLD_GROUP), SW_SHOW);

		// controls
		ShowWindow(GetDlgItem(m_hWnd, IDC_COMBO_VOXELS), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_INTEGRATION_WEIGHT_SLIDER), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_X), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Y), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Z), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_SLIDER_DEPTH_MIN), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_SLIDER_DEPTH_MAX), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_CHECK_PAUSE_INTEGRATION), SW_SHOW);

		//labels
		ShowWindow(GetDlgItem(m_hWnd, IDC_INTEGRATION_WEIGHT_TEXT), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_STATUS_X_AXIS), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_STATUS_Y_AXIS), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_STATUS_Z_AXIS), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_MIN_TEXT), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_MAX_TEXT), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_MIN_DIST_TEXT), SW_SHOW);
		ShowWindow(GetDlgItem(m_hWnd, IDC_MAX_DIST_TEXT), SW_SHOW);

		ShowWindow(GetDlgItem(m_hWnd, IDC_BUTTON_RESET_ANNOTATION), SW_HIDE);

		// Change label names
		SetWindowText(GetDlgItem(m_hWnd, IDC_BUTTON_MESH_DRAWING), L"Start Annotation");
		SetWindowText(GetDlgItem(m_hWnd, IDC_BUTTON_RESET_RECONSTRUCTION), L"Reset Reconstruction");

		// Un-check pause and reset reconstruction
		CheckDlgButton(m_hWnd, IDC_CHECK_PAUSE_INTEGRATION, BST_UNCHECKED);
		break;
	case Annotation:
		m_params.m_bInitializeAnnotationMode = true;
		m_processor.SetParams(m_params);


		// Hide all unecessary window components //
		ShowWindow(GetDlgItem(m_hWnd, IDC_RECON_VOLUME_SETTINGS_BOX), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_VOXELS_PER_METER_BOX), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_INTEGRATION_WEIGHT_BOX), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_VOLUME_RESOLUTION_BOX), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_DEPTH_THRESHOLD_GROUP), SW_HIDE);

		// controls
		ShowWindow(GetDlgItem(m_hWnd, IDC_COMBO_VOXELS), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_INTEGRATION_WEIGHT_SLIDER), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_X), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Y), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_COMBO_ROOM_Z), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_SLIDER_DEPTH_MIN), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_SLIDER_DEPTH_MAX), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_CHECK_PAUSE_INTEGRATION), SW_HIDE);

		//labels
		ShowWindow(GetDlgItem(m_hWnd, IDC_INTEGRATION_WEIGHT_TEXT), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_STATUS_X_AXIS), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_STATUS_Y_AXIS), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_STATUS_Z_AXIS), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_MIN_TEXT), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_MAX_TEXT), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_MIN_DIST_TEXT), SW_HIDE);
		ShowWindow(GetDlgItem(m_hWnd, IDC_MAX_DIST_TEXT), SW_HIDE);

		ShowWindow(GetDlgItem(m_hWnd, IDC_BUTTON_RESET_ANNOTATION), SW_SHOW);

		// Change label names
		SetWindowText(GetDlgItem(m_hWnd, IDC_BUTTON_MESH_DRAWING), L"Re-scan Arm");
		SetWindowText(GetDlgItem(m_hWnd, IDC_BUTTON_RESET_RECONSTRUCTION), L"Fix Camera");

		m_processor.ResetCamera();

		//m_processor.RedrawRenderedImage();
		break;
	}
}


/// <summary>
/// Update the internal variable values from the UI Horizontal sliders.
/// </summary>
void Scansify::UpdateHSliders()
{
    int mmMinPos = (int)SendDlgItemMessage(m_hWnd, IDC_SLIDER_DEPTH_MIN, TBM_GETPOS, 0,0);

    if (mmMinPos >= MIN_DEPTH_DISTANCE_MM && mmMinPos <= MAX_DEPTH_DISTANCE_MM)
    {
        m_params.m_fMinDepthThreshold = (float)mmMinPos * 0.001f;
    }

    int mmMaxPos = (int)SendDlgItemMessage(m_hWnd, IDC_SLIDER_DEPTH_MAX, TBM_GETPOS, 0,0);

    if (mmMaxPos >= MIN_DEPTH_DISTANCE_MM && mmMaxPos <= MAX_DEPTH_DISTANCE_MM)
    {
        m_params.m_fMaxDepthThreshold = (float)mmMaxPos * 0.001f;
    }

    int maxWeight = (int)SendDlgItemMessage(m_hWnd, IDC_INTEGRATION_WEIGHT_SLIDER, TBM_GETPOS, 0,0);
    m_params.m_cMaxIntegrationWeight = maxWeight % (MAX_INTEGRATION_WEIGHT+1);


    // update text
    WCHAR str[MAX_PATH];
    swprintf_s(str, ARRAYSIZE(str), L"%4.2fm", m_params.m_fMinDepthThreshold);
    SetDlgItemText(m_hWnd, IDC_MIN_DIST_TEXT, str);
    swprintf_s(str, ARRAYSIZE(str), L"%4.2fm", m_params.m_fMaxDepthThreshold);
    SetDlgItemText(m_hWnd, IDC_MAX_DIST_TEXT, str);

    swprintf_s(str, ARRAYSIZE(str), L"%u", m_params.m_cMaxIntegrationWeight);
    SetDlgItemText(m_hWnd, IDC_INTEGRATION_WEIGHT_TEXT, str);

    m_processor.SetParams(m_params);
}

/// <summary>
/// Set the status bar message
/// </summary>
/// <param name="szMessage">message to display</param>
void Scansify::SetStatusMessage(const WCHAR * szMessage)
{
    size_t length = 0;
    if (FAILED(StringCchLength(
        szMessage,
        KinectFusionProcessorFrame::StatusMessageMaxLen,
        &length)))
    {
        length = 0;
    }

    if (length > 0)
    {
        SendDlgItemMessageW(m_hWnd, IDC_STATUS, WM_SETTEXT, 0, (LPARAM)szMessage);
        m_tickLastStatus = GetTickCount64();
    }
    else
    {
        // Clear the status message after a timeout (as long as frames are flowing)
        if (GetTickCount64() - m_tickLastStatus > cStatusTimeoutInMilliseconds &&
            m_fFramesPerSecond > 0)
        {
            SendDlgItemMessageW(m_hWnd, IDC_STATUS, WM_SETTEXT, 0, 0);
            m_tickLastStatus = GetTickCount64();
        }
    }
}

/// <summary>
/// Set the frames-per-second message
/// </summary>
/// <param name="fFramesPerSecond">current frame rate</param>
void Scansify::SetFramesPerSecond(float fFramesPerSecond)
{
    if (fFramesPerSecond != m_fFramesPerSecond)
    {
        m_fFramesPerSecond = fFramesPerSecond;
        WCHAR str[MAX_PATH] = {0};
        if (fFramesPerSecond > 0)
        {
            swprintf_s(str, ARRAYSIZE(str), L"%5.2f FPS", fFramesPerSecond);
        }

        SendDlgItemMessageW(m_hWnd, IDC_FRAMES_PER_SECOND, WM_SETTEXT, 0, (LPARAM)str);
    }
}
