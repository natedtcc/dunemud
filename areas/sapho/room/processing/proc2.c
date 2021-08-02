// proc2.c - Sapho Processing Area Northern Quarter - Bebop 2021

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{

  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short(parse_ansi("$BOLD$REDSapho $OFF$WHITEProcessing Area$OFF"));
    set_long(
"This particular spot in the processing area is rather warm, as you're\n"
+"standing next to two gigantic heating tanks. These tanks house some\n"
+"kind of solvent that is used in the sapho refinement process, which\n"
+"must be kept at a constant 500 degrees celsius. A worker fiddles with\n"
+"a knob on a tank, also taking note of the pressure on a small digital\n"
+"screen. You wonder why they just let you wander around in here....\n");

    add_exit("west", ROOM_DIR + PROC + "proc1.c");
    add_exit("east", ROOM_DIR + PROC + "proc3.c");

    add_item(
      "tanks", "Huge vats of an unknown, most likely toxic, chemcical substance.");

    add_item(
      "screen", "It reads 421 PSI. That's kinda high, ain't it?");

    add_item(
      "racks", "Probably a knob to adjust the pressure of the tank. No, you can't\n"
      +"turn it.");

  }

  place_objects(NPC_DIR + "procworker.c", 4);

}