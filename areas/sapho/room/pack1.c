/// pack1.c - Sapho Packaging Area corner - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

// Var for checking if the containers have been checked...
int hasSearched;

void reset(int arg)
{
  
  ::reset(arg);
  // Reset ability to search 
  hasSearched = 0;
  place_objects(NPC_DIR + "guard.c", 2);

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

  add_exit("east", ROOM_DIR + "pack2.c");
  add_exit("south", ROOM_DIR + "pack4.c");

  add_item(
      "workers", "Can't blame them for being curious about you.");

  add_item(
      "boxes", "There are tons of them. Maybe you should investigate?");

}

void init()
{
::init();

  add_action("do_search", "search");
}

  // Prevent the player from searching initially

int guard_check()
{
  if (present("guard"))
  {
    return notify_fail(
      "The guard laughs and says \'You should probably step away now.\'\n");
    tell_room(this_object(),
      "\nThe Sapho-crazed guard doesn't like " + this_player()->query_name() 
      + " digging around in the boxes.\n",({this_player()}));
    
  }
  return 1;
}


// Search the boxes for some ph4t l00tz

int do_search(string str)
{
  // Make sure the guards are dead!
  if (guard_check()){
    if(str != "boxes")
      return notify_fail("Search what?\n");

    // Search cooldown (until next reset)
    if (str == "boxes" && hasSearched)
      return notify_fail ("There's nothing in here at the moment..\n");

    // All systems go...
    else{

      // Winner winner, chicken dinner!  
      // Currently 1/3 chance to get loot from a box
      if (generate_loot_outcome() <= LOOT_CHANCE){
        write(
          "You search around in the boxes and find a "
          +"large mass of red tar sapho!\n");

        place_objects(OBJ_DIR + "sapho_loot", 1, this_player());
      }

      // Failure..
      else{
        write(
          "You search around in the boxes but find "
          +"nothing! Try again later!\n");
        }
      // Disable searching til' next reload
      hasSearched = 1;
      return 1;
    }
  }
}