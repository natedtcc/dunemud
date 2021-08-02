// fhall4.c - Sapho Facility Hallway - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{
  ::reset(arg);
  if (!arg){
    set_light(1);
    set_short("A hallway in the facility");
    set_long(
      "You stand at the center of the long hallway, with two doors on each side\n"
      +"of you. The big guards here do not look too keen on letting you inside these\n"
      +"rooms, at least without a fight. A sign above the east door reads \"Processing\",\n"
      +"while the sign above the western door says \"Packaging\". All the windows\n"
      +"here are blacked out, so you have no idea what's really inside.\n");
    add_exit("south", ROOM_DIR + "fhall2.c");
    add_exit("north", ROOM_DIR + "fhall4.c");

    add_item(
      "doors",
        "There's two doors ahead, one to the west and one two the east. You also\n"
        +"see what seems to be a large metal door way down the hallway to the north.");

  }
}