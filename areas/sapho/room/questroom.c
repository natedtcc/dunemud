// questroom.c - Boss room / quest item location - Bebop 2018
#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg) {
  
  ::reset(arg);
  if (!arg){

    set_short("A hidden alcove");
    set_long(
      "You're in a small room with barely any light. There's a little table here\n"
      +"with a bit of food on it, most likely being eaten by the gentleman sitting\n"
      +"at the table. You wonder what this person is doing here...\n");
    
    set_property("no_fight");
    set_light(1);

    
    add_exit("west", ROOM_DIR + PROC + "proc6.c");
  }
      
  place_objects(NPC_DIR + "smuggler_quest.c", 1);

}
