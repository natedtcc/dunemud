/* Atreidies Underwater Research Base */
/* by Kiza 10-Jul-1996 */

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

#include "ansi.h"

object mon;

void reset (int arg){
  ::reset(arg);
  if (!present("board")) {
    object board = clone_object("/obj/board.c");
    board->set_name(BOLD + PINK + "Baroso research" + OFF);
    board->set_save_file("/d/Atreid/kiza/ubase/barosoboard");
    board->set_article("The");
    board->set_remove_level(100);
    board->secure();
    move_object(board, this_object());
  }
  
  if (!present("warning", this_object())) {
     transfer(clone_object("/d/Atreid/kiza/ubase/obj/warning.c"),this_object());
  }
  
  if (!present("baroso map", this_object())) {
     transfer(clone_object("/d/Atreid/kiza/ubase/obj/map.c"),this_object());
  }

  if (!present("guard", this_object())) {
      transfer(clone_object("/d/Atreid/kiza/ubase/npc/mguard.c"),this_object());
  }
  if (arg) return;
  set_light(1);
  set_short("Boroso Docking Port");
  set_long(
  "As you step from your hydropod, a small spritly man runs upto you and \n"+
  "says to you \"Welcome to "+GREEN+"Baroso"+OFF+", our secret research"+
  " facility, I hope you\nfind everything to your liking and that your work is productive\".\n"+
  "You glance around this area, noting that everything looks brand new\n"+
  "give or take a few months wear and tear.\n\n"+
  "You also notice some hydropods sitting ready for departure, with one\n"+
  "departing every few minutes with a single passenger onboard.\n\n"+
  "To your southeast is the entrance to the main hallway around Baroso.\n\n");
  
  add_item("baroso",
  "The entrance to Baroso looks to be rather new with "+
  "plasteel portals looking\nout into the seas of caladan. "+  
  "Supporting the roof of the base is a\nmultitude of girders.\n");
  add_item("man",
  "A small, but overly active man who seems to be briefing people\n"+
  "to Baroso as they step out of their hydropods.\n");
  add_item("dock",
  "A relitivly large dock, it can easily accomodate 5 hydropods and\n"+
  "about 50 people quite easily\n");
  add_item("hydropod",
  "A small white bullet shapped transport vehicle sitting on\n"+
  "a launch rail upon which it fires itself towards launch speed.\n");
  add_item("pod","=hydropod");
  add_item("pods","=hydropod");
  add_item("hydropods","=hydropod");

  add_exit("southeast","/d/Atreid/kiza/ubase/hall.c");
  add_exit("hydropod","/d/Atreid/kiza/ubase/obj/return_podloader.c");
 
  

}
