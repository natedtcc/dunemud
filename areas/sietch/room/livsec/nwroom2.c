inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "North-western corner.";
    long_desc = "\
You are in the north western corner of the fremen sietch.\n\
From here you can see the whole living section of this sietch.\n\
Only decorations on the walls are some torches, which also\n\
bring some light. You notice a kid playing in the distance\n\
and a huge circle in the middle of this gigantic room.\n";
    items = (["kids":"Just a few fremen kids.",
             "walls":"Dark, plain walls.",
           "torches":"Simple torches.",
            "circle":"You should come closer.",
            "corner":"You see nothing special.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "1nroom1.c",
                 "west":ROOM_DIR + LIVSEC + "nwroom1.c",
                "south":ROOM_DIR + LIVSEC + "nwroom4.c",

    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
