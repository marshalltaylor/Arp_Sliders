#include "bsp.h"
#include "linkedListTest.h"


#define localPrintf printf

/* Functions -----------------------------------------------------------------*/
int main ()
{
    int i;
    exampleListItem_t temp;
    exampleListItem_t * pTemp;
    UNUSED(pTemp);
    
    LinkedListTest unit(20, 24);
    
    unit.printList();
    unit.printInfo();
    
/*
Start by writing about 75% of the space for experimentation
*/
    printf("Do some normal operations\n");
    sprintf(temp.name, "Hello");
    temp.number = 2;
    unit.push(&temp);
    
    for(i = 0; i < 6; i++)
    {
        temp.number = 2 * i;
        unit.push(&temp);
    }
    
    unit.printList();
    unit.printInfo();
/*
Test the bookmarks
*/
    printf("Test the bookmarks\n");
    unit.setBookmark(0, 6);
    unit.setBookmark(1, 3);
    unit.printList();
    printf("  insert an early item\n");
    unit.insert(&temp, 2);
    unit.printList();
    printf("  insert a mid item\n");
    unit.insert(&temp, 5);
    unit.printList();
    printf("  insert a late item\n");
    unit.insert(&temp, 9);
    unit.printList();
/*
Edge case tests
*/
    printf("Overfill it, write 13\n");
    temp.number = 20;
    for(i = 0; i < 13; i++)
    {
        unit.push(&temp);
        temp.number++;
    }
    unit.printList();
    unit.printInfo();

    printf("Expect 1 bad read, 2 bad writes\n");
    unit.insert(&temp, 25);
    unit.drop(25);
    pTemp = (exampleListItem_t*)unit.readObject(25);
    
    unit.printInfo();
    
    printf("Clear all data\n");
    unit.clear();
    unit.clearErrorStats();

    unit.printList();
    unit.printInfo();
    unit.printBookmarks();
}
