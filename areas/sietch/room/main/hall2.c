inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"
void reset(int arg)
{
  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Hallway to water store.";
    long_desc = "\
You step in huge hallway with several guards\n\
to both sides of the path you think that this\n\
path probably leads to sietch water reserves.\n\
In the distance to the south of you are placed\n\
living houses of fremens, you also see small\n\
kids playing around them.\n";
    dest_dir = (["south":ROOM_DIR + MAIN + "hall1",
                  "east":ROOM_DIR + MAIN + "room3",
                  "west":ROOM_DIR + MAIN + "room4",
                 "north":ROOM_DIR + MAIN + "hall3",
    ]);
    items = (["sand":"You see nothing special about it.\n",
             "rocks":"Cold, sable rocks surrounds you.\n",
             "walls":"Plain walls, made of huge rocks.\n",
           "windows":"You see some windows.\n",
            "window":"You can see the desert of Arrakis outside.\n",
    ]);
  }

}
