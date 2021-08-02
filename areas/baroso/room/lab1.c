/*
** Lab 1 - Atreides Weapons Lab
** by Kiza 21-Jul-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg) {
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/mortain.c", 1);
  if(arg) return;
  
  set_light(1);
  
  set_short("Atreides Weapons Lab");
  set_long(
  "As you enter this room, you notice that there is bits of metal, and\n"+
  "electronics lying everywhere, this is definitely one lab which is not\n"+
  "kept as tidy, it's an utter mess.  On closer inspection of items that\n"+
  "are scattered around the floor and tables, you deduce that this is\n"+
  "a weapons research lab, where they try to improve new weapons, and\n"+
  "also create new weapons.\n\n");
 

  add_item("metal",
  "The metal lying around looks of no use to you, it could probably be made\n"+
  "into something useful at the hands of a trained weapon master.");
  add_item("mess",
  "It's a mess, a mess of metal, electronics and other materials.  Plain and\n"+
  "simple, it makes your bedroom look tidy!");
  add_item("floor",
  "You can't see the floor through this mess, but you know there is one "+
  "because\nyou feel it under your feet.");
  add_item("tables",
  "The tables are nothing fancy, plain wooden work tables with a vice on each\n"+
  "of them.");
  add_item("vice",
  "A steel vice there is nothing you can think to use it with.");



 
  add_exit("south","/d/Atreid/kiza/ubase/hall5.c");
 
}

