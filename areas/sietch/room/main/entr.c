inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"
void reset(int arg)
{
  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Fremen sietch";
    long_desc = "\
You have just entered the fremen sietch. This place\n\
seem dangerous for you. To the north you hear a voices\n\
and to the south you notice a nice hallway which ends\n\
at some strange room. Torches are lined up at the walls\n\
Your intuition tells you to get out of here as fast as\n\
you can. \n";
    dest_dir = (["north":ROOM_DIR + MAIN + "ientr.c",
                 "south":ROOM_DIR + MAIN + "lsentr.c",
                   "out":"d/Fremen/desert/room/sietch13.c",
    ]);
    items = (["torches":"Simple torches.",
              "hallway":"You have walk trough it.",
    ]);
  }
}

// Added an explorer flag to the entrance - perhaps this will
// entice players to explore further? - Bebop 2021

void init()
{
  ::init();
  this_player()->add_explorer_flag(object_name(this_object()));
}
