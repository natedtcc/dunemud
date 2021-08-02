// proc11.c - Sapho Processing Area entrance - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{

  ::reset(arg);
  if (!arg){
   
    set_light(1);
    set_short("$BOLD$REDSapho $OFF$WHITEProcessing Area$OFF");
    set_long(
"You've reached the entance of the processing area, where the red-tar sapho\n"
+"is actually being created. The path moves around in a circular shape, with\n"
+"tons of heavy industrial equipment surrounding you on all sides. There is\n"
+"a strange acrid aroma coming from this place, which you suspect to most\n"
+"likely be the chemicals used in the processing factory. After all, the\n"
+"workers here are all decked out in full hazmat suits while they pour\n"
+"sketchy looking liquids into huge metallic vats. The exit is to the\n"
+"west.");

    add_exit("north", ROOM_DIR + PROC + "proc12.c");
    add_exit("south", ROOM_DIR + PROC + "proc10.c");
    add_exit("west", ROOM_DIR + "fhall2.c");

    add_item(
      "equipment", "Tons of fancy, heavy looking lab equipment used in the chemical\nextraction of sapho.");

    add_item(
      "chemicals", "Lord knows what these people are using to make this potent extract..");

    add_item(
      "vats", "Large metallic containers for mixing chemicals.");
  }

  place_objects(NPC_DIR + "procworker.c", 4);

}