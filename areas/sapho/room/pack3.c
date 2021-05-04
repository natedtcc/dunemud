/// pack3.c - Sapho Packaging Area - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  if (arg)
    return;
  set_light(1);
  set_short("$BOLD$REDSapho $OFF$WHITEPackaging Area$OFF");
  set_long(
      "Blah\n" + "\n" + "\n" + "\n" + "\n" + "\n");

  add_exit("west", ROOM_DIR + "pack2.c");
  add_exit("east", ROOM_DIR + "fhall2.c");
  add_exit("south", ROOM_DIR + "pack6.c");

  add_item(
      "pipes", "Curved, elongated pipes, all full with red-tar Sapho.");

  add_item(
      "junkies", "Most of the bean-bags are occupied by them, and you doubt that\n" + "they will be moving any time soon.");

  add_item(
      "den", "This place is very dimly lit, and has many low-lying bean-bag type\n" + "chairs on the floor, for easy access in case anyone needs to sit down\n" + "quickly. The overall vibe is quite chill, however the general lack of\n" + "consciousness plays a huge part in that.");
}