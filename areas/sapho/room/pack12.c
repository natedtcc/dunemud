/// pack12.c - Sapho Packaging Area corner - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

// Var for checking if the containers have been checked...
int hasSearched;

void reset(int arg)
{
  // Initialize value of hasSearched (false)
  hasSearched = 0;
  
  ::reset(arg);
  if (arg)
    return;
  set_light(1);
  set_short("$BOLD$REDSapho $OFF$WHITEPackaging Area$OFF");
  set_long(
      "You've reached a corner in the Sapho Packaging area. Every now and then,\n" 
      +"a worker glances over at you suspiciously, curious as to why you're\n" 
      +"here, fully clothed, and not wearing a respirator. Piles of boxes \n"
      +"presumably containing red-tar sapho are stacked here, waiting to be\n"
      +"shipped off to some unknown location.\n");

  add_exit("west", ROOM_DIR + "pack11.c");
  add_exit("north", ROOM_DIR + "pack9.c");

  add_item(
      "workers", "Can't blame them for being curious about you.");

  add_item(
      "boxes", "There are tons of them. Maybe you should investigate?\n");
}

void init()
{
::init();

  add_action("search", "search");
}

// Search the boxes for some ph4t l00tz

int search(string str)
{
  if(str != "boxes")
    return notify_fail("Search what?\n");
	
  if (str == "boxes" && hasSearched)
    return notify_fail ("There's nothing in here at the moment..\n");
 
  else{

    // Currently 1/3 chance to get loot from a box
    if (generate_loot_outcome() <= LOOT_CHANCE){
      write("You search around in the boxes and find a large mass of red tar sapho!\n");
      place_objects(OBJ_DIR + "redtar_loot", 1, this_player());
    }

    // Failure..
    else{
      write("You search around in the boxes but find nothing! Try again later!\n");
    }
    // Disable searching til' next reload
    hasSearched = 1;
    return 1;
  }
}