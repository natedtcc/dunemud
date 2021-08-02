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
You are standing at the great circle of equals, you\n\
feel something unusual connected with this place,\n\
some kind of strong force coming from it. In the\n\
distance you notice a few kids playing and running\n\
around the tents. Leaving this place is probably\n\
the best you could you for yourself. \n";
    items = (["tents":"Just a few fremen kids.",
              "walls":"Dark, plain walls.",
            "torches":"Simple torches.",
             "circle":"You should come closer.",
             "corner":"You see nothing special.",
    ]);
    dest_dir = (["east":ROOM_DIR + LIVSEC + "croom8.c",
                 "west":ROOM_DIR + LIVSEC + "croom6.c",
                "north":ROOM_DIR + LIVSEC + "croom.c",
                "south":ROOM_DIR + LIVSEC + "11mroom.c",
    ]);
  }

  place_objects(NPC_DIR + "fremen.c", 2);
}
