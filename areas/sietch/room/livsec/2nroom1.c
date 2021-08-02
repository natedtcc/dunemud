inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Northern part of sietch";
    long_desc = "\
You are now standing in the northern part of\n\
fremen sietch. You can see few fremens around\n\
you. They all look very well trained and able\n\
to survive living in desert. They children also\n\
look very well trained even at their youth.\n";
    items = (["rocks":"Cold, sable rocks surrounds you.",
              "walls":"Dark, plain walls.",
            "windows":"Tiny windows hewn in the rocks.",
             "window":"All you see is endless dune.",
            "torches":"Simple torches.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "2nroom2.c",
                "south":ROOM_DIR + LIVSEC + "nwmroom.c",
                 "west":ROOM_DIR + LIVSEC + "nwroom4.c",
                "north":ROOM_DIR + LIVSEC + "1nroom1.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
