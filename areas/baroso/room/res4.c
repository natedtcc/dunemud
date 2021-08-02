/* Residental Area Junction */
/* by Kiza 11-Jul-1996 */
/* cleaned up / upgraded by Bebop July 2018 */

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg){
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/mguard.c", 2);
  if(arg) return;
    
  set_light(1);
  set_short("Lab Worker Residential Rooms");
  set_long( 
  "This is the entrance to the workers quarters, where they come to relax\n"+
  "after their shifts are complete.\n\n");

  add_exit("north","/d/Atreid/kiza/ubase/res3.c");
  add_exit("south","/d/Atreid/kiza/ubase/res4a.c");
  add_exit("west","/d/Atreid/kiza/ubase/res4b.c");
  add_exit("east","/d/Atreid/kiza/ubase/hall4.c");

  add_item("entrance",
  "There are multiple entrances leading off from this room\n"+
  "to private quarters of Atreides Technicians.");
}               

