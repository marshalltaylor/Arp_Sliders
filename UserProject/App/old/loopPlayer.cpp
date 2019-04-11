#include "Arduino.h"
#include "outputNoteMixer.h"
#include "globals.h"

extern OutputNoteMixer outputNoteMixer;

LoopPlayer::LoopPlayer(void)
{
	playHeadTicks = 0;
	drone = 0;
	root = -1;
	oldRoot = -1;
}

//void LoopPlayer::setLoop( Loop * input )
//{
//	loaded = input;
//}

void LoopPlayer::updateTicks( uint32_t ticks )
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

void LoopPlayer::setRoot( int16_t newRoot )
{
	if( rootHasChanged )
	{
		//Still haven't used up all the note offs
	}
	else
	{
		oldRoot = root;
	}
	root = newRoot;
	rootHasChanged = true;
	if( oldRoot == -1 )
	{
		// force valid value
		oldRoot = root;
	}
}

void LoopPlayer::setDrone( bool value )
{
	drone = value;
	Serial6.print("Drone: ");
	Serial6.println(value);
}

void LoopPlayer::printDebug( void )
{
	
	char buffer[200] = {0};
	sprintf(buffer, "loopPlayer debug: oldRoot, root, drone, rootHasChanged\n");
	Serial6.print(buffer);
	sprintf(buffer, "                  %d, %d, %d, %d\n", oldRoot, root, drone, rootHasChanged);
	Serial6.print(buffer);

}
