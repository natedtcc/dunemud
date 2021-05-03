/// sapho2.c - Sapho Den - Bebop 2019

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
      "Welcome to the $REDSapho$OFF den. The first thing that greets you here" + "\nis a very intoxicated man, stumbling around and laughing at you\n" + "maniacaly. As you look around, you notice that there many people\n" + "all holding very elaborate and elongated pipes. They are puffing\n" + "away quite liberally on some unknown, very fruity smelling substance.\n" + "This isn't your every-day sapho den; It seems that the owners of this\n" + "den have discovered a way to both refine and concentrate the famously\n" + "intoxicating beverage into a smokable ball of tar. What a time to be\n" + "alive!\n");

  add_exit("south", ROOM_DIR + "denentrance.c");
  add_exit("west", ROOM_DIR + "sapho1.c");
  add_exit("east", ROOM_DIR + "sapho3.c");
  add_exit("north", ROOM_DIR + "sapho5.c");

  add_item("pipes", "Curved, elongated pipes, all full with red-tar sapho.");

  add_item("sapho",
           "Someone is making a concentrated verison of the famous drink. God only\n" + "knows what harsh chemical process produces this. Everyone seems to be\n" + "having a great time, though. The smell is quite strong, and different\n" + "than anything you've ever encountered.");

  add_item("people",
           "These people are gone, dead to the world. No use trying to communicate\n" + "with these babbling, drooling junkies.");

  add_item("den",
           "This place is very dimly lit, and has many low-lying bean-bag type\n" + "chairs on the floor, for easy access in case anyone needs to sit down\n" + "quickly. The overall vibe is quite chill, however the general lack of\n" + "consciousness plays a huge part in that.");
}
