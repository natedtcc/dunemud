// proc9.c - Sapho Processing Area Southern Quarter - Bebop 2021

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
"The amount of lab equipment here is staggering. Large vats with hood vents\n"
+"are dotted throughout the lab, most likely where the final purging process\n"
+"takes place. There are also large drying racks with sheet trays containing\n"
+"what appears to be the finished product, becoming more maliable as it cools\n"
+"after the extraction process. You are starting to get a bit dizzy from all\n"
+"the fumes in this place.\n");

    add_exit("west", ROOM_DIR + PROC + "proc10.c");
    add_exit("east", ROOM_DIR + PROC + "proc8.c");

    add_item(
      "equipment", "Tons of fancy, heavy looking lab equipment used in the chemical\nextraction of sapho.");

    add_item(
      "vats", "Wide cylindrical metal containers, most likely used for vac-purging.");

    add_item(
      "racks", "You can see some of the finished product that has cooled a bit here.");
  }

  place_objects(NPC_DIR + "procworker.c", 4);

}