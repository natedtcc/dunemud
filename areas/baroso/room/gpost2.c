/* 
** Guard Post 2
** by Kiza 1-Aug-1996
** cleaned up / upgraded by Bebop 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg) {
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/vet_guard.c", 4);
  if(arg) return;
  
  set_light(1);
  set_short("Baroso Guard Post");
  set_long(
"You enter this adequatly sized room, it's walls lined with coverings, with\n"+
"a hawk emblem on the far covering, you also notice a few screens and video\n"+
"monitors constantly flicking between images.  Near the screens is a small\n"+
"workdesk which is used by the guard on duty.  It occurs to you that this is\n"+
"a secondary guard post, used to keep a closer eye on the far side of Baroso\n");

  add_item("coverings",
"These are crafted of a fine material, and are hung on the walls to not only\n"+
"decorate the room, but to hide the bland colour of the walls behind them.");
  add_item("workdesk",
"This desk is used by the guards on duty to prepare and complete paperwork\n"+
"required for their shift.");
  add_item("screens",
"There are a numerous images on these screens, from almost every room in\n"+
"Baroso, you see people walking through the hallways, working in the labs\n"+
"relaxing in their rooms, and doing many other activities through these\n"+
"screens.");
  add_item("monitors","=screens");

  add_exit("west","/d/Atreid/kiza/ubase/hall1.c");

}
