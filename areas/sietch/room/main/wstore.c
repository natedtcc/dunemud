#include <mudlib.h>
#include <ansi.h>
inherit ROOM;
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(mixed arg)
{
  ::reset(arg);
  if (!arg)
  {
    set_property("no_fight");
    set_light(1);
    set_short("Sietch water store");
    set_long (
"You have arrived to the huge sietch water store room. In the\n\
middle of the room you see a huge hole full of water. And an old man\n\
sitting in the corner. You feel something unusual, something you can't\n\
explain. Perhaps it's your intuition trying to tell you something.\n");
 
    add_item ("sand", "You see nothing special about it.");
    add_item ("rocks", "Cold sable rocks surround you.");
    add_item ("water", "The water has a silvery tint.");
    add_item ("walls", "Plain walls made of huge rocks.");
    add_item ("hole", "This hole was hewn in the rocks.");
    add_exit ("up", ROOM_DIR + MAIN + "sentr");
  }

  place_objects(NPC_DIR + "wisfrem", 1);
}