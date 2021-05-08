// proc11.c - Sapho Processing Area entrance - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

    place_objects(NPC_DIR + "procworker.c", 3);

  if (arg)
    return;
  set_light(1);
  set_short("$BOLD$REDSapho $OFF$WHITEProcessing Area$OFF");
  set_long(
      "You've reached the entance of the processing area, where the red-tar sapho\n"
      +"is actually being created. The path moves around in a circular shape, with\n"
      +"tons of heavy industrial equipment surrounding you on all sides. There is a\n"
      +"strange acrid aroma coming from this place, which you suspect to most likely\n"
      +"be the chemicals used in the processing factory. After all, the workers here\n"
      +"are all decked out in full hazmat suits while they pour sketchy looking liquids\n"
      +"into huge metallic vats. The exit is to the west.\n");

  add_exit("north", ROOM_DIR + "proc12.c");
  add_exit("south", ROOM_DIR + "proc10.c");
  add_exit("west", ROOM_DIR + "fhall2.c");

  add_item(
      "equipment", "Tons of fancy, heavy looking lab equipment used in the chemical\nextraction of sapho.");

  add_item(
      "chemicals", "Lord knows what these people are using to make this potent extract..");

  add_item(
      "vats", "Large metallic containers for mixing chemicals.");
}