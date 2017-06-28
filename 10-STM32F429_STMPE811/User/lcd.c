#include "lcd.h"

uint32_t LastTime;
uint8_t i = 0;

/* Graph handle */
GRAPH_Handle hGraph;
GRAPH_DATA_Handle hData;
GRAPH_DATA_Handle hData2;
GRAPH_DATA_Handle hData3;
GRAPH_SCALE_Handle hScale;

	/* Add a button */
	BUTTON_Handle Button1;
	BUTTON_Handle Button2;
	BUTTON_Handle Button3;
	BUTTON_Handle Button4;
	
TM_STMPE811_TouchData touchData;


void CreateGraph(void){
		/* Create graph through all LCD screen */
	hGraph = GRAPH_CreateEx(0, 0, 320, 200, 0, WM_CF_SHOW, 0, GUI_ID_GRAPH0);
	
	/* Set grids and border */
	GRAPH_SetGridVis(hGraph, 1);
    GRAPH_SetBorder(hGraph, 25, 5, 5, 5);
	GRAPH_SetColor(hGraph, 0x00202020, GRAPH_CI_GRID);
	GRAPH_SetVSizeX(hGraph, 100);
	GRAPH_SetVSizeY(hGraph, 100);
	
	/* Create a curve for graph */
	hData = GRAPH_DATA_YT_Create(GUI_DARKRED, 305, 0, 0); 
	hData2 = GRAPH_DATA_YT_Create(GUI_DARKGREEN, 305, 0, 0); 
	hData3 = GRAPH_DATA_YT_Create(GUI_YELLOW, 305, 0, 0); 
	
	/* Attach curve to graph */
    GRAPH_AttachData(hGraph, hData);
    GRAPH_AttachData(hGraph, hData2);
    //GRAPH_AttachData(hGraph, hData3);
	
	/* Create scale for graph */
	hScale = GRAPH_SCALE_Create(3, GUI_TA_LEFT, GRAPH_SCALE_CF_VERTICAL | GUI_TA_VCENTER, 82);
	GRAPH_SCALE_SetFactor(hScale, 6.6/180);
	GRAPH_SCALE_SetNumDecs(hScale, 1);
	GRAPH_SCALE_SetOff(hScale, 90);
	GRAPH_SCALE_SetTextColor(hScale, GUI_BLUE);
	
	/* Attach it to graph */
	GRAPH_AttachScale(hGraph, hScale);

	CreateButtons();

	/* Change layers for LTDC, show layer 2 on LCD */
	GUI_SetBkColor(GUI_RED);					
if (TM_EMWIN_Exec()) {
		/* Turn on GREEN LED if non-zero value is returned */
		TM_DISCO_LedOn(LED_GREEN);
	}

}

void CreateButtons(void)
{	
	 Button1 = BUTTON_Create (20, 200, 70, 50, 1, BUTTON_CF_SHOW );
	 Button2 = BUTTON_Create (90, 200, 70, 50, 1, BUTTON_CF_SHOW );
	 Button3 = BUTTON_Create (160, 200, 70, 50, 1, BUTTON_CF_SHOW );
	 Button4 = BUTTON_Create (230, 200, 70, 50, 1, BUTTON_CF_SHOW );

	 //BUTTON_SetPressed(Button1, BUTTON_STATE_PRESSED);
	 BUTTON_SetText(Button1, "Przycisk 1");
	 BUTTON_SetText(Button2, "Przycisk 2");
	 BUTTON_SetText(Button3, "Przycisk 3");
	 BUTTON_SetText(Button4, "Przycisk 4");

	
}

void ShowResult(void){
		if ((TM_DELAY_Time() - LastTime) > 10) {
			/* Reset time */
			LastTime = TM_DELAY_Time();
			
		uint16_t Result = TM_ADC_Read(ADC1, ADC_Channel_0);
		Result = Result*3300/(4096*44);
		TouchDetected();
			if(!BUTTON_IsPressed(Button1)){
			/* Add new fake values to graph */
			GRAPH_DATA_YT_AddValue(hData, 100 +  Result);
			GRAPH_DATA_YT_AddValue(hData2, 100 + 50 * sin((float)2 * (float)5 * (float)3.14 * (float)i / (float)255));

			i++;
			}
		}
}

void TouchInit(void)
{
		if (TM_STMPE811_Init() != TM_STMPE811_State_Ok) {
		TM_ILI9341_Puts(20, 20, "STMPE811 Error", &TM_Font_11x18, ILI9341_COLOR_ORANGE, ILI9341_COLOR_BLACK);
		
		while (1);
	}
		
}

bool TouchDetected(void)
{
		char str[30];
touchData.orientation = TM_STMPE811_Orientation_Landscape_2;

	if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed) {
			/* Touch valid */
			if(touchData.y > 20 && touchData.y < 90 && touchData.x > 200 && touchData.x < 250)
			{
				BUTTON_SetPressed(Button1, BUTTON_STATE_PRESSED);
				BUTTON_SetText(Button1, "1 wcisniety");
				return true;
			}
			else
			{
				BUTTON_SetPressed(Button1, 0);
				BUTTON_SetText(Button1, "1 wcisniety");
				return false;
			}
	}
	return false;
}

