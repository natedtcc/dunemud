#include <mudlib.h>
inherit ROOM;

void reset(int arg) {
  

  ::reset(arg);
  
  place_objects("/players/bebop/npc/bigboy.c", 1);
  
  if(arg)
  return;
  set_light(1);
  set_short("$BOLD$WHITEGabriel's Saferoom$OFF");
  set_long(
  "You've stumbled into the hidden saferoom of Gabriel, the drug lord of \n"
  +"Salusa. The walls are adorned with the heads of various rare creatures,\n"
  +"as well as priceless artifacts undoubtedly plundered from somewhere on \n"
  +"Arrakis. There's piles of solaris in every direction and you get the feeling\n"
  +"Gabriel doesn't want you around...");
  
  add_exit("south","/players/bebop/room/sapho8.c");
}