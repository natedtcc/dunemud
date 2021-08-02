/*
** Lab 4 - Atreides Armour Lab
** by Kiza 21-Jul-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg) {
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/kalneric.c", 1);
  if(arg) return;
 
  set_light(1);
  set_short("Atreides Armour Lab");
  set_long(
  "Upon entering this Lab, you see many materials lying around, most are a\n"+
  "an extra strong material to provide tough but light armour for the field\n"+
  "operatives of House Atreides.  You also see lots of electronic parts\n"+
  "strewn across tables, and also other materials which go into making alot\n"+
  "of the house armours.\n\n");

  
  add_item("material",
  "There are piles of metallic materials located around the room,\n"+
  "you can only assume that these will wind up making armour\n"+
  "for the legions of the Duke.  There are also piles of white\n"+
  "materials, which you cannot identify.");
  add_item("armour",
  "There are many pieces of armour lying around, either complete\n"+
  "or in the process of being made.  Unfortunately you do not\n"+
  "think Kalneric would appreciate you taking any of his armour.\n");
  add_item("electronics",
  "These look to be rather delicate but highly advanced electronic\n"+
  "circuit boards, which could be used for some form of 'smart'\n"+
  "armour.");
  add_item("materials","=material");
  add_item("armours","=armour");
  add_item("electronic","=electronics");
  
  add_exit("north","/d/Atreid/kiza/ubase/hall6.c");

  return;
}     

