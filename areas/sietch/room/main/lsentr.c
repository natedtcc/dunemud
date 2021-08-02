

inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"

void reset(int arg)
{
  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Entrance to living sietch";
    long_desc = "\
  Entrance to the living section of this fremen sietch. To\n\
  the south you see many desert tents had been build on the\n\
  ground around some magical circle which is placed in the\n\
  center of this room. Suddenly you remember that your mother\n\
  were telling you about fremen places and you recall some\n\
  strange circle which was named a Circle of Equals. \n";
    dest_dir = (["north":ROOM_DIR + MAIN + "entr",
                 "south":ROOM_DIR + LIVSEC + "1nroom3",
    ]);
    items = (["tents":"Desert tents",
            "torches":"Simple torches.",
            "hallway":"You have walk trough it.",

    ]);
  }

  place_objects(OBJ_DIR + "sign.c", 1);
}
