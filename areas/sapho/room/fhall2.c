// fhall2.c - Sapho Facility Hallway - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

// Include keycard generator
inherit "/players/bebop/area/sapho/include/keycard_gen.c";

void reset(int arg)
{

  ::reset(arg);

  // Generate a keycard on reset..

  keycard_gen();

  place_objects(NPC_DIR + "door_guard.c", 2);

  if (arg)
    return;

  set_light(1);
  set_short("$WHITEA hallway in the refinery");
  set_long(
      "You stand at the center of the long hallway, with two doors on each side\n" + "of you. The big guards here do not look too keen on letting you inside these\n" + "rooms, at least without a fight. A sign above the east door reads \"Processing\",\n" + "while the sign above the western door says \"Packaging\". All the windows\n" + "here are blacked out, so you have no idea what's really inside.\n");

  add_exit("south", ROOM_DIR + "fhall1.c");
  add_exit("north", ROOM_DIR + "fhall3.c");
  add_exit("west", ROOM_DIR + "pack3.c");
  add_exit("east", ROOM_DIR + "proc11.c");

  add_item("doors",
           "There's two doors, one to the west and one two the east. You also see what\n"
           +"seems to be an elevator way down the hallway.");
}

void init()
{
  ::init();

  // Add actions for east / west movement to block player

  add_action("check", "east");
  add_action("check", "west");
}

/* Block players from entering eastern and western rooms 
  until they kill the guards in the room. */

check()
{
  if (present("guard"))
  {
    tell_object(this_player(),
                "The guard frowns and says: I ought to end you right now.\n");
    tell_room(this_object(),
              "\nThe Sapho-crazed guard threatens to murder " +
                  this_player()->query_name() +
                  ".\n",
              ({this_player()}));
    return 1;
  }

  return 0;
}
