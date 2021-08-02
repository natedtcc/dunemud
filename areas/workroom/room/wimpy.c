// Battle testing - Bebop 2021

#include <mudlib.h>
inherit ROOM;

void reset(int arg)
{

  ::reset(arg);
  if(arg) return;

  set_light(1);
  set_short(" The Wimpy Room ");

  set_long(
      "\nYou ran!\n"
      "Go south to return to the fight.\n");

  add_exit("south", "/players/bebop/room/btest");

}