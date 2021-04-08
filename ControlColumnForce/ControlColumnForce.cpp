// ControlColumnForce.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include "framework.h"
#include "ControlColumnForce.h"

#include <stdio.h>
#include <string> 
#include <tchar.h>
#include <iostream>
#include "SerialPort.hpp"
#include "SimConnect.h"
#include <strsafe.h>

#define MAX_LOADSTRING 100
#define DATA_LENGTH 255
const char* portName = "COM3";
int     quit = 0;
HANDLE  hSimConnect = NULL;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

SerialPort* arduino = new SerialPort(portName);


// A basic structure for a single item of returned data
struct StructOneDatum {
    int		id;
    float	value;
};

// maxReturnedItems is 6 in this case, as the sample only requests
// vertical speed and pitot heat switch data
#define maxReturnedItems    6
// A structure that can be used to receive Tagged data
struct StructDatum {
    StructOneDatum  datum[maxReturnedItems];
};

enum EVENT_PDR {
    EVENT_SIM_START,
};

enum DATA_DEFINE_ID {
    DEFINITION_PDR,
};

enum DATA_REQUEST_ID {
    REQUEST_PDR,
};

enum DATA_NAMES {
    DATA_VERTICAL_SPEED,
    DATA_PITOT_HEAT,
    DATA_AILERON_POSITION,
    DATA_YOKE_X_POSITION,
    DATA_YOKE_Y_POSITION,
    DATA_AUTOPILOT_MASTER
};


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CONTROLCOLUMNFORCE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONTROLCOLUMNFORCE));

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;

}

void CALLBACK MyDispatchProcPDR(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
    HRESULT hr;
    
   /* std::string line1 = "";
    line1 = "--> Checking data 1";
    OutputDebugStringA(line1.c_str());*/


    switch (pData->dwID)
    {
       

        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;
            switch (evt->uEventID)
            {
            case EVENT_SIM_START:

                // Make the call for data every second, but only when it changes and
                // only that data that has changed
                // SIMCONNECT_PERIOD_SIM_FRAME
                hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_PDR, DEFINITION_PDR,
                    SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME,
                    SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);

                break;

            default:
                break;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

            switch (pObjData->dwRequestID)
            {
            case REQUEST_PDR:
            {
                int	count = 0;;
                StructDatum* pS = (StructDatum*)&pObjData->dwData;

                // There can be a minimum of 1 and a maximum of maxReturnedItems
                // in the StructDatum structure. The actual number returned will
                // be held in the dwDefineCount parameter.

                float simValX = 0;
                float simValY = 0;
                char msgs[2000];
                std::string line = "";
                std::string valueFormattedX = "";
                std::string valueFormattedY = "";


                while (count < (int)pObjData->dwDefineCount)
                {
                    switch (pS->datum[count].id)
                    {
                    case DATA_AILERON_POSITION:
                        printf("\nAileron pos = %f", pS->datum[count].value);
                        break;

                     // Pitch
                    case DATA_YOKE_Y_POSITION:
                        simValY = (pS->datum[count].value * 6000) +  500;
                        valueFormattedY = "<Y_POS, 0, " + std::to_string(simValY) + ">";
                        line = "\nYoke y pos = " + (std::to_string(pS->datum[count].value)) + " " + valueFormattedY;
                        OutputDebugStringA(line.c_str());

                        arduino->writeSerialPort(_strdup(valueFormattedY.c_str()), DATA_LENGTH);

                        break;
                    // Roll
                    case DATA_YOKE_X_POSITION:
                        simValX = (pS->datum[count].value * 5000) + 500;
                        valueFormattedY = "<X_POS, 0, " + std::to_string(simValX) + ">";
                        line = "\nYoke X pos = " + (std::to_string(pS->datum[count].value)) + " " + valueFormattedY;
                        OutputDebugStringA(line.c_str());
                        
                        arduino->writeSerialPort(_strdup(valueFormattedY.c_str()), DATA_LENGTH);
                        break;

                    case DATA_VERTICAL_SPEED:
                        //printf("\nVertical speed = %f", pS->datum[count].value );
                        break;

                    case DATA_PITOT_HEAT:
                        //printf("\nPitot heat = %f", pS->datum[count].value);
                        line = "\nPITO= " + (std::to_string(pS->datum[count].value));
                        OutputDebugStringA(line.c_str());

                        break;

                    case DATA_AUTOPILOT_MASTER: {
                       // printf("\nAutopilot heading lock = %f", pS->datum[count].value);
                        line = "\nAutopilot heading= " + (std::to_string(pS->datum[count].value));
                        OutputDebugStringA(line.c_str());
                        break;
                    }

                    default:
                        printf("\nUnknown datum ID: %d", pS->datum[count].id);
                        line = "unknown = " + (std::to_string(pS->datum[count].value));
                        OutputDebugStringA(line.c_str());
                        break;
                    }
                    ++count;
                }
                break;
            }

            default:
                break;
            }
            break;
    }


        case SIMCONNECT_RECV_ID_QUIT:
        {
            quit = 1;
            break;
        }

        default:
            printf("\nUnknown dwID: %d", pData->dwID);
            break;
        
    }
}

void testTaggedDataRequest()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Tagged Data", NULL, 0, 0, 0)))
    {

        std::string line = "-> Connected to Prepar3D!";
        OutputDebugStringA(line.c_str());


//        printf("\nConnected to Prepar3D!");

        // Set up the data definition, ensuring that all the elements are in Float32 units, to
        // match the StructDatum structure
        // The number of entries in the DEFINITION_PDR definition should be equal to
        // the maxReturnedItems define

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_PDR, "Vertical Speed", "Feet per second",
            SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_VERTICAL_SPEED);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_PDR, "Aileron Position", "Feet per second",
            SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_AILERON_POSITION);

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_PDR, "Yoke Y Position", "Feet per second",
            SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_YOKE_Y_POSITION);

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_PDR, "Yoke X Position", "Feet per second",
            SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_YOKE_X_POSITION);

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_PDR, "Pitot Heat", "Bool",
            SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_PITOT_HEAT);

        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_PDR, "Autopilot Master", "Bool",
            SIMCONNECT_DATATYPE_FLOAT32, 0, DATA_AUTOPILOT_MASTER);

        

        // Request a simulation start event
        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");

        while (0 == quit)
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcPDR, NULL);
            Sleep(1);
        }

        hr = SimConnect_Close(hSimConnect);
    }
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CONTROLCOLUMNFORCE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CONTROLCOLUMNFORCE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...


            if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Request Data", NULL, 0, 0, 0)))
            {
                printf("\nConnected to Prepar3D!");

                TextOut(hdc,
                  25, 25,
                   _T("Prepar3D: CONNECTED"), _tcslen(_T("Prepar3D: CONNECTED")));
            }
            else
            {

                TextOut(hdc,
                    25, 25,
                    _T("Prepar3D: Not Running"), _tcslen(_T("Prepar3D: Not Running")));
            }

            printf("Welcome to the serial test app!\n\n");

            TCHAR arduinoStatus[] = _T("Arduino Board: Connected");
            const char* sendString = "20";

            if (arduino->isConnected())
            {
                printf("We're connected");
                bool hasWritten = arduino->writeSerialPort(sendString, DATA_LENGTH);

                TextOut(hdc,
                    25, 50,
                    arduinoStatus, _tcslen(arduinoStatus));
            }
              
            testTaggedDataRequest();


            EndPaint(hWnd, &ps);

          

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}