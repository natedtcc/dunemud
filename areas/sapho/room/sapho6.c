/// sapho6.c - Sapho Den - Bebop 2019

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  place_objects(NPC_DIR + "junkie.c", 2);

  if (arg)
    return;

  set_light(1);
  set_short("$BOLD$WHITEA $REDSapho $WHITEDen$OFF");
  set_long(
      "Another junkie-filled and tar-stained room. You hear two junkies bickering\n" + "over their last little slab of red-tar. Other than that, the atmosphere is\n" + "quite relaxing here. There's a viewscreen playing some trippy videos for the\n" + "inebriated patrons of the sapho den.\n");

  add_exit("south", ROOM_DIR + "sapho3.c");
  add_exit("north", ROOM_DIR + "sapho9.c");
  add_exit("west", ROOM_DIR + "sapho5.c");

  add_item("walls",
           "Sticky to the touch, and red in color. Definitely sapho tar.");
  add_item("junkies",
           "These two junkies are about to get very violent over a thumbnail-sized0\n" + "sliver of sapho.");
  add_item("viewscreen",
           "You are briefly entranced by vivid colors on the viewscreen.");
}
