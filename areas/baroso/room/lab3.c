/*
** Lab 3 - Marine Project
** by Kiza 17-Jul-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg) {
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/scientist.c", 2);
  if(arg) return;

  set_light(1);
  short_desc = "Atreides Lab 3";
  set_long(
  "The first thing you notice about this lab is huge tanks full of marine\n"+
  "creatures, which you recognize as being not only from Caladan's oceans,\n"+
  "but also the oceans of other worlds, there are even creatures you can't\n"+
  "identify.  There is a scientist here sitting in front of a large tank,\n"+
  "constantly taking notes.\n\n");


  add_item("tank",
  "Large clear walled tanks are scattered around the room, filled\n"+
  "with all types of sea creatures, native to Caladan and other local\n"+
  "planets in this system.  There are even some creatures you can not\n"+
  "identify, you assume they must be some cross-breeding experiment.");
  add_item("tanks","=tank");
  add_item("creature",
  "You examine the weird and wonderful creatures contained\n"+
  "within these tanks, marveling at the wonder of life.\n"+
  "Some of these creatures you cannot identify, and they\n"+
  "look incredibly alien.");
  add_item("creatures","=creature");

  add_exit("south","/d/Atreid/kiza/ubase/hall6.c");

}


