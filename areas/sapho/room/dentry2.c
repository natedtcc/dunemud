/// dentry2.c - Sapho Den Entry - Bebop 2019
#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  if (arg)
    return;

  set_light(1);
  set_short("$BOLDEntrance to a $REDSapho$OFF Den");
  set_long(
      "\nYou have found your way into some kind of Sapho den. The smell here\n" + "is pretty strong, but you cant quite put your finger on it. There's\n" + "an odd, ash-like substance on the floor in front of you. The walls\n" + "here have a reddish tint to them, and are sticky to the touch. You hear\n" + "laughing and coughing up ahead, as you wonder what you are getting\n" + "yourself into.\n");
  add_item("ash", "It looks like something that someone might have been smoking.");
  add_item("walls", "A thick reddish film covers the walls here.");
  add_exit("north", ROOM_DIR + "sapho2.c");
  add_exit("south", ROOM_DIR + "dentry1.c");
}