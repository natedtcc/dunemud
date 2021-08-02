// fentry.c - Sapho Refinery Entry - Bebop 2019

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
    set_short(parse_ansi("$WHITEThe $BOLD$REDSapho$OFF $WHITERefinery$OFF"));
    set_long(
  "After falling through the floor, you gather your bearings. You find yourself\n" 
  +"inside of a dimly lit hallway, with shady looking individuals with hazmat\n"
  +"gear shuffling to and from the different rooms in the hallway. They seem to\n"
  +"be carrying different types of chemicals and strange bags, full of what looks\n"
  +"like red-tar sapho. You also notice some pretty heavily armed guards, who\n"
  +"haven't quite noticed you yet. By the looks of things, you have stumbled upon\n"
  +"some massive underground sapho refinery! The smell here is rather enchanting,\n"
  +"and seems to be beckoning you to draw closer to this extremely questionable\n"
  +"hallway. There's a small button behind you that will take you back up to the\n"
  +"sapho den.");

    add_exit("north", ROOM_DIR + "fhall1.c");

    add_item(
      "individuals",
        "These guys are decked out in full hazmat suits. Looks like they're the ones\n"
        +"responsible for cooking up the red-tar!");

    add_item(
      "chemicals",
        "Odd looking powders and liquids can be seen further ahead and in the hands\n"
        +"of a few of the workers.");
    
    add_item(
      "bags",
        "Looks like bags of red-tar, bundled up and ready for sale on the black market.\n");
  }

  place_objects(OBJ_DIR + "sign.c", 1);

}

void init()
{
  ::init();

  // Add actions for push and press to return to the sapho den

  add_action("push", "push");
  add_action("push", "press");
}

// If the button is pushed, move player back to the den...

int push(string str)
{
  if (str == "button")
  {
    write(
      "\n\nAs you press the button, you are sucked into the ceiling!!!\n"
      +"How does that even work?!\n\nYou slam into the ceiling of"+
      "the sapho den and land flat\non your face.\n\n");

    this_player()->move_player(
      "gets sucked through the ceiling", ROOM_DIR + DEN + "den9.c");

    /* Attempt to generate a keycard when the player leaves the area */
    LOGGER->writeLog(
      this_player()->query_name() + " attempting to spawn a keycard (fentry) @ " + ctime(time()));
    keycard_gen();
    
    return 1;
  }
  else
    return 0;
}
