inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "South-eastern corner.";
    long_desc = "\
You are standing in the south-eastern corner of this\n\
fremen sietch. Right before your eyes are placed few\n\
desert tents. In the center you see a huge, red circle,\n\
this might be the legendary fremen circle of equals, where\n\
they fight each other to determine which one will be the\n\
next leader. \n";
    items = (["kids":"Just a few fremen kids.",
             "walls":"Dark, plain walls.",
           "torches":"Simple torches.",
            "circle":"You should come closer.",
            "corner":"You see nothing special.",
    ]);
    dest_dir = (["west":ROOM_DIR + LIVSEC + "seroom1.c",
                "north":ROOM_DIR + LIVSEC + "1eroom5.c",
                "south":ROOM_DIR + LIVSEC + "seroom4.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
