inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{

  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "In the sietch.";
    long_desc = "\
You are somewhere in the living section of this\n\
fremen sietch. You see many tents around you and\n\
a huge circle in the middle of this huge room. You\n\
think that this might be a fremen circle of equals.\n\
Being here is not a very good idea. It could be\n\
dangerous for your life. \n";
    items = (["tents":"Just a few fremen kids.",
              "walls":"Dark, plain walls.",
            "torches":"Simple torches.",
             "circle":"You should come closer.",
             "corner":"You see nothing special.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "2eroom1.c",
                 "west":ROOM_DIR + LIVSEC + "3mroom.c",
                "north":ROOM_DIR + LIVSEC + "2nroom5.c",
                "south":ROOM_DIR + LIVSEC + "5mroom.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
