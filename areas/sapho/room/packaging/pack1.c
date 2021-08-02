// pack1.c - Sapho Packaging Area corner - Bebop 2020

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
"You've reached a corner in the Sapho Packaging area. Every now and then,\n" 
+"a worker glances over at you suspiciously, curious as to why you're\n" 
+"here, fully clothed, and not wearing a respirator. Piles of boxes \n"
+"presumably containing red-tar sapho are stacked here, waiting to be\n"
+"shipped off to some unknown location.\n");

    add_exit("east", ROOM_DIR + PACK + "pack2.c");
    add_exit("south", ROOM_DIR + PACK + "pack4.c");

    add_item(
      "workers", "Can't blame them for being curious about you.");

    add_item(
      "boxes", "There are tons of them, stuffed full of red tar.");
  }
  
  place_objects(NPC_DIR + "guard.c", 2);

}