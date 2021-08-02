// pack2.c - Sapho Packaging Area - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{
  ::reset(arg);
  if (!arg){
    
    set_light(1);
    set_short(parse_ansi("$BOLD$REDSapho $OFF$WHITEPackaging Area$OFF"));
    set_long(
"The smell here is incredible! As you wander through the aisles of the\n" 
+"packaging area, you can't help but be enchanted by the sweet aroma\n" 
+"emitted by the abundance of red-tar sapho around you. The workers are\n" 
+"here, standing at their tables, weighing different amounts of red-tar\n"
+"depending on it's destination. They seem to tolerate you being here, but\n"
+"it wouldn't be wise to stay too long.\n");

    add_exit("west", ROOM_DIR + PACK + "pack1.c");
    add_exit("east", ROOM_DIR + PACK + "pack3.c");
    add_exit("south", ROOM_DIR + PACK + "pack5.c");

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
  
  place_objects(NPC_DIR + "packworker.c", 4);

}
