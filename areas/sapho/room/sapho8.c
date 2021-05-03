/// sapho8.c - Sapho Den - Bebop 2019

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  if (arg)
    return;

  set_light(1);
  set_short("$BOLD$WHITEA $REDSapho $WHITEDen$OFF");
  set_long(
      "Beanbags line the walls in this room, once again mostly occupied. There's\n" + "a big group of junkies hovering around a massive sapho hookah, generating\n" + "quite a large amount of smoke. It's so strong it almost burns the eyes!\n" + "This room is otherwise largely unremarkable.\n");

  add_exit("east", ROOM_DIR + "sapho9.c");
  add_exit("south", ROOM_DIR + "sapho5.c");
  add_exit("west", ROOM_DIR + "sapho7.c");

  add_item("hookah",
           "A massive six-hose hookah, surrounded by junkies.");
  add_item("beanbags",
           "They look so damned comfortable, but they're always occupied!");
}
