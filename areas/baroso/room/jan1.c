/* 
** Janitors Closet
** by Kiza 3-Aug-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

#include <mudlib.h>
 
#define CTPQRN TP->QN

int searched;

void reset(int arg) {
  ::reset(arg);
  searched = 0;
  if(arg) return;
  
  set_light(0);
  set_short("Janitor's Closet");
  set_long(
 "This room is a complete mess, with cleaning implements all over the place,\n"+
 "some carefully balanced on top of other things, the rest are strew over the\n"+
  "strewn across the floor... be careful in here, the slightest wrong move\n"+
  "could bring everything crashing down, alerting guards to your presence in\n"+
  "here, so you must be very quiet.\n");

  add_exit("west","/d/Atreid/kiza/ubase/hall3.c");
  
  
  add_item("cleaning tools",
  "Numerous cleaning tools, lie scattered around the floor\n"+
  "and a whole heap of them stacked high, very tediously\n"+
  "balanced, ready to tumble at any stage.\n\n");
  add_item("cleaning","=cleaning tools");
  add_item("tools","=cleaning tools");
  
}

init(arg) {
  ::init(arg);

  if(arg)return;
  add_action("search","search");
}

int search() 
{
object mon, mon2;
if(!present("guard",this_object())&&searched == 0) {
    searched = 1;
    mon = clone_object("/d/Atreid/kiza/ubase/npc/vet_guard.c");
    mon2 = clone_object("/d/Atreid/kiza/ubase/npc/mguard.c");
    move_object(mon2,this_object());
    move_object(mon,this_object());
    write(
"As you start to search, you knock over a whole pile of equipment, creating\n"+
"an enormous crash as everything comes tumbling down around you.  As the \n"+
"dust settles, you hear footsteps coming towards the room, and two guards\n"+
"walk through the door.\n");
    say(
CTPQRN+" knocks over some equipment, causing everything to crash down around\n"+
"you, as the dust settles you hear footsteps walking towards this room, and\n"+
"two guards burst through the door and confront "+CTPQRN+"\n");
    write(
"The Guard says to you: Hey, what are you doing in here, leave here\n"+
"immediately.\n");
    say(
"The Guard says to "+CTPQRN+": Hey, what are you doing in here, leave here\n"+
"immediately.\n");
    mon->attack_object(this_player());
  }
  else {  
    write("You search through the room and find nothing of interest.\n");
  }
  return 1;
}  