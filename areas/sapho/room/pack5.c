/// pack5.c - Sapho Packaging Area - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  place_objects(NPC_DIR + "packworker.c", 4);

  if (arg)
    return;
  set_light(1);
  set_short("$BOLD$REDSapho $OFF$WHITEPackaging Area$OFF");
  set_long(
    "The smell here is incredible! As you wander through the aisles of the packaging\n" 
    +"area, you can't help but be enchanted by the sweet aroma emitted by the abundance\n" 
    +"of red-tar sapho around you. The workers are here, standing at their tables, weiging\n" 
    +"different amounts of red-tar depending on it's destination. They seem to tolerate you\n"
    +"being here, but it wouldn't be wise to stay too long.\n");
  
  add_exit("north", ROOM_DIR + "pack2.c");
  add_exit("west", ROOM_DIR + "pack4.c");
  add_exit("east", ROOM_DIR + "pack6.c");
  add_exit("south", ROOM_DIR + "pack8.c");

  add_item(
    "tables",
      "On the tables, you see several huge rocks of red sapho and a few small\n"
      +"nano-cutters used to slice smaller pieces of red-tar, as well as digital\n"
      +"scales for weighing the product.");

  add_item(
      "nano-cutters",
      "A tiny device that uses lasers to cut through objects with extreme precision.");

  add_item(
      "workers",
        "They seem extremely focused. It'd be best to leave them be.");

  add_item(
      "scales",
      "Big, flat, and rectangular with a silver plate in its center.");
}