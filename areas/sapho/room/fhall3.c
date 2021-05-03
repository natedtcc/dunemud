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
  set_short("$WHITEA hallway in the refinery");
  set_long(
      "\nHallway - LOREM IPSUM" + "\n" + "\n" + "\n");

  add_exit("south", ROOM_DIR + "fhall2.c");
  add_exit("north", ROOM_DIR + "fhall4.c");
  // add_exit("north", ROOM_DIR + "fhall2.c");
  // add_exit("north", ROOM_DIR + "fhall2.c");
}