inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "South-western corner.";
    long_desc = "\
You are now standing in very south-western corner of\n\
this huge room. It seems like this room has been hewn\n\
in rocks. You notice a few kids playing in distance. You\n\
notice an interesting thing - all tents are build so that\n\
they all face to the east.\n";
    items = (["kids":"Just a few fremen kids.",
             "walls":"Dark, plain walls.",
           "torches":"Simple torches.",
            "circle":"You should come closer.",
            "corner":"You see nothing special.",
    ]);
    dest_dir = (["west":ROOM_DIR + LIVSEC + "swroom1.c",
                 "east":ROOM_DIR + LIVSEC + "2sroom1.c",
                "south":ROOM_DIR + LIVSEC + "swroom4.c",
                "north":ROOM_DIR + LIVSEC + "2wroom5.c",

    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
