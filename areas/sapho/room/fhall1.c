// fhall1.c - Sapho Facility Hallway - Bebop 2020

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
      "The smell here is amazing. You feel almost hypnotized by it! As you look\n" + "around, you see a lot of very unfriendly faces. You get the feeling you\n" + "really don't belong here. You see a couple doors ahead, with guards standing\n" + "nearby looking for people just like you. You see workers carrying supplies to\n" + "different parts of the building. Who is running this place? Maybe you should\n" + "get a closer look at the operation?\n");

  add_exit("south", ROOM_DIR + "fentry.c");
  add_exit("north", ROOM_DIR + "fhall2.c");

  add_item("guards",
           "They look...large. And pissed.");
  add_item("doors",
           "There's two doors, one to the west and one two the east. You also see what\n" + "seems to be an elevator way down the hallway.");
}
