// fhall4.c - Sapho Facility End of hall / pre-boss area - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg){

  ::reset(arg);

  place_objects(NPC_DIR + "door_guard.c", 2);

  if (arg)
    return;

  set_light(1);
  set_short("$WHITEEnd of the hallway");
  set_long(
      "\nYou've reached the end of the hallway in the $RED$BOLDSapho$OFF refinery. There's a\n"
      +"big, metallic door to the north, and judging by the scuffed floor, this is a rather\n"
      +"high traffic area. There is a sign posted near the elevator, and a red lever on the\n"
      +"western wall. A couple of guards stand here, toking away on their sapho pipes.");

  add_exit("south", ROOM_DIR + "fhall3.c");

  add_item("sign",
      "The sign reads : \"VIP Lounge\"");
 
  add_item("lever",
      "A shiny red lever, ripe for the pullin'.");
  
  add_item("floor",
      "Lots of footprints here, and rectangular part of the floor is a\n"
      +"little darker than the rest");
  
  add_item("door",
      "A big metallic door. It looks automatic.\nThere appears to be a slot on the right side of it.");

  add_item("slot",
      "It looks like it could facilitate some kind of small keycard..\n"
      +"Wonder where you might find that?");
}

void init(){
::init();

  add_action("do_read", "read");
  add_action("do_pull", "pull");
  add_action("do_swipe", "swipe");
}

// No one's gettin' in this door without a keycard


int do_read(string str)
{
  if (str != "sign"){
    return notify_fail("Read what? The sign?\n");
  }
    
  write("The sign reads : \"VIP Lounge\".\n");
    return 1;

}

// Lever pull == get trapped, scrub

int do_pull(string str)
{
  if (str != "lever"){
    return notify_fail("Pull what? The lever?\n");
  }

  write(
    "\nAs you pull the button, a trapdoor opens beneath you!"
    +"\n\n**THUD**\n\nYou fall about 20 ft and crash into the ground!\n");

    this_player()->move_player(
    "disappears from sight!\n", ROOM_DIR + "deathpit.c");
    return 1;
  
}

// Gain access to VIP with a keycard

int do_swipe(string str)
{
  
  if (str != "keycard") {return notify_fail("Swipe what? The keycard??\n");}

  else if (!deep_present("bebop_vip_keycard", this_player())) {
    return notify_fail ("You don't have a keycard.\n");}

  // If it's in a pack..
  else if (!present("bebop_vip_keycard", this_player())) {
    return notify_fail ("How you gonna swipe the keycard when it's in your pack?\n");}

  write(
    "\nYou swipe the keycard and shuffle into the next room quickly.\n");

  this_player()->move_player(
    "enters the VIP Lounge.\n", ROOM_DIR + "vip4.c");
  return 1;
}