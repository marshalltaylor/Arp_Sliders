#include "stdint.h"
#include "Arduino.h"
#include "MidiUtils.h"

#define Serial Serial6

// #define PRINT_DEBUG

//**********************************************************************//
//  MidiMessageQueue Class
//
//  Creates link list
//
//  Construct with passed max depth or use default
//
MidiMessageQueue::MidiMessageQueue( mmqItemNumber_t inputNumber )
{
  startObjectPtr = &nullObject;  //start pointed to null note
  currentPosition = 0 ;  //Size 0
  maxLength = inputNumber;  //set max size
  nullObject.controlMask = InvalidType;  //define null note
  nullObject.nextObject = &nullObject;  //Points to self.

}

MidiMessageQueue::MidiMessageQueue( void )
{
  startObjectPtr = &nullObject;
  currentPosition = 0;
  maxLength = 1000;
  nullObject.controlMask = InvalidType;
  nullObject.nextObject = &nullObject;  //Points to self.

}

//Pass mmqObject_t to be pushed to the stack (top)
void MidiMessageQueue::pushObject( mmqObject_t * objectToPush )
{
  //This creates a new object, but then forgets the name
  //and doesn't delete the object.
  //
  //If the stack size is too big, new object won't be accepted
  if ( currentPosition < maxLength )
  {
    //Make a new object to the list
    mmqObject_t *newObject = new mmqObject_t;
#ifdef PRINT_DEBUG
    Serial.print("\nMADE A NEW LIST ITEM\n");
#endif

    //Point the start to the new note if this is the first one
	if( currentPosition == 0 )
	{
	  startObjectPtr = newObject;
	}
	else
	{
	  //Otherwise set the previous note to this new address
	  mmqObject_t * previousObjectPtr = readObject( currentPosition - 1 );
	  previousObjectPtr->nextObject = newObject;
	}

    //grow list size
    currentPosition++;
	
	//****CUSTOM OBJECT TYPE CODE****//
    //Set the new note's params
    newObject->controlMask = objectToPush->controlMask;
	newObject->channel = objectToPush->channel;
    newObject->value = objectToPush->value;
	newObject->data = objectToPush->data;
	newObject->tick = objectToPush->tick;
	
    newObject->nextObject = &nullObject;
  }

}

//Pass mmqObject_t to be pushed to the stack (top)
void MidiMessageQueue::pushObject( MidiMessage * objectToPush )
{
  //This creates a new object, but then forgets the name
  //and doesn't delete the object.
  //
  //If the stack size is too big, new object won't be accepted
  if ( currentPosition < maxLength )
  {
    //Make a new object to the list
    mmqObject_t *newObject = new mmqObject_t;
#ifdef PRINT_DEBUG
    Serial.print("\nMADE A NEW LIST ITEM\n");
#endif

    //Point the start to the new note if this is the first one
	if( currentPosition == 0 )
	{
	  startObjectPtr = newObject;
	}
	else
	{
	  //Otherwise set the previous note to this new address
	  mmqObject_t * previousObjectPtr = readObject( currentPosition - 1 );
	  previousObjectPtr->nextObject = newObject;
	}

    //grow list size
    currentPosition++;
	
	//****CUSTOM OBJECT TYPE CODE****//
    //Set the new note's params
    newObject->controlMask = objectToPush->controlMask;
	newObject->channel = objectToPush->channel;
    newObject->value = objectToPush->value;
	newObject->data = objectToPush->data;
	newObject->tick = 0;
	
    newObject->nextObject = &nullObject;
  }

}

//Pass position, returns void
void MidiMessageQueue::dropObject( mmqItemNumber_t positionToDrop )
{
  //pointer for object to drop
  mmqObject_t * objectToDrop;
  mmqObject_t * tempObjectPtr;

  if ( positionToDrop >= currentPosition )
  {
    //Too deep!  Do nothing.
  }
  else
  {
    //Ok, good to do it.
	//Go to the object before positionToDrop
    if ( positionToDrop == 0 )
    {
		//Save the start object
		objectToDrop = startObjectPtr;
  	
		//Move startObjectPtr
		startObjectPtr = startObjectPtr->nextObject;
		
		//if( startObjectPtr->nextObject == &nullObject )
		//{
		//	//If the start object is the null object, we are empty
		//	startObjectPtr = &nullObject;
		//}
    }
    else
	{
		//go to the position before the one to drop
		tempObjectPtr = startObjectPtr;
		for (uint8_t i = 0; i < (positionToDrop - 1); i++ )
		{
			tempObjectPtr = tempObjectPtr->nextObject;
		}
		//Point out the drop target
		objectToDrop = tempObjectPtr->nextObject;
		//move the older object to skip
		tempObjectPtr->nextObject = objectToDrop->nextObject;
	}
    //Drop it like it's not
    delete objectToDrop;
#ifdef PRINT_DEBUG
    Serial.println("\nDROPPED AN ITEM\n");
#endif
    currentPosition--;
  }

}

//Pass mmqObject_t and position
void MidiMessageQueue::insertObject( mmqObject_t * objectToInsert, mmqItemNumber_t positionToBe )
{
	//pointer for object to drop
	mmqObject_t * tempObjectPtr;
	
	if(( currentPosition < maxLength ) && ( positionToBe <= currentPosition ))
	{
		//If position is 0, do it a different way
		if( positionToBe == 0 )
		{
			//pushObject( objectToInsert );
			//Make a new object
			mmqObject_t *newObject = new mmqObject_t;
#ifdef PRINT_DEBUG
			Serial.print("\nINSERTING AN OBJECT AT 0, MADE A NEW LIST ITEM\n");
#endif
			
			
			//Point the new object at the previous target
			newObject->nextObject = startObjectPtr;
			
			//Point the one above to the new one
			startObjectPtr = newObject;
			
			//Apply the values
			newObject->controlMask = objectToInsert->controlMask;
			newObject->channel = objectToInsert->channel;
			newObject->value = objectToInsert->value;
			newObject->data = objectToInsert->data;
			newObject->tick = objectToInsert->tick;
			
			//grow list size
			currentPosition++;
		}
		else
		{
			//Make a new object
			mmqObject_t *newObject = new mmqObject_t;
#ifdef PRINT_DEBUG
			Serial.print("\nINSERTING AN OBJECT, MADE A NEW LIST ITEM\n");
#endif
			
			//Go to the position above, save pointer
			tempObjectPtr = startObjectPtr;
			for (uint8_t i = 0; i < ( (int32_t)positionToBe - 1 ); i++ )
			{
				tempObjectPtr = tempObjectPtr->nextObject;
			}
			
			//Point the new object at the previous target
			newObject->nextObject = tempObjectPtr->nextObject;
			
			//Point the one above to the new one
			tempObjectPtr->nextObject = newObject;
			
			//Apply the values
			newObject->controlMask = objectToInsert->controlMask;
			newObject->value = objectToInsert->value;
			newObject->data = objectToInsert->data;
			newObject->channel = objectToInsert->channel;
			newObject->tick = objectToInsert->tick;
			
			//grow list size
			currentPosition++;
		}
	}
}

//Clear out the list, point afresh
void MidiMessageQueue::clear( void )
{
	//Drop it like it's NOT
	while( currentPosition > 0 )
	{
		dropObject( 0 );
	}

}


//returns mmqObject_t
mmqObject_t * MidiMessageQueue::readObject( mmqItemNumber_t inputNumber ) //position 0 = top
{
  mmqObject_t * objectToReturn;

  if (inputNumber < maxLength) //Prevent out of range peeks
  {
    objectToReturn = startObjectPtr;
    //Iterate to the depth
    for ( uint8_t i = 0; i < inputNumber; i++ )
    {
      objectToReturn = objectToReturn->nextObject;
    }
  }
  else
  {
    objectToReturn = &nullObject;
  }
  return objectToReturn;
}

// Copy an object into the passed address
//mmqObject_t * MidiMessageQueue::copyObject( mmqItemNumber_t inputNumber ) //position 0 = top
//{
//  mmqObject_t * objectToReturn;
//
//  if (inputNumber < maxLength) //Prevent out of range peeks
//  {
//    objectToReturn = startObjectPtr;
//    //Iterate to the depth
//    for ( uint8_t i = 0; i < inputNumber; i++ )
//    {
//      objectToReturn = objectToReturn->nextObject;
//    }
//  }
//  else
//  {
//    objectToReturn = &nullObject;
//  }
//  return objectToReturn;
//}

//returns depth of stack.
mmqItemNumber_t MidiMessageQueue::listLength( void )
{
  return currentPosition;
}

//printfs the stack
void MidiMessageQueue::printfMicroLL( void )
{

  mmqObject_t * tempObject;
  long output;
  tempObject = startObjectPtr;
  Serial.println("\n Pos, tick, type, channel, value, data, address, nextObjectAddress");
  Serial.println(  "-------------------------");
  //Iterate to the depth
  for ( uint8_t i = 0; i < currentPosition; i++ )
  {
    Serial.print(" ");
    Serial.print(i);
    Serial.print(", ");
    Serial.print(tempObject->tick);
    Serial.print(", 0x");
    Serial.print(tempObject->controlMask, HEX);
    Serial.print(", ");
    Serial.print(tempObject->channel);
    Serial.print(", ");
    Serial.print(tempObject->value);
    Serial.print(", ");
    Serial.print(tempObject->data);
    Serial.print(", ");
    output = (long)tempObject;
    Serial.print(output, HEX);
    Serial.print(", ");
    output = (long)tempObject->nextObject;
    Serial.println(output, HEX);    
    //move note
    tempObject = tempObject->nextObject;
  }
  Serial.print("\ncurrentPosition:");
  Serial.println(currentPosition);

}

mmqItemNumber_t MidiMessageLinkedList::seekObjectByNoteValue( mmqObject_t * noteToSeek ) //pass mmqObject_t, returns position
{
  //Create temporary note to use for comparison
  mmqObject_t * tempObject;
  tempObject = startObjectPtr;
  mmqItemNumber_t returnVar = -1;

  for (uint8_t i = 0; i < currentPosition; i++)
  {
    //****CUSTOM OBJECT TYPE CODE****//
    //For now, only use the value
    if ( tempObject->value == noteToSeek->value )
    {
      returnVar = i;
    }
    //Walk down the list
    tempObject = tempObject->nextObject;
  }
  return returnVar;
}


// I think these types should take uint8_t * value...

mmqItemNumber_t MidiMessageLinkedList::seekObjectByNoteValue( MidiMessage * data )
{
  //Create temporary note to use for comparison
  mmqObject_t * tempObject;
  tempObject = startObjectPtr;
  mmqItemNumber_t returnVar = -1;

  for (uint8_t i = 0; i < currentPosition; i++)
  {
    //****CUSTOM OBJECT TYPE CODE****//
    //For now, only use the value
    if ( tempObject->value == data->value )
    {
      returnVar = i;
    }
    //Walk down the list
    tempObject = tempObject->nextObject;
  }
  return returnVar;
}

void MidiMessageLinkedList::printDebug( void )
{
	mmqObject_t * tempObject = startObjectPtr;
	//Print all the objects in the list
	char buffer[200] = {0};
	sprintf(buffer, "MidiMessageLinkedList listing\n");
	Serial6.print(buffer);
	
	for (uint8_t i = 0; i < currentPosition; i++)
	{
		//****CUSTOM OBJECT TYPE CODE****//
		sprintf(buffer, "  [%d] 0x%02X 0x%02X 0x%02X 0x%02X at %10lu\n", i, tempObject->channel, tempObject->controlMask, tempObject->value, tempObject->data, tempObject->tick);
		Serial6.print(buffer);

		//Walk down the list
		tempObject = tempObject->nextObject;
	}

}
