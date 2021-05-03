/// sapho4.c - Sapho Den - Bebop 2019

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  place_objects(NPC_DIR + "junkie.c", 1);

  if (arg)
    return;

  set_light(1);
  set_short("$BOLD$WHITEA $REDSapho $WHITEDen$OFF");
  set_long(
      "So many damn junkies. You wonder where they all come from. They all\n" + "look like caricatures of used car salesmen from Arrakis, and there\n" + "sure are a hell of a lot of them at 4:30 am on a Sunday morning. They\n" + "clearly have no idea what time it is, probably because of the lack of\n" + "outside light in this place..ideal conditions for a junkie. The walls\n" + "here have a sticky red film on them.\n");

  add_exit("east", ROOM_DIR + "sapho5.c");
  add_exit("north", ROOM_DIR + "sapho7.c");
  add_exit("south", ROOM_DIR + "sapho1.c");

  add_item("walls",
           "Sticky to the touch, and red in color. Definitely sapho tar.");
  add_item("junkies",
           "These people are gone, dead to the world. No use trying to communicate\n" + "with these babbling, drooling junkies.");
}
