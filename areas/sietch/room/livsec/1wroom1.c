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
You are in the western part of the sietch,\n\
right next to you is wall with a few tiny\n\
windows. A narrow shelf occupies the wall.\n\
The shelf holds a row of portraits - mostly bared\n\
fremen, some in still suits with their catch\n\
tubes dangling, some in Imperial uniforms posed\n\
against exotic offworld backgrounds. You notice\n\
few fremen woman standing in a distance and doing\n\
something.\n";
    items = (["rocks":"Cold, sable rocks surrounds you.",
              "walls":"Dark, plain walls.",
            "windows":"Tiny windows hewn in the rocks.",
             "window":"All you see is endless dune.",
            "torches":"Simple torches.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "2wroom1.c",
                "south":ROOM_DIR + LIVSEC + "1wroom2.c",
                "north":ROOM_DIR + LIVSEC + "nwroom3.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
