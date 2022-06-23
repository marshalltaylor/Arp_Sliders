/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "dac.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"
#include "bspPrivate.h"

/* References ----------------------------------------------------------------*/
#define DEBUG_STRIPES 0


typedef enum
{
	CRT_STATE_SYNC_ODD = 0,
	CRT_STATE_SYNC_EVEN,
	CRT_STATE_BLANK_PRE_VIDEO,
	CRT_STATE_H_SYNC,
	CRT_STATE_VIDEO,
	CRT_STATE_BLANK_POST_VIDEO,
} crtStates_t;


/* ... video buffers ... */
//uint8_t video[640][480]; // = 307.2 kB
//uint8_t video[320][240]; // = 76.8 kB

#define DAC_BUFFER_LEN 64

//#define CRT_PARAM_SYNC_TIP_LEVEL 0
//#define CRT_PARAM_PORCH_LEVEL 57 //63, 120 and 191 worked a bit
//#define CRT_PARAM_BLACK_LEVEL CRT_PARAM_PORCH_LEVEL + 14
//#define CRT_PARAM_STRIPE_BLACK_LEVEL CRT_PARAM_PORCH_LEVEL + 14
//#define CRT_PARAM_WHITE_LEVEL CRT_PARAM_STRIPE_BLACK_LEVEL + 128

#define CRT_PARAM_SYNC_TIP_LEVEL 0
#define CRT_PARAM_PORCH_LEVEL 72 //63, 120 and 191 worked a bit
#define CRT_PARAM_BLACK_LEVEL CRT_PARAM_PORCH_LEVEL + 26
#define CRT_PARAM_STRIPE_BLACK_LEVEL CRT_PARAM_PORCH_LEVEL + 26
#define CRT_PARAM_WHITE_LEVEL CRT_PARAM_STRIPE_BLACK_LEVEL + 64 //128

#define CRT_PARAM_AMP_SHIFT 2

//The following are measuared in clocks since the start of the front porch.
//t + n type measurement
//#define N_CLK_SYNC 6
//#define N_CLK_BACK_PORCH 30
//#define N_CLK_BLACK 45

#define N_CLK_SYNC 10
#define N_CLK_BACK_PORCH 34
#define N_CLK_BLACK 49

#define N_CLK_BACK_PORCH_SHORT_SYNC 15
//#define N_CLK_BACK_PORCH_SHORT_VIDEO 26

#define N_CLK_BACK_PORCH_VERT_SYNC_RISE 113

//for lookup table
#define ODD_SYNC_V_SCAN_START_INDEX 8
#define ODD_SYNC_V_SCAN_LENGTH 18
#define EVEN_SYNC_V_SCAN_START_INDEX 8
#define EVEN_SYNC_V_SCAN_LENGTH 20

//Offsets act on the entire sync pattern.
//offset is time before t=0 (ft. p)
#define N_CLK_H_OFFSET 1

#define N_LINES_TOTAL 255 //262
#define N_LINES_SYNC_BLANK 10
#define N_LINES_PRE_BLANK 16//19
#define N_LINES_ACTIVE 216 //216
#define N_LINES_POST_BLANK (N_LINES_TOTAL - N_LINES_SYNC_BLANK - N_LINES_PRE_BLANK - N_LINES_ACTIVE)
#define N_LINES_EVEN_OFFSET -1

//These are in number of frames
#define N_FRAMES_SCAN_LINE 4
#define N_FRAMES_HALF_SCAN_LINE 2
#define N_FRAMES_VIDEO_DATA 3

//1 frame is DAC_BUFFER_LEN clocks long
//3 frames per half-line,
//6 frames per line,

uint8_t halfLineHSync[DAC_BUFFER_LEN*N_FRAMES_HALF_SCAN_LINE];
uint8_t halfLineHSyncToVSync[DAC_BUFFER_LEN*N_FRAMES_HALF_SCAN_LINE];
uint8_t halfLineVSync[DAC_BUFFER_LEN*N_FRAMES_HALF_SCAN_LINE];

uint8_t fullLineHVideoBlack[DAC_BUFFER_LEN*N_FRAMES_SCAN_LINE];
uint8_t fullLineHVideoStripes[DAC_BUFFER_LEN*N_FRAMES_SCAN_LINE];
uint8_t fullLineHVideoToSync[DAC_BUFFER_LEN*N_FRAMES_SCAN_LINE];
uint8_t fullLineHPorchToVideo[DAC_BUFFER_LEN*N_FRAMES_SCAN_LINE];

uint8_t fullLineNonVideoTestSignal[DAC_BUFFER_LEN*N_FRAMES_SCAN_LINE];

int16_t frameCounter = 0;
int16_t scanCounter = 0;
int16_t lineCounter = 0;
int16_t loopCounter = 0;

static bool bspDACPopState( uint8_t ** ppData );

static void initVSyncRecipe(void);


uint8_t * oddSyncHalfScanTable[30];
uint8_t * evenSyncHalfScanTable[30];

#define PX_FRAME_WIDTH 192
#define PX_FRAME_HEIGHT 144
#define PX_PER_FRAME (PX_FRAME_WIDTH * PX_FRAME_HEIGHT)

uint8_t frameA[PX_PER_FRAME];
uint8_t frameB[PX_PER_FRAME];
uint8_t * videoOutputActive = frameA;
bool frameAActive = true;
bool swapPending = false;

static void initVSyncRecipe(void)
{
	oddSyncHalfScanTable[0]  = halfLineHSync;
	oddSyncHalfScanTable[1]  = halfLineHSync;
	oddSyncHalfScanTable[2]  = halfLineHSync;
	oddSyncHalfScanTable[3]  = halfLineHSync;
	oddSyncHalfScanTable[4]  = halfLineHSync;
	oddSyncHalfScanTable[5]  = halfLineHSync;
	oddSyncHalfScanTable[6]  = halfLineHSync;
	oddSyncHalfScanTable[7]  = halfLineHSync;
	oddSyncHalfScanTable[8]  = halfLineHSync;
	oddSyncHalfScanTable[9]  = halfLineHSync;
	oddSyncHalfScanTable[10] = halfLineHSync;
	oddSyncHalfScanTable[11] = halfLineHSync;
	oddSyncHalfScanTable[12] = halfLineHSync;
	oddSyncHalfScanTable[13] = halfLineHSync;
	oddSyncHalfScanTable[14] = halfLineHSyncToVSync;
	oddSyncHalfScanTable[15] = halfLineVSync;
	oddSyncHalfScanTable[16] = halfLineVSync;
	oddSyncHalfScanTable[17] = halfLineVSync;
	oddSyncHalfScanTable[18] = halfLineVSync;
	oddSyncHalfScanTable[19] = halfLineVSync;
	oddSyncHalfScanTable[20] = halfLineHSync;
	oddSyncHalfScanTable[21] = halfLineHSync;
	oddSyncHalfScanTable[22] = halfLineHSync;
	oddSyncHalfScanTable[23] = halfLineHSync;
	oddSyncHalfScanTable[24] = halfLineHSync;
	oddSyncHalfScanTable[25] = halfLineHSync;
	oddSyncHalfScanTable[26] = fullLineHPorchToVideo;
	oddSyncHalfScanTable[27] = fullLineHPorchToVideo + (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE);
	oddSyncHalfScanTable[28] = fullLineHVideoBlack;
	oddSyncHalfScanTable[29] = fullLineHVideoBlack + (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE);

	evenSyncHalfScanTable[0]  = fullLineHVideoToSync;
	evenSyncHalfScanTable[1]  = fullLineHVideoToSync + (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE);
	evenSyncHalfScanTable[2]  = halfLineHSync;
	evenSyncHalfScanTable[3]  = halfLineHSync;
	evenSyncHalfScanTable[4]  = halfLineHSync;
	evenSyncHalfScanTable[5]  = halfLineHSync;
	evenSyncHalfScanTable[6]  = halfLineHSync;
	evenSyncHalfScanTable[7]  = halfLineHSync;
	evenSyncHalfScanTable[8]  = halfLineHSync;
	evenSyncHalfScanTable[9]  = halfLineHSync;
	evenSyncHalfScanTable[10] = halfLineHSync;
	evenSyncHalfScanTable[11] = halfLineHSync;
	evenSyncHalfScanTable[12] = halfLineHSync;
	evenSyncHalfScanTable[13] = halfLineHSync;
	evenSyncHalfScanTable[14] = halfLineHSync;
	evenSyncHalfScanTable[15] = halfLineHSyncToVSync;
	evenSyncHalfScanTable[16] = halfLineVSync;
	evenSyncHalfScanTable[17] = halfLineVSync;
	evenSyncHalfScanTable[18] = halfLineVSync;
	evenSyncHalfScanTable[19] = halfLineVSync;
	evenSyncHalfScanTable[20] = halfLineVSync;
	evenSyncHalfScanTable[21] = halfLineHSync;
	evenSyncHalfScanTable[22] = halfLineHSync;
	evenSyncHalfScanTable[23] = halfLineHSync;
	evenSyncHalfScanTable[24] = halfLineHSync;
	evenSyncHalfScanTable[25] = halfLineHSync;
	evenSyncHalfScanTable[26] = fullLineHPorchToVideo;
	evenSyncHalfScanTable[27] = fullLineHPorchToVideo + (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE);
	evenSyncHalfScanTable[28] = fullLineHVideoBlack;
	evenSyncHalfScanTable[29] = fullLineHVideoBlack + (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE);
};

/* ... state variables ... */
crtStates_t crtState;
uint16_t lineNumber = 0;
uint8_t * pVideoPage = NULL;
static bool evenScan = false;
static uint8_t ditherByNext = 1;
static uint8_t numberOfScans = 0;
static bool onlyOddFrame = false;

/* Functions -----------------------------------------------------------------*/
void bspDACSendBuffer(uint32_t * buffer, uint16_t len)
{
	if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, buffer, len, DAC_ALIGN_8B_R) != HAL_OK)
	{
		volatile char garbage = 2;
		UNUSED(garbage);
	}
	
	//HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0x00000044);
}

void bspDACStartDMA(void)
{
	uint8_t * pData = NULL;
	uint8_t * pDataM1 = NULL;
	bspDACPopState( &pData );
	bspDACPopState( &pDataM1 );
	//if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)pData, len, DAC_ALIGN_8B_R) != HAL_OK)
	//if(HAL_DAC_Test_Ping_Pong(&hdac, DAC_CHANNEL_1))
	//if(HAL_DAC_START_DOUBLE(&hdac, DAC_CHANNEL_1, (uint32_t*)test_pattern_160x120_data, (uint32_t*)test_pattern_160x120_data, DAC_BUFFER_LEN))
	if(HAL_DAC_START_DOUBLE(&hdac, DAC_CHANNEL_1, (uint32_t*)pData, (uint32_t*)pDataM1, DAC_BUFFER_LEN))
	{
		volatile char garbage = 2;
		UNUSED(garbage);
	}
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	uint8_t * pData = NULL;
	bspDACPopState( &pData );
	
	hdac->DMA_Handle1->Instance->M0AR = (uint32_t)pData;
}

void HAL_DAC_M1ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	uint8_t * pData = NULL;
	bspDACPopState( &pData );
	
	hdac->DMA_Handle1->Instance->M1AR = (uint32_t)pData;
}

void bspDACInit( void )
{
	crtState = CRT_STATE_SYNC_ODD;
	
	int i;
	int j;
	//NTSC sync patterns, half lines
	j = N_CLK_H_OFFSET;
	for(i = 0; i < (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE); i++)
	{
		if(i < N_CLK_SYNC)
		{
			halfLineVSync[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else if(i < N_CLK_BACK_PORCH_VERT_SYNC_RISE)
		{
			halfLineVSync[j] = CRT_PARAM_SYNC_TIP_LEVEL;
		}
		else
		{
			halfLineVSync[j] = CRT_PARAM_PORCH_LEVEL;
		}
		
		if((i < N_CLK_SYNC) || (i >= (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE) - N_CLK_H_OFFSET))
		{
			halfLineHSyncToVSync[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else
		{
			halfLineHSyncToVSync[j] = CRT_PARAM_SYNC_TIP_LEVEL;
		}
		
		//halfLineHSync
		if(i < N_CLK_SYNC)
		{
			halfLineHSync[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else if(i < N_CLK_BACK_PORCH_SHORT_SYNC)
		{
			halfLineHSync[j] = CRT_PARAM_SYNC_TIP_LEVEL;
		}
		else
		{
			halfLineHSync[j] = CRT_PARAM_PORCH_LEVEL;
		}
		
		j++;
		if(j >= (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE))
		{
			j = 0;
		}
	}
	
	//NTSC sync patterns, full lines
	j = N_CLK_H_OFFSET;
	uint8_t striper = 0;
	for(i = 0; i < (DAC_BUFFER_LEN * N_FRAMES_SCAN_LINE); i++)
	{
		//fullLineHVideoBlack
		if(i < N_CLK_SYNC)
		{
			fullLineHVideoBlack[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else if(i < N_CLK_BACK_PORCH)
		{
			fullLineHVideoBlack[j] = CRT_PARAM_SYNC_TIP_LEVEL;
		}
		else if(i < N_CLK_BLACK)
		{
			fullLineHVideoBlack[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else
		{
			fullLineHVideoBlack[j] = CRT_PARAM_BLACK_LEVEL;
		}
		
		//fullLineHVideoStripes
		if(i < N_CLK_SYNC)
		{
			fullLineHVideoStripes[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else if(i < N_CLK_BACK_PORCH)
		{
			fullLineHVideoStripes[j] = CRT_PARAM_SYNC_TIP_LEVEL;
		}
		else if(i < N_CLK_BLACK)
		{
			fullLineHVideoStripes[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else
		{
#if DEBUG_STRIPES == 1
			if((i > N_CLK_BLACK)&&(i < (N_FRAMES_SCAN_LINE*DAC_BUFFER_LEN) - 25))
			{
				if(striper < 20)
				{
					fullLineHVideoStripes[j] = CRT_PARAM_WHITE_LEVEL;
				}
				else
				{
					fullLineHVideoStripes[j] = CRT_PARAM_STRIPE_BLACK_LEVEL;
				}
			}
			else
			{
				fullLineHVideoStripes[j] = CRT_PARAM_STRIPE_BLACK_LEVEL;
			}
#else
				fullLineHVideoStripes[j] = CRT_PARAM_STRIPE_BLACK_LEVEL;
#endif
		}
		
		//fullLineHVideoToSync
		if(i < N_CLK_SYNC)
		{
			fullLineHVideoToSync[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else if(i < N_CLK_BACK_PORCH)
		{
			fullLineHVideoToSync[j] = CRT_PARAM_SYNC_TIP_LEVEL;
		}
		else if(i < N_CLK_BLACK)
		{
			fullLineHVideoToSync[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else if(i < ((DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE) + N_CLK_SYNC))
		{
			fullLineHVideoToSync[j] = CRT_PARAM_BLACK_LEVEL;
		}
		else if(i < (DAC_BUFFER_LEN * N_FRAMES_HALF_SCAN_LINE) + N_CLK_BACK_PORCH_SHORT_SYNC)
		{
			fullLineHVideoToSync[j] = CRT_PARAM_SYNC_TIP_LEVEL;
		}
		else if(j < N_CLK_H_OFFSET)
		{
			fullLineHVideoToSync[j] = CRT_PARAM_BLACK_LEVEL;
		}
		else
		{
			fullLineHVideoToSync[j] = CRT_PARAM_PORCH_LEVEL;
		}
		
		//fullLineHPorchToVideo
		if(i < N_CLK_SYNC)
		{
			fullLineHPorchToVideo[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else if(i < N_CLK_BACK_PORCH)
		{
			fullLineHPorchToVideo[j] = CRT_PARAM_SYNC_TIP_LEVEL;
		}
		else if(i < N_CLK_BLACK)
		{
			fullLineHPorchToVideo[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else if(j < N_CLK_H_OFFSET)
		{
			fullLineHPorchToVideo[j] = CRT_PARAM_PORCH_LEVEL;
		}
		else
		{
			fullLineHPorchToVideo[j] = CRT_PARAM_BLACK_LEVEL;
		}
		
		//fullLineNonVideoTestSignal
		if(i < 2)
		{
			fullLineNonVideoTestSignal[i] = (i % 2) * 255;
		}
		else
		{
			fullLineNonVideoTestSignal[i] = 0;
		}
		
		//Chores
		j++;
		if(j >= (DAC_BUFFER_LEN * N_FRAMES_SCAN_LINE))
		{
			j = 0;
		}
		striper++;
		if(striper >= 40)
		{
			striper = 0;
		}
	}
	
	//Fix test data
	for(i = 0; i < (160*120); i++)
	{
		//test_pattern_160x120_data[i] = ((test_pattern_160x120_data[i])>>2) + CRT_PARAM_BLACK_LEVEL;
	}
	for(i = 0; i < (192*144); i++)
	{
		//test_pattern_192x144_data[i] = ((test_pattern_192x144_data[i]) >> CRT_PARAM_AMP_SHIFT) + CRT_PARAM_BLACK_LEVEL;
		//test_image_192x144_data[i] = ((test_image_192x144_data[i]) >> CRT_PARAM_AMP_SHIFT) + CRT_PARAM_BLACK_LEVEL;
		//custom_test_pattern_192x144_data[i] = ((custom_test_pattern_192x144_data[i]) >> CRT_PARAM_AMP_SHIFT) + CRT_PARAM_BLACK_LEVEL;
	}
	
	//Load 2-part buffers
	initVSyncRecipe();

	//Debug buffers by putting a spike at byte 0
	//halfLineHSync[0] = 0xAF;
	//halfLineHSyncToVSync[0] = 0x8F;
	//halfLineVSync[0] = 0x8F;
	//fullLineHVideoBlack[0] = 0xCF;
	//fullLineHVideoToSync[0] = 0xFF;
	//fullLineHVideoStripes[0] = 10;
	//fullLineHVideoStripes[1] = 10;
	
	//videoOutputActive = bufferA;
	
	//Start with a black frame
	uint8_t * frame;
	bspDACGetBufferBlank(&frame, 0x00); //address of pointer
	bspDACSwapBuffers();
	
	bspDACStartDMA();
}

static bool bspDACPopState( uint8_t ** ppData )
{
	switch( crtState )
	{
		//BEGIN ODD SYNC BRANCH oddSyncHalfScanTable
		case CRT_STATE_SYNC_ODD:
		{
			uint8_t * buffer = oddSyncHalfScanTable[loopCounter];
			*ppData = &buffer[DAC_BUFFER_LEN * frameCounter];
			frameCounter++;
			if(frameCounter >= N_FRAMES_HALF_SCAN_LINE)
			{
				frameCounter = 0;
				loopCounter++;
				if(loopCounter >= ODD_SYNC_V_SCAN_START_INDEX + ODD_SYNC_V_SCAN_LENGTH)
				{
					loopCounter = 0;
					evenScan = false;
					numberOfScans = 0;
					ditherByNext = 1;
					crtState = CRT_STATE_BLANK_PRE_VIDEO;
				}
			}
			break;
		}
		//END ODD SYNC BRANCH
		
		//BEGIN EVEN SYNC BRANCH
		case CRT_STATE_SYNC_EVEN:
		{
			uint8_t * buffer = evenSyncHalfScanTable[loopCounter];
			*ppData = &buffer[DAC_BUFFER_LEN * frameCounter];
			frameCounter++;
			if(frameCounter >= N_FRAMES_HALF_SCAN_LINE)
			{
				frameCounter = 0;
				loopCounter++;
				if(loopCounter >= EVEN_SYNC_V_SCAN_START_INDEX + EVEN_SYNC_V_SCAN_LENGTH)
				{
					loopCounter = 0;
					evenScan = true;
					numberOfScans = 0;
					ditherByNext = 2;
					crtState = CRT_STATE_BLANK_PRE_VIDEO;
				}
			}
			break;
		}
		//END EVEN SYNC BRANCH
		
		// Blank video scans
		case CRT_STATE_BLANK_PRE_VIDEO:
		{
			if((loopCounter >= N_LINES_SYNC_BLANK) && (loopCounter < N_LINES_SYNC_BLANK + N_LINES_PRE_BLANK))
			{
				*ppData = &fullLineHVideoStripes[DAC_BUFFER_LEN * frameCounter];
			}
			else
			{
				*ppData = &fullLineHVideoBlack[DAC_BUFFER_LEN * frameCounter];
			}
			frameCounter++;
			if(frameCounter >= N_FRAMES_SCAN_LINE)
			{
				loopCounter++;
				frameCounter = 0;
				int8_t offset = 0;
				if(evenScan)
				{
					offset = N_LINES_EVEN_OFFSET;
				}
				if(loopCounter >= N_LINES_SYNC_BLANK + N_LINES_PRE_BLANK + offset)
				{
					loopCounter = 0;
					crtState = CRT_STATE_H_SYNC;
				}
			}
			break;
		}
		// Start of horizontal video scan
		case CRT_STATE_H_SYNC:
		{
			*ppData = fullLineHVideoBlack; // just use first part
			crtState = CRT_STATE_VIDEO;
			break;
		}
		case CRT_STATE_VIDEO:
		{
			uint16_t index = (lineCounter * 192) + (DAC_BUFFER_LEN * frameCounter);
			//*ppData = &custom_test_pattern_192x144_data[index];
			//*ppData = &test_pattern_192x144_data[index];
			//*ppData = &test_image_192x144_data[index];
			*ppData = &videoOutputActive[index];
			//if(evenScan)
			//{
			//	//*ppData = &test_image_192x144_data[index];
			//	*ppData = &fullLineHVideoBlack[(DAC_BUFFER_LEN * frameCounter)];
			//}
			frameCounter++;
			if(frameCounter >= N_FRAMES_VIDEO_DATA)
			{
				
				frameCounter = 0;

				numberOfScans++;
				if(numberOfScans >= ditherByNext)
				{
					numberOfScans = 0;
					if(ditherByNext == 2)
					{
						ditherByNext = 1;
					}
					else
					{
						ditherByNext = 2;
					}
					lineCounter++;
				}
				loopCounter++;
				
				//Choose the next state
				//if(lineCounter >= 144)
				if(loopCounter >= N_LINES_ACTIVE)
				{
					lineCounter = 0;
					loopCounter = 0;
					//Master sync of data side frame
					{
						if(swapPending)
						{
							if(frameAActive)
							{
								videoOutputActive = frameB;
								frameAActive = false;
							}
							else
							{
								videoOutputActive = frameA;
								frameAActive = true;
							}
							swapPending = false;
						}
					}
					crtState = CRT_STATE_BLANK_POST_VIDEO;
				}
				else
				{
					crtState = CRT_STATE_H_SYNC;
				}
			}
			break;
		}
		// Blank video scans
		case CRT_STATE_BLANK_POST_VIDEO:
		{
			//if(loopCounter < N_LINES_POST_BLANK)
			//{
				*ppData = &fullLineHVideoStripes[DAC_BUFFER_LEN * frameCounter];
			//}
			//else
			//{
			//	*ppData = &fullLineHVideoBlack[DAC_BUFFER_LEN * frameCounter];
			//}
			int8_t offset = 0;
			if(evenScan)
			{
				offset = N_LINES_EVEN_OFFSET;
			}
			frameCounter++;
			if(frameCounter >= N_FRAMES_SCAN_LINE)
			{
				frameCounter = 0;
				loopCounter++;
				if(loopCounter >= N_LINES_POST_BLANK - offset)
				{
					loopCounter = 0;
					if(evenScan || onlyOddFrame)
					{
						loopCounter = ODD_SYNC_V_SCAN_START_INDEX;
						crtState = CRT_STATE_SYNC_ODD;
						//Toggle debug pin
						//bspIOPinWrite(D31, 0);
						//bspIOPinWrite(D31, 1);

					}
					else
					{
						loopCounter = EVEN_SYNC_V_SCAN_START_INDEX;
						crtState = CRT_STATE_SYNC_EVEN;
					}
				}
			}
			break;
		}
		
		default:
		break;
	}
	//*ppData = fullLineNonVideoTestSignal;
	return true;
}

bool bspDACGetBufferStale(uint8_t ** output)
{
	if(swapPending)
	{
		return false;
	}
	//else
	if(frameAActive)
	{
		*output = frameB;
	}
	else
	{
		*output = frameA;
	}
	return true;
}

bool bspDACGetBufferCopy(uint8_t ** output)
{
	if(swapPending)
	{
		return false;
	}
	//else
	if(frameAActive)
	{
		memcpy(frameB, frameA, PX_PER_FRAME);
		*output = frameB;
	}
	else
	{
		memcpy(frameA, frameB, PX_PER_FRAME);
		*output = frameA;
	}
	return true;
}

bool bspDACGetBufferBlank(uint8_t ** output, uint8_t fill)
{
	fill = (fill >> 2) + CRT_PARAM_BLACK_LEVEL;
	if(swapPending)
	{
		return false;
	}
	//else
	if(frameAActive)
	{
		memset(frameB, fill, PX_PER_FRAME);
		*output = frameB;
	}
	else
	{
		memset(frameA, fill, PX_PER_FRAME);
		*output = frameA;
	}
	return true;
}

void bspDACSwapBuffers(void)
{
	swapPending = true;
}


void bspDACInterlace(bool input)
{
	onlyOddFrame = !input;
}
