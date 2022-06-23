#ifndef BSPSPI_H
#define BSPSPI_H

/* Spi Driver */
// The driver can handle a certain number of types of operation.  This is for special cases of CS driving or length or whatnot
//man not need this
//struct enum
//{
//	SPI_MODE_DEFAULT,
//	SPI_MODE_DIO,
//	SPI_MODE_TEENSYVIEW,
//	SPI_MODE_SEL_CLOCK,
//	SPI_MODE_NUM
//} spiMode_t;

// This object is created by a module and given to the operators
typedef struct
{
	uint32_t bspPin;
	uint8_t len;
	bool pendingXfer;
	uint8_t* txData;
	uint8_t* rxData;
} spiDevice_t;

typedef struct
{
	spiDevice_t* userDevice;
} spiControl_t;

// Create a buffer for the ID
//void bspSPIConfig(spiDeviceId_t id, uint32_t pin, uint32_t len);
// Operate.  Return false if busy
bool bspSPIWrite(spiDevice_t * dev);
void bspSPIWriteWait(spiDevice_t * dev);
bool bspSPIRead(spiDevice_t * dev);
// These two calls 
void bspSPIPendXfer(spiDevice_t * dev);
void bspSPIExecPending(spiDevice_t * dev);
#endif
