// proc6.c - Sapho Processing Area Eastern Quarter - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";
// Include keycard generator
inherit "/d/Imperial/bebop/sapho/include/keycard_gen.c";

void reset(int arg)
{

  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short(parse_ansi("$BOLD$REDSapho $OFF$WHITEProcessing Area$OFF"));
    set_long(
"As you wander into the room, you bump into a worker carrying a container\n"
+"full of some kind of shiny metallic flakes. He doesn't even notice, and\n"
+"continues walking towards a large metal vat. You notice there are a ton \n"
+"of storage containers in this area; the walls are lined with drums filled\n"
+"with chemicals, tubs of powders and various other substances. This room in\n"
+"particular is extremely well ventilated, as you notice a gust of fresh air\n"
+"breezing past your nose. Part of the wall seems to be recessed here.\n");

    add_exit("north", ROOM_DIR + PROC + "proc5.c");
    add_exit("south", ROOM_DIR + PROC + "proc7.c");

    add_item(
      "flakes", "The byproduct of mixing this with a certain chemical is crucial to sapho\n"
      +"processing.");

    add_item(
      "tubs", "Big storage tubs, neatly stacked to the ceiling, full of powders.");

    add_item(
      "drums", "Most of these drums are labeled 'Methylamine'.");

    add_item(
      "vat", "This contains a certain chemical liquid that will react to the metallic\n"
      +"flakes.");

    add_item(
      "wall", "There's a small groove in the recession of the wall here...");

    add_item(
      "groove", "You touch the groove and notice a small switch!");

    add_item(
      "switch", "Perhaps you could attempt to 'flip' it?");
  }

  place_objects(NPC_DIR + "procworker.c", 4);

}

void init()
{
::init();

  add_action("flip_switch", "flip");
}

int flip_switch (string str){

    if (str != "switch") return notify_fail("Flip what? The switch?\n");
    
    write(
      "\n\nAs you flip the switch, the wall rotates you into another room!" + "\n\n");

    this_player()->move_player(
      "disappears without a trace", ROOM_DIR + "questroom.c");

    /* Attempt to generate a keycard when the player enters this area */
    LOGGER->writeLog(
      this_player()->query_name() + " attempting to spawn a keycard (proc6) @ " + ctime(time()));
    keycard_gen();

    return 1;
}