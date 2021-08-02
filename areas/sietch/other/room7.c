inherit "/room/newroom";
#include "gen.h"
int check;
reset(arg)
{
  check = 0;
  if (arg)
    return;
  set_planet("giedi prime");
  set_light(0);
  short_desc = "Weapons cache";
  long_desc = "\
You have found the hidden weapons cache. It is a small \n\
room, but packed wall to wall, floor to ceiling with weapons\n\
of all sorts and sizes. Dust covers most of them, but some \n\
look to be recently put here quite recently. Search around a bit.\n";
  items = (["cache":"This is the weapons cache of the bunker.",
             "room":"=cache",
          "weapons":"There are tons of them, but you can only take one.",
  ]);
  dest_dir = (["up":"/d/Harko/sauron/base/admin/room3",
  ]);
}
init()
{
  ::init();
  add_action("search", "search");
}
search()
{
  if (check)
  {
    write("The weapons cache has already been scavenged.\n");
    return 1;
  }
  if (!random(3))
  {
    write("You search around a bit and find a nice Crysknife.\n");

    // Changed to reflect the updated quest area - Bebop 2021
    move_object(clone_object("/d/Fremen/bebop/sietch/obj/quest_crys"), this_player());
    check = 1;
    if (present("crysknife", this_player()))
      present("crysknife", this_player())->set_finder(TP->query_real_name());
    log_file("sauron.log", TP->QN + " found the crysknife on " + ctime(time()) + ".\n");
    return 1;
  }
  else
  {
    write("You search around but fail to find anything of value.\n");
    return 1;
  }
}