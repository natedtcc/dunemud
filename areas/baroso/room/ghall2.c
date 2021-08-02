/* 
** Guard Hall 2
** by Kiza 28-Jul-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg){
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/vet_guard.c", 3);
  if(arg)return;
  
  set_light(1);
  set_short("Baroso Guard Quarters Hallway");
  set_long( 
  "This is a hallway in the Guard's Quarters, it runs east to west of you\n"+
  "with rooms off to the north and south.\n"+
  "Around you you can see more pictures adorning the walls, with wall \n"+
  "coverings behind them, trying to hide the sterile look of the hallway\n"+
  "walls which peak out from behind the pictures.\n\n");

  add_item("pictures",
  "Along the walls are pictures of many a great Atreides dukes through the\n"+
  "ages, it is obvious the guards in these quarters take pride in their\n"+
  "house.");
  add_item("picture","=pictures");
  add_item("walls",
  "The walls are made of mainly plasteel, and look rather new and sterile,\n"+
  "obviously the paintings and wall coverings were trying to hide this.");
  add_item("coverings",
  "These wall coverings are used to disguise the sterile look of the plasteel\n"+
  "walls they hang upon, and are finely crafted in a material which you have\n"+
  "never seen before, but which shimmers a little, and is incredibly soft and\n"+
  "smooth to the touch.");
  add_item("trolley"
  "This is a well designed cleaning trolley, it contains everything needed "+
  "for\na cleaner to carry out his job of keeping Baroso clean.");

  add_exit("north","/d/Atreid/kiza/ubase/groom3.c");
  add_exit("south","/d/Atreid/kiza/ubase/groom4.c");
  add_exit("west","/d/Atreid/kiza/ubase/ghall1.c");
  add_exit("east","/d/Atreid/kiza/ubase/ghall3.c");
  
}
 
init() {
::init();

  add_action("trap","s");
  add_action("trap","south");
}


trap() {
  write(
  "A cleaner passes you as you enter the room, obviously returning to their\n"+
  "trolley to continue cleaning the guard quarters.\n\n");
}

