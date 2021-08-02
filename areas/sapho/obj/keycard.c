// Elevator keycard - Sapho Facility - Bebop 2020
// Special thanks to Bubbs for helping we with lifetime.c

#include <mudlib.h>

inherit OBJECT;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";
inherit "/d/Imperial/bebop/sapho/include/lifetime.c";

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("keycard");
    set_alias(({"bebop_vip_keycard"}));
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

    // Set the keycard to despawn after 35 minutes
    set_lifespan(KEYCARD_LIFETIME * 60, #'check_lifetime, #'complete_lifetime);
  }
}
