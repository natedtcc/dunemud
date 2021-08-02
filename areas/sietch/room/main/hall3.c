inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"
void reset(int arg)
{
  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Middle of the hallway.";
    long_desc = "\
You are at the middle of the hallway leading\n\
to water reserves and opposide direction to\n\
living section. Two guards are standing next\n\
to you. A dim light comes in from windows\n\
irradiating the footpath you have to travel on.\n";
    dest_dir = (["south":ROOM_DIR + MAIN + "hall2",
                  "east":ROOM_DIR + MAIN + "room5",
                  "west":ROOM_DIR + MAIN + "room6",
                 "north":ROOM_DIR + MAIN + "hall4",
    ]);
    items = (["sand":"You see nothing special about it.\n",
             "rocks":"Cold, sable rocks surrounds you.\n",
             "walls":"Plain walls, made of huge rocks.\n",
           "windows":"You see some windows.\n",
            "window":"You can see the desert of Arrakis outside.\n"]);
  }

}
