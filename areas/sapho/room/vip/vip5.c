// vip5.c - VIP Lounge Entryway - Bebop 2021

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{

  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short(parse_ansi("$WHITEThe $YELLOWVIP$OFF Lounge - Hallway"));
    set_long(
      "You are walking along the circular hallway inside the VIP area. The path\n"
      +"continues to the north. You can see a few bouncers up ahead. Wonder what\n"
      +"they're doing in the hallway?\n");

    add_exit("south", ROOM_DIR + VIP + "vip3.c");
    add_exit("north", ROOM_DIR + VIP +"vip8.c");

    add_item("bouncers",
      "Seems like they're guarding something.");

    add_item("hallway",
      "It's a pretty plain hallway.");

  }
}