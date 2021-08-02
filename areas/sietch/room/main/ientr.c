inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_planet("arrakis");
    set_light(1);
    short_desc = "North-eastern corner.";
    long_desc = "\
You are neat to the sietch entrance. To the north\n\
from you lies the way to the water store, guarded\n\
by some very strong looking fremen naibs. At the\n\
walls you see some lighten torches. To the north\n\
you notice many tents and decide that it must be\n\
a living section of this sietch. \n";
    items = ([
      "tents"    : "Well made desert tents.",
      "walls"    : "Dark, plain walls.",
      "naibs"    : "You have to come closer.",
      "torches"  : "Simple wooden torches.",
    ]);
    dest_dir = ([
      "south" : ROOM_DIR + MAIN + "entr.c",
      "north" : ROOM_DIR + MAIN + "hall1.c",
    ]);
  }

}
