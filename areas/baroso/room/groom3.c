/* 
** Guard Room 3
** by Kiza 24-Aug-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg) {
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/vet_guard.c", 3);
  if(arg) return;
  
  set_light(1);
  set_short("Baroso Guard's Room");
  set_long( 
  "This room is rather small, and is used to house the Baroso Marine Guards\n"+
  "the main security for Baroso.  The room is finely decorated, with a small\n"+
  "desk in one corner, with a bed next to it.  Above the desk is a book shelf\n"+
  "filled with all sorts of fine reading literature.\n\n");

  add_exit("south","/d/Atreid/kiza/ubase/ghall2.c");
 
  add_item("desk",
  "This is a finely crafted work desk, used by the guard who resides in this\n"+
  "room, it has neatly stacked papers and books in each corner and looks "+
  "well\nused by the rooms resident.");
  add_item("bed",
  "This is a small, but extremely comfortable looking bed which has been "+
  "neatly\nmade, the covers have a small hawk emblem in the corner, signifying"+
  " they\nbelong to house Atreides.");
  add_item("shelf",
  "On the shelves are many fine books which have been published over the "+
  "years.");
  add_item("shelves","=shelf");
  add_item("books","=shelf");
  add_item("book shelf","=shelf");
  
}


