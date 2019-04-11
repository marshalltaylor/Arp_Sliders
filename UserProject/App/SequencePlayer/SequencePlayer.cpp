#include "Arduino.h"
//#include "outputNoteMixer.h"
#include "globals.h"
#include "SequencePlayer.h"

//extern OutputNoteMixer outputNoteMixer;

SequencePlayer::SequencePlayer(void) : outputQueue(200), outputNoteOn(127)
{
	playHeadTicks = 0;
	mainRegister = NULL;
}

void SequencePlayer::attachMainRegister( SequenceRegister * targetRegister )
{
	// Remove old association
	if( mainRegister )
	{
		mainRegister->unsubscribe(mainRegisterChanged, this);
	}
	// Subscribe to new register
	targetRegister->subscribe(mainRegisterChanged, this);
	// Indicate change locally once
	mainRegisterChanged();
}

soid SequencePlayer::mainRegisterChanged( void )
{
	while( outputNoteOn.listLength() )
	{
		listIdemNumber_t * object = outputNoteOn.readObject(0);
		// Convert to note off
		object->controlMask = NoteOff;
		// Give to output queue
		outputQueue.pushObject(object);
		// Drop from list
		outputNoteOn.dropObject(0);
	}
}

uint16_t SequencePlayer::available( void )
{
	return outputQueue.listLength();
}

void SequencePlayer::read( listIdemNumber_t * outputObject )
{
	if(!available())
	{
		return;
	}
	copyObject( outputObject, 0 );
}

void SequencePlayer::updateTicks( uint32_t ticks )
{
//	if( loaded == NULL )
//	{
//		return;
//	}
//	//Get a reference to the loop's linked list
//	MicroLL * loopLL = loaded->getLoopPtr();
//	uint16_t ticksModLength = ticks % loaded->length;
//	
//	listObject_t * itemReference;
//	listObject_t modifiedObject;
//	
//	listIdemNumber_t itemLocation = loopLL->seekObjectbyTimeStamp(ticksModLength);
//	if(( itemLocation != -1 ) && root != -1 )
//	{
//		// play tick
//		itemReference = loopLL->readObject( itemLocation );
//		modifiedObject.timeStamp = itemReference->timeStamp;
//		modifiedObject.eventType = itemReference->eventType;
//		modifiedObject.channel = itemReference->channel;
//		modifiedObject.data = itemReference->data;
//		switch( itemReference->eventType )
//		{
//			case 0x09: //note on
//			{
//				if( rootHasChanged )
//				{
//					// It's ok, no notes on
//					rootHasChanged = false;
//				}
//				modifiedObject.value = ((int16_t)root + (int16_t)itemReference->value - 48);
//				if(( clockSocket.getSocketedClock()->getState() == Playing ) &&
//						(drone || outputNoteMixer.keyboardInputNoteOnList.listLength()) )
//				{
//					outputNoteMixer.playerInput(&modifiedObject);
//				}
//			}
//			break;
//			case 0x08: //note off
//			{
//				if( rootHasChanged )
//				{
//					// Use the old root
//					rootHasChanged = false;
//					modifiedObject.value = ((int16_t)oldRoot + (int16_t)itemReference->value - 48);
//				}
//				else
//				modifiedObject.value = ((int16_t)root + (int16_t)itemReference->value - 48);
//				outputNoteMixer.playerInput(&modifiedObject);
//			}
//			break;
//			default:
//			break;
//		}
//	}
}

void LoopPlayer::printDebug( void )
{
	
	char buffer[200] = {0};
	sprintf(buffer, "loopPlayer debug: oldRoot, root, drone, rootHasChanged\n");
	Serial6.print(buffer);
	sprintf(buffer, "                  %d, %d, %d, %d\n", oldRoot, root, drone, rootHasChanged);
	Serial6.print(buffer);

}
