/**
 *	Keil project for STMPE811 touch driver controller
 *	
 *	Works on STM32F429 Discovery board by default
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

//#include "tm_stm32f4_ili9341.h"
//#include "tm_stm32f4_stmpe811.h"

/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_lcd.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_usart.h"
#include "stdio.h"
#include "tm_stm32f4_dma2d_graphic.h"
#include "tm_stm32f4_emwin.h"
#include "GUI.h"
#include "tm_stm32f4_adc.h"

#include "PROGBAR.h"
#include "BUTTON.h"
#include "GRAPH.h"
#include "DIALOG.h"
#include "math.h"

/* Graph handle */
GRAPH_Handle hGraph;
GRAPH_DATA_Handle hData;
GRAPH_DATA_Handle hData2;
GRAPH_DATA_Handle hData3;
GRAPH_SCALE_Handle hScale;

#define ILI9341_PIXEL				76800
/* Starting buffer address in RAM */
/* Offset for Layer 1 = SDRAM START ADDRESS + FRAME_BUFFER */
#define ILI9341_FRAME_BUFFER		SDRAM_START_ADR
/* Offset for Layer 2 */
#define ILI9341_FRAME_OFFSET		(uint32_t)ILI9341_PIXEL * 2

#define MAX 6

float intArray[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

void insert(float data) ;
float removeData(void) ;

int main(void) {
	uint32_t LastTime;
	float ACDResult = 0;
	/* Create TouchData struct */

	
	/* Initialize system */
	SystemInit();
	
	/* Initialize LCD */
	
	/* Initialize touchscreen */
		/* Initialize delay */
	TM_DELAY_Init();
	/* Initialize ADC */
	TM_ADC_Init(ADC1, ADC_Channel_0);
		/* Init EMWIN */
	TM_EMWIN_Init();
			/* Rotate LCD */
	TM_EMWIN_Rotate(TM_EMWIN_Rotate_270);
		/* Init DMA2D graphic acceleration */
	TM_DMA2DGRAPHIC_Init();
		/* 
	   Enable memory for EMWIN 
	   This features allows you to update screen very fast without flickering, because EMWIN draws on LCD layer 1 (LTDC) 
	   but layer 2 (LTDC) is shown on LCD. After all drawings from EMWIN are done, layer 1 is copied to layer 2 
	   and everything is shown to user at same time, without flickering.
	   You have to use @ref TM_EMWIN_Exec() to execute all EMWIN pending tasks in future or memory does not have effect here
	*/
	TM_EMWIN_MemoryEnable();
	
	/* Create graph through all LCD screen */
	hGraph = GRAPH_CreateEx(0, 0, 320, 240, 0, WM_CF_SHOW, 0, GUI_ID_GRAPH0);
	
	/* Set grids and border */
	GRAPH_SetGridVis(hGraph, 1);
  GRAPH_SetBorder(hGraph, 30, 5, 5, 5);
	GRAPH_SetColor(hGraph, 0x00202020, GRAPH_CI_GRID);
	GRAPH_SetVSizeX(hGraph, 100);
	GRAPH_SetVSizeY(hGraph, 100);
	
	/* Create a curve for graph */
	hData = GRAPH_DATA_YT_Create(GUI_DARKRED, 315, 0, 0);  
	
	/* Attach curve to graph */
    GRAPH_AttachData(hGraph, hData);
	
	/* Create scale for graph */
	hScale = GRAPH_SCALE_Create(3, GUI_TA_LEFT, GRAPH_SCALE_CF_VERTICAL, 10);
	GRAPH_SCALE_SetTextColor(hScale, GUI_BLUE);
	GRAPH_SCALE_SetNumDecs(hScale,1);
	GRAPH_SCALE_SetTickDist(hScale, 20);
	GRAPH_DATA_YT_SetAlign(hData,GRAPH_ALIGN_LEFT);
	/* Attach it to graph */
	GRAPH_AttachScale(hGraph, hScale);
	
	/* Change layers for LTDC, show layer 2 on LCD */
	GUI_SetBkColor(GUI_RED);					
	TM_EMWIN_Exec();
	insert(120);
	insert(120);
	insert(120);
	insert(120);
	while(1){
	TM_EMWIN_Exec();
	if ((TM_DELAY_Time() - LastTime) > 10) {
			/* Reset time */
			LastTime = TM_DELAY_Time();
			
			/* Add new fake values to graph */
			GRAPH_DATA_YT_AddValue(hData, removeData());
		}
	else{
			ACDResult = TM_ADC_Read(ADC1, ADC_Channel_0);
			insert(ACDResult);
	}
	
	}
}
void insert(float data) {

      if(rear == MAX-1) {
         rear = -1;            
      }       

      intArray[++rear] = data;
      itemCount++;
}

float removeData() {
   int data = intArray[front++];
	
   if(front == MAX) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}
