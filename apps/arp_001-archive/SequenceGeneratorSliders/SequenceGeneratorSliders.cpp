#include "Arduino.h"
//#include "outputNoteMixer.h"
#include "globals.h"
#include "SequenceGeneratorSliders.h"

//extern OutputNoteMixer outputNoteMixer;

SequenceGeneratorSliders::SequenceGeneratorSliders(void) : outputQueue(200), outputNoteOn(127)
{
	playHeadTicks = 0;
	mainRegister = NULL;
}

void SequenceGeneratorSliders::attachMainRegister( SequenceRegister * targetRegister )
{
	// Remove old association
	if( mainRegister )
	{
		mainRegister->removeUser(this);
	}
	// Subscribe to new register
	mainRegister = targetRegister->addUser(this);
}

uint16_t SequenceGeneratorSliders::available( void )
{
	return outputQueue.listLength();
}

void SequenceGeneratorSliders::read( listIdemNumber_t * outputObject )
{
	if(!available())
	{
		return;
	}
	mmqObject_t * nextObject = readObject( 0 );
	outputObject->controlMask = nextObject->controlMask;
	outputObject->channel = nextObject->channel;
	outputObject->value = nextObject->value;
	outputObject->data = nextObject->data;
}

// not sure if input is ticks from loop start or delta ticks
void SequenceGeneratorSliders::updateTicks( uint32_t ticks )
{
	// Leave immediately if no register loaded
	if( mainRegister == NULL )
	{
		return;
	}
	
	// Do things if the pointed to register has changed.
	if( mainRegister->serviceChanged() )
	{
		//send all the note offs
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
	
	// This should be smarter.  If a sequence changes, but the note
	// in progress is the same it shouldn't be restart.
	

	//Get a reference to the loop's linked list
	MicroLL * loopLL = loaded->getLoopPtr();
	uint16_t ticksModLength = ticks % loaded->length;
	
	
	/***** This object will only play without transpose *****/
	// also note length fixed to 1/2 step for now
	
	// calculate ticks per half-setp
	uint8_t ticksPerHalfStep = register->ticksPerStep / 2;
	
	// calculate current step
	// Index to step, round to earlier
	uint8_t currentStep = ticks / ticksPerStep;
	
	// Figure out what to do
	if( ticks == currentStep )
	{
		//note on
		mmqObject_t newNoteOn.params = things
		
		outputNoteOn.pushObject(newNoteOn);
		outputQueue.pushObject(newNoteOn);
	}
	if( ticks == currentStep + ticksPerHalfStep )
	{
		//note off
		mmqObject_t newNoteOff.params = things
		
		if( queue has note on )
		{
			outputNoteOn.dropObject...
		}
		// Always send note-off, who knows what harm it will do?
		outputQueue.pushObject(newNoteOff);
	}
}

void SequenceGeneratorSliders::printDebug( void )
{
//	
//	char buffer[200] = {0};
//	sprintf(buffer, "loopPlayer debug: oldRoot, root, drone, rootHasChanged\n");
//	Serial6.print(buffer);
//	sprintf(buffer, "                  %d, %d, %d, %d\n", oldRoot, root, drone, rootHasChanged);
//	Serial6.print(buffer);
//
}

//**********************************************************************//
//
//
//
//
//
//

//Loop::Loop( void )
//{
//	
//}
//
//MicroLL * Loop::getLoopPtr( void )
//{
//	return &loopData;
//}
	
//void Loop::clear( void )
//{
//	loopData.clear();
//}

//LoopGenTest::LoopGenTest( void )
//{
//	
//}
//
//void LoopGenTest::generate( Loop * loop, float lengthInBeats, uint8_t subdivision )
//{
//	MicroLL * noteList = loop->getLoopPtr();
//	noteList->clear();
//	listObject_t tempLO;
//	tempLO.channel = 1;
//	
//	uint16_t writeTick = 0;
//	uint16_t totalPatternTicks = lengthInBeats * 24;
//	loop->length = totalPatternTicks;
//	uint16_t noteLengthTicks = (24 / (subdivision + 1));
//	uint16_t noteOnTicks = noteLengthTicks / 2;
//	uint16_t noteOffTicks = noteLengthTicks - noteOnTicks;
//	while(writeTick < totalPatternTicks)
//	{
//		//Write a simple pattern
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x09;
//		tempLO.value = 48;
//		tempLO.data = 100;
//		noteList->pushObject(tempLO);
//		writeTick += noteOnTicks;
//
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x08;
//		tempLO.data = 0;
//		noteList->pushObject(tempLO);
//		writeTick += noteOffTicks;
//
//
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x09;
//		tempLO.value = 60;
//		tempLO.data = 100;
//		noteList->pushObject(tempLO);
//		writeTick += noteOnTicks;
//
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x08;
//		tempLO.data = 0;
//		noteList->pushObject(tempLO);
//		writeTick += noteOffTicks;
//
//		
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x09;
//		tempLO.value = 72;
//		tempLO.data = 100;
//		noteList->pushObject(tempLO);
//		writeTick += noteOnTicks;
//
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x08;
//		tempLO.data = 0;
//		noteList->pushObject(tempLO);
//		writeTick += noteOffTicks;
//	}
//	
//	noteList->printfMicroLL();
//	
//}


	// Operational
//void PatternContainer::resetStep( void )
//{
//}
//
//int16_t PatternContainer::incStep( void )
//{
//	return 0;
//}
//
//int16_t PatternContainer::getNote( void )
//{
//	return 0;//loopData.noteValue[step];
//}
//
//Sequence * PatternContainer::curPattern( void )
//{
//	return &data;
//}
//
//void PatternContainer::clearCtrlNotes( void )
//{
//	for( int i = 0; i < 25; i++ )
//	{
//		data.ctrlNotes[i] = false;
//	}
//}
//
//void PatternContainer::saveCtrlNote( uint8_t input )
//{
//	if(input > 24)
//	{
//		return;
//	}
//	data.ctrlNotes[input] = true;
//}
//
//void PatternContainer::setRoot( int16_t input )
//{
//	constrain(&input, 0, 127);
//	data.rootNote = input;
//}
//
//// Generational
//
//void PatternContainer::constrain( int16_t * input, int16_t lower, int16_t upper )
//{
//	if( *input > upper )
//	{
//		*input = upper;
//	}
//	if( *input < lower )
//	{
//		*input = lower;
//	}
//}
//
//// Range a little over 3 octaves
//void PatternContainer::setDepth( int16_t input )
//{
//	constrain(&input, 0, 40);
//	data.depth = input;
//}
//
//void PatternContainer::setOverclock( int16_t input )
//{
//}
//
//// Input in ticks (24 ticks/beat)
//void PatternContainer::setPatternLength( int16_t input )
//{
//	constrain(&input, 24, 768);
//	data.patternLength = input;
//}
//
//// Input ticks
//void PatternContainer::setNoteLength( int16_t input )
//{
//	constrain(&input, 0, 23);
//	data.noteLength = input;
//}
//
//void PatternContainer::setSubDivision( int16_t input )
//{
//	constrain(&input, 0, 2);
//	data.subDivision = input;
//	data.ticksPerStep = 24 / ( data.subDivision + 1 );
//}
//
//void PatternContainer::setDirection( int16_t input )
//{
//	constrain(&input, -1, 1);
//	data.direction = input;
//}
//
//void PatternContainer::setQuantize( bool input )
//{
//	data.quantize = input;
//}
//
//void PatternContainer::setSyncDownbeat( bool input )
//{
//	data.syncDownbeat = input;
//}
//
//void PatternContainer::generatePattern( void )
//{
//	int16_t dir = data.direction;
//	if( dir == 0 )
//	{
//		dir = 1; //Start upwards
//	}
//	int16_t nextNote = data.rootNote;
//	for( int i = 0; i < ( data.patternLength / data.ticksPerStep ); i++ )
//	{
//		data.step[i].value = nextNote;
//		// Calculate next note
//		switch( dir )
//		{
//			case -1:
//			nextNote = nextNote + 12;
//			if( nextNote > data.rootNote + data.depth )
//			{
//				nextNote = data.rootNote;
//				if( data.direction == 0 )
//				{
//					dir = 1;
//				}
//			}
//			break;
//			case 1:
//			nextNote = nextNote - 12;
//			if( nextNote < data.rootNote )
//			{
//				nextNote = (( data.depth / 12 ) * 12) + data.rootNote;
//				if( data.direction == 0 )
//				{
//					dir = -1;
//				}
//			}
//			break;
//			default:
//			break;
//		}
//		
//	}	
//}
//
//void PatternContainer::printControls( void )
//{
//	char buffer[200] = {0};
//	sprintf(buffer, "---Controls---\n");
//	Serial6.print(buffer);
//	sprintf(buffer, "  rNote: %2d dir: %3d depth: %3d, len: %3d, tkPerStep: %3d\n", data.rootNote, data.direction, data.depth, data.patternLength, data.ticksPerStep);
//	Serial6.print(buffer);
//	sprintf(buffer, "\n");
//	Serial6.print(buffer);
//}
//
//void PatternContainer::printPattern( void )
//{
//	char buffer[200] = {0};
//	int16_t lengthInSteps = data.patternLength / data.ticksPerStep;
//	sprintf(buffer, "---Pattern---\n");
//	Serial6.print(buffer);
//	for( int i = 0; i < lengthInSteps; i++ )
//	{
//		sprintf(buffer, "  step[ %2d ] note: %3d\n", i, data.step[i].value);
//		Serial6.print(buffer);
//		
//	}
//	sprintf(buffer, "\n");
//	Serial6.print(buffer);
//	sprintf(buffer, "---ControlNotes---\n");
//	Serial6.print(buffer);
//	for( int i = 0; i < 25; i++ )
//	{
//		sprintf(buffer, "  key[ %2d ] state: %3d\n", i, data.ctrlNotes[i]);
//		Serial6.print(buffer);
//		
//	}
//	sprintf(buffer, "\n");
//	Serial6.print(buffer);
//}
