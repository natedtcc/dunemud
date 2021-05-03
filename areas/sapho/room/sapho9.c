/// sapho9.c - Sapho Den - Bebop 2019

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
  set_short("$BOLD$WHITEA $REDSapho $WHITEDen$OFF");
  set_long(
      "Loads of tar cover the walls in this area. You wonder if they ever hire\n" + "someone to come in and clean the place, though you imagine they probably\n" + "don't. There's a painting of a massive glistening rock of red-tar sapho\n" + "hanging on the wall here. All the junkies here are babbling about how the\n" + "last batch of sapho was pretty weak. Seems like they're gaining tolerance.\n");

  add_exit("south", ROOM_DIR + "sapho6.c");
  add_exit("west", ROOM_DIR + "sapho8.c");

  add_item("painting",
           "This painting is rather unremarkable, except for it's crooked frame.");
  add_item("frame",
           "While investigating the frame you notice some fingerprints.");
  add_item("fingerprints",
           "You touch the fingerprints and a mechanical panel springs open, revealing\n" + "a button!");

  add_item(
      "junkies", "A bunch of babbling fools.");
}
void init()
{
  ::init();

  add_action("push", "push");
  add_action("push", "press");
}

int push(string str)
{
  if (str == "button")
  {
    write(
        "\n\nAs you press the button, you are sucked into the floor!!!" + "\n\nYou land softly.\n\n");

    this_player()->move_player(
        "gets sucked through the floor.\n", ROOM_DIR + "fentry.c");

    this_player()->add_explorer_flag(object_name(this_object()));

    /* Attempt to generate a keycard when the player enters the area */

    keycard_gen();

    return 1;
  }

  else
    return 0;
}
