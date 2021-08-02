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
You are standing at the northern wall of\n\
living section. You notice huge statuettes\n\
in all corners of this huge room. In the center\n\
is a circle of equals, where fremen fight each\n\
other to prove which of them is stronger and\n\
deserves to get higher guild level or position.\n";
    items = (["rocks":"Cold, sable rocks surrounds you.",
              "walls":"Dark, plain walls.",
            "windows":"Tiny windows hewn in the rocks.",
             "window":"All you see is endless dune.",
            "torches":"Simple torches.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "1nroom3.c",
                "south":ROOM_DIR + LIVSEC + "2nroom2.c",
                 "west":ROOM_DIR + LIVSEC + "1nroom1.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
