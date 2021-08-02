// fhall4.c - Sapho Facility End of hall / pre-boss area - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg){

  ::reset(arg);
  if (!arg){

    place_objects(NPC_DIR + "door_guard.c", 2);
    set_light(1);
    set_short("End of facility hallway");
    set_long(
      "\nYou've reached the end of the hallway in the $RED$BOLDSapho$OFF refinery. There's a big,\n"
      +"metallic door to the north, and judging by the scuffed floor, this is a rather\n"
      +"high traffic area. There is a sign posted near the door, and a red lever on the\n"
      +"western wall. A couple of guards stand here, toking away on their sapho pipes.\n");

    add_exit("south", ROOM_DIR + "fhall3.c");

    add_item("sign",
      "The sign reads : VIP Lounge - Members Only!");
  
    add_item("lever",
      "A shiny red lever, ripe for the pullin'.");
    
    add_item("floor",
      "Lots of footprints here, and rectangular part of the floor is a\n"
      +"little darker than the rest");
    
    add_item("door",
      "A big metallic door. It looks automatic.\nThere appears to be a slot on the right side of it.");

    add_item("slot",
      "It looks like you could 'swipe' some kind of small keycard here..\n"
      +"Wonder where you might find that?");
  }
}

void init(){
::init();

  add_action("do_read", "read");
  add_action("do_pull", "pull");
  add_action("do_swipe", "swipe");
  add_action("wiz_door", "north");
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
      "disappears from sight", ROOM_DIR + "deathpit.c");
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
    "enters the VIP Lounge", ROOM_DIR + VIP + "vip2.c");
    LOGGER->writeLog(
      this_player()->query_name() + " entered the VIP area @ " + ctime(time()));
  
  if (!(this_player()->query_explorer_flag(this_object()))){
    this_player()->add_explorer_flag(object_name(this_object()));
  }
  
  return 1;
}

// Debug door for wizards
int wiz_door(){
  if (wizardp(this_player())) {
    this_player()->move_player(
      "without saying goodbye. What a jerk!", ROOM_DIR + VIP + "vip2.c");
      return 1;
    }
  else return notify_fail("There is no exit leading north...");
}