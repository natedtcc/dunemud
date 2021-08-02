/*
** Atreides Lab 5 - Unused Lab
** by Kiza 21-Jul-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg){
  ::reset(arg);
  if(arg) return;
  
  set_light(0);
  set_short("Unused Atreides Lab");
  set_long(
  "Around you are many unused benches and testing equipment which were used\n"+
  "on projects long since finished, you hear a low humming sound of the air\n"+
  "pumps keeping a constant flow of oxygen throughout the base.\n\n"+
  "You see little of importance here, except for the exit to the south back\n"+
  "to the Hallway.\n\n");

  
  add_item("equipment",
  "It looks as though the equipment here, which consists of\n"+
  "hammers, screwdrivers, screws and other numerous tools, have\n"+
  "been in their racks here for a while, obviously this lab\n"+
  "outlived it's usefulness and has been closed until a newer\n"+
  "project comes alone.");
  add_item("pumps",
  "These look nothing more than air grills, you can hear the pumps\n"+
  "but not see them.");
  add_item("sound",
  "A low humming sound, coming from behind the air grills.");
  add_item("pump","=pumps");
  add_item("bench",
  "A wooden bench used for constructing items and equipment.\n"+
  "It is no longer in use, as is the rest of the lab.");
  add_item("benches","=bench");

  add_exit("south","/d/Atreid/kiza/ubase/hall7.c");
  
}

