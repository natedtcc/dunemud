inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Eastern part of sietch";
    long_desc = "\
You are in the eastern part of the fremen sietch.\n\
It seems like this place has been hewn in the\n\
rocks, many torches are hanging on the walls in\n\
metal holders. Tiny windows are hewn in the rocks.\n\
You feel the presence of guards here, but you can't\n\
see them. They are acting themselves as other\n\
fremens without doing anything special.\n";
    items = (["rocks":"Cold, sable rocks surrounds you.",
              "walls":"Dark, plain walls.",
            "windows":"Tiny windows hewn in the rocks.",
             "window":"All you see is endless dune.",
            "torches":"Simple torches.",
    ]);
    dest_dir = (["west":ROOM_DIR + LIVSEC + "2eroom1.c",
                "south":ROOM_DIR + LIVSEC + "1eroom2",
                "north":ROOM_DIR + LIVSEC + "neroom4.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
