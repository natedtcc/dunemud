/// fentry.c - Sapho Refinery Entry - Bebop 2019

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

// Include keycard generator
inherit "/players/bebop/area/sapho/include/keycard_gen.c";

void reset(int arg)
{

  ::reset(arg);

  if (arg)
    return;

  set_light(1);
  set_short("$WHITEThe $REDSapho $WHITERefinery$OFF");
  set_long(
      "After falling through the floor, you gather your bearings. You find yourself\n" + "inside of a dimly lit hallway, with shady looking individuals with hazmat gear\n" + "shuffling to and from the different rooms in the hallway. They seem to be carrying\n" + "different types of chemicals and strange bags, full of what looks like red-tar\n" + "sapho. You also notice some pretty heavily armed guards, who haven't quite noticed\n" + "you yet. By the looks of things, you seem to have stumbled upon some massive underground\n" + "sapho refinery! The smell here is rather enchanting, and seems to be beckoning you to\n" + "draw closer to this extremely questionable hallway. There's a small button behind you\n" + "that will take you back up to the sapho den.\n");

  add_exit("north", ROOM_DIR + "fhall1.c");

  add_item("individuals",
           "These guys are decked out in full hazmat suits. Looks like they're the ones\n" + "responsible for cooking up the red-tar!");
  add_item("chemicals",
           "Odd looking powders and liquids can be seen further ahead and in the hands\n" + "of a few of the workers.");
  add_item("bags",
           "Looks like bags of red-tar, bundled up and ready for sale on the black market.\n");
}
void init()
{
  ::init();

  /// Add actions for push and press to return to the sapho den

  add_action("push", "push");
  add_action("push", "press");
}

/// If the button is pushed, move player back to the den...

int push(string str)
{
  if (str == "button")
  {
    write(
        "\n\nAs you press the button, you are sucked into the ceiling!!!\n" + "How does that even work?!\n\nYou slam into the ceiling of" + " the sapho den and land flat\non your face.\n\n");

    this_player()->move_player(
        "gets sucked through the ceiling!\n", ROOM_DIR + "sapho9.c");

    /* Attempt to generate a keycard when the player leaves the area */
    keycard_gen();

    return 1;
  }
  else
    return 0;
}
