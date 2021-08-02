inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Southern wall.";
    long_desc = "\
You are standing at the souther wall of this\n\
fremen sietch. Right before you are many fremen\n\
desert tents. You notice a few fremens standing\n\
around the circle right in the middle of this\n\
room.\n";
    items = (["tents":"Fremen desert tents.",
             "fremen":"You have to come closer.",
            "windows":"Tiny windows hewn in the rocks.",
             "window":"All you see is endless dune.",
            "torches":"Simple torches.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "1sroom3.c",
                "north":ROOM_DIR + LIVSEC + "2sroom2.c",
                 "west":ROOM_DIR + LIVSEC + "1sroom1.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
