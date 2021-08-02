inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Southern part of sietch";
    long_desc = "\
Southern part of living section in fremen\n\
sietch. You wonder how can fremens live here,\n\
it must be very hard to survive in places like\n\
this. You see a few kids running between tents.\n\
If you are a stranger exploring this area is not\n\
very good idea.\n";
    items = (["tents":"Small tents.",
               "kids":"They seem very happy.",
             "window":"All you see is endless dune.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "seroom1.c",
                 "west":ROOM_DIR + LIVSEC + "2sroom4.c",
                "south":ROOM_DIR + LIVSEC + "1sroom5.c",
                "north":ROOM_DIR + LIVSEC + "semroom.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
