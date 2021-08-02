inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Western part of sietch";
    long_desc = "\
This place is western part of the Fremen\n\
sietch, you have come into living section\n\
of it. Some kids are playing around with\n\
many toes. In the distance you see a two\n\
fremen doing the combat training. This might\n\
be dangerous for you to be here.\n";
    items = (["kids":"Just a few fremen kids.",
             "walls":"Dark, plain walls.",
           "windows":"Tiny windows hewn in the rocks.",
            "window":"All you see is endless dune.",
           "torches":"Simple torches.",
              "toes":"You can' t take them away from children.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "6mroom.c",
                 "west":ROOM_DIR + LIVSEC + "1wroom3.c",
                "south":ROOM_DIR + LIVSEC + "2wroom4.c",
                "north":ROOM_DIR + LIVSEC + "2wroom2.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
