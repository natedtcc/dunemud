// sapho3.c - Sapho Den - Bebop 2019

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{

  ::reset(arg);
  if (!arg) {

    set_light(1);
    set_short(parse_ansi("$BOLD$WHITEA $REDSapho $WHITEDen$OFF"));
    set_long(
      "There are many junkies visible in this room. There are green and yellow\n"
      +"beanbags strewn across the floor, almost every one of them occupied\n"
      +"by a comatose junkie. You can hear the sound of lighters clicking and\n"
      +"pipes puffing every few seconds, and the smoke in this room is pretty\n"
      +"damn thick. The walls are stained red with tar from the large amount\n"
      +"amount of $BOLD$REDSapho$OFF being smoked here.\n");

    add_exit("west", ROOM_DIR + DEN + "den2.c");
    add_exit("north", ROOM_DIR + DEN + "den6.c");

    add_item(
      "pipes",
          "Curved, elongated pipes, all full with red-tar Sapho.\n");

    add_item(
      "junkies", 
          "Most of the beanbags are occupied by them, and you doubt that\n"
          +"they will be moving any time soon.");

    add_item(
      "beanbags",
          "Brightly colored and highly sought after.");

    add_item(
        "walls",
          "Covered with a mild red tint and slightly sticky.");
  }
      
  place_objects(NPC_DIR + "junkie.c", 3);

}
