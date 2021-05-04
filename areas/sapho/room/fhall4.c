// fhall4.c - Sapho Facility Hallway - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  if (arg)
    return;

  set_light(1);
  set_short("$WHITEEnd of the hallway");
  set_long(
      "\nEnd of hallway - LOREM IPSUM" + "\n" + "\n" + "\n");

  add_exit("south", ROOM_DIR + "fhall3.c");
  // add_exit("north", ROOM_DIR + "fhall2.c");
  // add_exit("north", ROOM_DIR + "fhall2.c");
  // add_exit("north", ROOM_DIR + "fhall2.c");

  add_item("sign",
           "The sign reads : \"VIP Lounge\".");

  add_item("elevator",
           "As far as elevators go, this is...an elevator. There's a\n" + "small keypad on the right side of it.");
  add_item("doors",
           "It looks like it could facilitate some kind of small keycard..\n" + "Wonder where you might find that?");
}

int read(string str)
{
  if (str == "sign")
  {
    write("The sign reads : \"VIP Lounge\".\n");
    return 1;
  }
  else
  {
    return notify_fail("Read what? The sign?");
  }
}
