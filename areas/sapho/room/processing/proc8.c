// proc8.c - Sapho Processing Area Southern Quarter - Bebop 2021

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
"More and more equpment lines the walls here, as processing workers move\n"
+"diligently to get their next batch of product ready. You can see a few\n"
+"bins of red tar sapho stacked along the wall here, ready to be taken to\n"
+"the packaging area. Two workers are arguing over how much of a certain\n"
+"powder they should add to a solution, and you feel awkward for watching.\n");

    add_exit("west", ROOM_DIR + PROC + "proc9.c");
    add_exit("east", ROOM_DIR + PROC + "proc7.c");

    add_item(
      "bins", "Large storage totes filled to the brim with huge masses of red tar.");

    add_item(
      "powder", "A fluffy looking off-white substance.");

    add_item(
      "solution", "This solution looks amber in colour, and quite cloudy.");

  }

  place_objects(NPC_DIR + "procworker.c", 4);

}