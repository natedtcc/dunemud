// sapho7.c - Sapho Den - Bebop 2019

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{
  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short(parse_ansi("$BOLD$WHITEA $REDSapho $WHITEDen$OFF"));
    set_long(
      "Not much to see in this dreary corner of the sapho den. There are a few\n"
      +"chairs strewn about, although none are currently occupied. Like every\n"
      +"other room in this den, there is a sticky red film covering the walls,\n"
      +"a sign of heavy sapho smoking.\n");

    add_exit("east", ROOM_DIR + DEN + "den8.c");
    add_exit("south", ROOM_DIR + DEN + "den4.c");

    add_item(
      "chairs", "They look pretty comfy.\n");
    add_item(
      "film",
        "Because of this film, everything is tinted red and slighyl sticky.\n");
  }

  place_objects(NPC_DIR + "junkie.c", 2);

}
