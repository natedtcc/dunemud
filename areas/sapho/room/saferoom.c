#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg) {
  

  ::reset(arg);
  
  place_objects(NPC_DIR + "gabriel.c", 1);
  
  if(arg)
  return;
  set_light(1);
  set_short("$BOLD$WHITEGabriel's Saferoom$OFF");
  set_long(
  "You've stumbled in to the saferoom of Gabriel, the murderous drug lord of \n"
  +"Salusa. The walls are adorned with the heads of various rare creatures,\n"
  +"as well as priceless artifacts undoubtedly plundered from somewhere on \n"
  +"Arrakis. There's piles of solaris in every direction and you get the feeling\n"
  +"Gabriel doesn't want you around...");
  
  add_exit("south", ROOM_DIR +"vip4.c");
}