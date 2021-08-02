inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"
void reset(int arg)
{
  if (!present("guard"))
  {
    move_object(clone_object(NPC_DIR + "sguard"), this_object());
  }
  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Water Store entrance.";
    long_desc = "\
You arrive at a very important place for all\n\
fremens living in this sietch - water store.\n\
In the distance you notice some other fremens\n\
and their kids doing something. There are small\n\
windows made in the cliffs. This must be a pretty\n\
good place for living.\n";
    dest_dir = (["down":ROOM_DIR + MAIN + "wstore",
                "south":ROOM_DIR + MAIN + "hall4",
    ]);
    items = (["sand":"You see nothing special about it.",
             "rocks":"Cold, sable rocks surrounds you.",
             "walls":"Plain walls, made of huge rocks.",
           "windows":"You see some windows.",
            "window":"You can see the desert of Arrakis outside.",
    ]);
  }

  place_objects(NPC_DIR + "sguard.c", 2);
}
