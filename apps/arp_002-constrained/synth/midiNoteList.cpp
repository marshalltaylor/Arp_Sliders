#include <stdint.h>
#include "bsp.h"

#include "midiCommon.h"
#include "midiNoteList.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

///* Functions -----------------------------------------------------------------*/
MidiNoteList::MidiNoteList(listIndex_t maxLengthIn, uint16_t dataSizeIn) : LinkedList(maxLengthIn, dataSizeIn)
{
    ((MidiMessage*)&nullObject)->controlMask = 0;
    ((MidiMessage*)&nullObject)->channel = 0;
    ((MidiMessage*)&nullObject)->value = 0;
    ((MidiMessage*)&nullObject)->data = 0;
    ((MidiMessage*)&nullObject)->tick = 0;
}

// I think these types should take uint8_t * value...

listIndex_t MidiNoteList::seekObjectByNoteValue( MidiMessage * data )
{
  //Create temporary note to use for comparison
  listItemContainer_t * tempCont;
  tempCont = startObjectPtr;
  listIndex_t returnVar = -1;

  for (uint8_t i = 0; i < currentPosition; i++)
  {
    //****CUSTOM OBJECT TYPE CODE****//
    MidiMessage * pMsg = (MidiMessage *)tempCont->data;
    //For now, only use the value
    if ( pMsg->value == data->value )
    {
      returnVar = i;
    }
    //Walk down the list
    tempCont = tempCont->next;
  }
  return returnVar;
}

void MidiNoteList::printList(void)
{
    listItemContainer_t * tempCont;
    //our custom type
    MidiMessage * pData;
    tempCont = startObjectPtr;
    pData = (MidiMessage *)tempCont->data;
    localPrintf("\nNote list listing\n");
    localPrintf(  "-------------------------\n");
    //Iterate to the depth
    for ( uint8_t i = 0; i < currentPosition; i++ )
    {
        localPrintf("  [%d] 0x%02X 0x%02X 0x%02X 0x%02X at %10lu\n",
                    i,
                    pData->channel,
                    pData->controlMask,
                    pData->value,
                    pData->data,
                    pData->tick
                    );
        //move index
        tempCont = tempCont->next;
        pData = (MidiMessage *)tempCont->data;
  }
  localPrintf("\ncurrentPosition: %d\n", currentPosition);
  
}
