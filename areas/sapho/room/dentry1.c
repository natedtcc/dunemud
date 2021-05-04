
/// dentry1.c - Sapho Den Entry - Bebop 2019
#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  if (arg)
    return;

  set_light(1);
  set_short("A short hallway");
  set_long("\nThis is a very short, plainly decorated hallway. Not too\n\
much to see here. You can smell something burning in the distance. The\n\
Lounge is to the south. This hallway continues to the north.\n");

  add_exit("south", "/players/bebop/room/lounge.c");
  add_exit("north", ROOM_DIR + "dentry2.c");
}