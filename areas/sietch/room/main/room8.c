inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"
void reset(int arg)
{
  ::reset(arg);
  if (!arg)
  {
    set_planet("arrakis");
    set_light(1);
    short_desc = "Guard post.";
    long_desc = "\
You come into well lighted room without\n\
any carpets, rock walls and small windows.\n\
You see a place for guard in the middle of\n\
this room. Also you notice a small blue stone\n\
in the corner. You wonder how could water\n\
be so important thing that it is guarded by\n\
so many guards.\n";
    dest_dir = (["east":ROOM_DIR + MAIN + "hall4",
    ]);
    items = (["sand":"You see nothing special about it.",
             "rocks":"Cold, sable rocks surrounds you.",
             "walls":"Plain walls, made of huge rocks.",
           "windows":"You see some windows.",
            "window":"You can see the desert of Arrakis outside.",
             "stone":"You see nothing special about it.",
    ]);
  }

  place_objects(NPC_DIR + "sguard.c", 2);
}
