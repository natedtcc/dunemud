// proc7.c - Sapho Processing Area Southern Quarter - Bebop 2020

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
"You've reached a corner in the circular path around the processing facility, \n"
+"where most of the cooling red tar is held. The racks give off a reddish hue\n"
+"due to the overhead lamps, creating the illusion that the racks themselves\n"
+"are glowing red. Some workers are here loading trays onto the racks, being\n"
+"careful not to jostle the cooling substance. The fumes here are a little\n"
+"less noticable.\n");

    add_exit("west", ROOM_DIR + PROC + "proc8.c");
    add_exit("north", ROOM_DIR + PROC + "proc6.c");

    add_item(
      "trays", "Large trays of cooling red tar. It still looks quite loose.");

    add_item(
      "workers", "Hard at work. They don't appreciate being bothered.");

    add_item(
      "racks", "These racks appear to be glowing. You notice yourself being hypnotized by\n"
      +"the delicate glow of the sapho, but you quickly snap out of it.");
  }

  place_objects(NPC_DIR + "procworker.c", 4);

}