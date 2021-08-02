// sapho1.c - Sapho Den - Bebop 2019

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
    +"bean-bags strewn across the floor, almost every one of them occupied\n"
    +"by a comatose junkie. You can hear the sound of lighters clicking and\n"
    +"pipes puffing every few seconds, and the smoke in this room is pretty\n"
    +"damn thick. The walls are stained red with tar from the large amount\n"
    +"amount of $BOLD$REDSapho$OFF being smoked here.\n");

  add_exit("east", ROOM_DIR + DEN + "den2.c");
  add_exit("north", ROOM_DIR + DEN + "den4.c");

  add_item(
    "pipes", 
      "Curved, elongated pipes, all full with red-tar Sapho.");

  add_item(
    "junkies", 
      "Most of the bean-bags are occupied by them, and you doubt that\n"
      +"they will be moving any time soon.");

  add_item(
    "den", 
      "This place is very dimly lit, and has many low-lying bean-bag type\n"
      +"chairs on the floor, for easy access in case anyone needs to sit down\n"
      +"quickly. The overall vibe is quite chill, however the general lack of\n"
      +"consciousness plays a huge part in that.");
  }
 place_objects(NPC_DIR + "junkie.c", 2);

}