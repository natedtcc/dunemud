// Elevator keycard - Sapho Facility - Bebop 2020
// Special thanks to Bubbs for helping we with lifetime.c

#include <mudlib.h>

inherit OBJECT;

inherit "/players/bebop/area/sapho/include/lifetime.c";

void reset(int arg)
{

  ::reset(arg);
  if (arg)
    return;

  set_name("keycard");
  set_alias(({"keycard", "bebop_vip_keycard"}));
  set_short("A small plastic keycard");
  set_long(
      "A small plastic keycard, with the letters \"VIP\" printed on it. Apparently, this\n"
      +"particular keycard is highly sought after by workers and guards alike, and could\n"
      +"be stolen from you at any time.\nYou could try to 'swipe' it, wherever you can use it...");
  set_weight(0);
  set_value(0);

  // Display message when keycard is loaded..
  if (environment())
  {
    tell_others(environment(),
        "\nOut of the corner of your eye, you notice a shady looking sapho smuggler\n"
        +"attempting to sell what appears to be a keycard to %N.\n\n", 2);
  }

  // Set the keycard to despawn after 30 minutes
  set_lifespan(40 * 60, #'check_lifetime, #'complete_lifetime);
}

init()
{

  ::init();
  add_action("insert_key", "insert");
}

int insert_key(string str)
{
  if (str == "keycard")
  {
    // move the player
  }
  else
    return notify_fail("Insert what? Keycard?");
}

