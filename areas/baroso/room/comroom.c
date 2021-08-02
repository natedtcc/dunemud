/* 
** Commander's Room
** by Kiza 24-Aug-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg) {
  ::reset(arg);
  if(!present("commander",this_object())) {
    transfer(clone_object("/d/Atreid/kiza/ubase/npc/commander.c"),this_object());
  }
  if(arg) return;
  
  set_light(1);
  set_short("Baroso Commander's Room");
  set_long( 
  "This room is larger than the other room, and is used as Baroso's Base\n"+
  "Commanders room.  The room is finely decorated, with a small desk in one\n"+
  "corner, with a bed next to it.  Above the desk, which is full of reports,\n"+
  "is a book shelf filled with all sorts of fine reading literature.\n\n");

  add_exit("west","/d/Atreid/kiza/ubase/ghall4.c");
 
  
  add_item("desk",
  "This is a finely crafted work desk, used by the base commander, has neatly\n"+
  "stacked papers and books in each corner and looks well used by the "+
  "commander.");
  add_item("bed",
  "This is a small, but extremely comfortable looking bed which has been "+
  "neatly\nmade, the covers have a small hawk emblem in the corner, "+
  "signifying they\nbelong to house Atreides.");
  add_item("shelf",
  "On the shelves are many fine books which have been published over "+
  "the years.");
  add_item("shelves","=shelf");
  add_item("books","=shelf");
  add_item("book shelf","=shelf");
  
}