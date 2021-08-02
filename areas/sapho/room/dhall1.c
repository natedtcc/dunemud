
// dentry1.c - Sapho Den Entry - Bebop 2019
#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{

  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short("A short hallway");
    set_long(
      "This is a very short, plainly decorated hallway. Not too\n"
      +"much to see here. You can smell something burning in the distance. The\n"
      +"Lounge is to the south. This hallway continues to the north.\n");

    add_exit("north", ROOM_DIR + "dhall2.c");
    add_exit("west", "/d/Imperial/domain/2main5.c");
  }
}
