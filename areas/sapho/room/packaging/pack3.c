// pack3.c - Sapho Packaging Area - Bebop 2020

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
"You've made it to the entrance of the red tar sapho packaging area, where the\n"
+"finished product goes for the finishing touches. You see tons of big storage\n"
+"totes filled with huge stones of red tar, ready to be broken down into smaller\n"
+"bits for distribution. An aisle is visible to the west, running north to south\n"
+"through the area. You see several packaging workers working on long tables,\n"
+"weighing gram baggies for their potential customers - and they're wearing only\n"
+"their underwear and an N95 mask. Considering the amount of guards in this area,\n"
+"you assume this dresscode is enforced to deter theft. To the east is the main\n"
+"facility hallway.\n");

    add_exit("west", ROOM_DIR + PACK + "pack2.c");
    add_exit("east", ROOM_DIR + "fhall2.c");
    add_exit("south", ROOM_DIR + PACK + "pack6.c");

    add_item(
      "stones", "Red tar sapho slabs bigger than your head.");

    add_item(
      "tables", "A bunch of tables that are neatly lined on either side of the aisle. They are\n"
        +"covered sapho and digital scales.");

    add_item(
      "aisle", "It runs from north to south, and has many tables lined on either side of it.\n"
        +"The aisle begins in the room west of here.");

    add_item(
      "underwear", "PERVERT ALERT!");

  }
  place_objects(NPC_DIR + "packworker.c", 4);
}