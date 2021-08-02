/* 
** Guard Post 1
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
"As you enter this room, you notice a multitude of screens and video displays\n"+
"with images of rooms and hallways in Baroso, and you realise that this is one\n"+
"of Baroso's guard posts, monitoring everything which goes on in Baroso.\n"+
"The walls have a finely crafted green covering over them, with a hawk emblem\n"+
"on the covering located on the far wall, the room itself is rather large,\n"+
"with enough room to fit more than just the current monitor bays and workdesk\n"+
"currently located in here.\n");


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
