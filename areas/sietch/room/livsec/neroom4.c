inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "North-eastern corner.";
    long_desc = "\
You are in the north western corner of the fremen\n\
sietch. From here you can see the whole living section\n\
of this sietch. Only decorations on the walls are some\n\
torches, which also bring some light. You notice a kid\n\
playing in the distance and a huge circle in the middle\n\
of this gigantic room.\n";
    items = (["kids":"Just a few fremen kids.",
             "walls":"Dark, plain walls.",
           "torches":"Simple torches.",
            "circle":"You should come closer.",
            "corner":"You see nothing special.",
    ]);
    dest_dir = (["west":ROOM_DIR + LIVSEC + "neroom3.c",
                "south":ROOM_DIR + LIVSEC + "1eroom1.c",
                "north":ROOM_DIR + LIVSEC + "neroom2.c",

    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
