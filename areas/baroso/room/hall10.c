/* Atreidies Underwater Base - Hall 10 */
/* by Kiza 10-Jul-1996 */
/* 28-Aug-1996 - Added Guard Trapping */
/* cleaned up / upgraded by Bebop July 2018 */

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

#include "/d/Atreid/guild/include/defines.h"
#define TPQN TP->QN

int count;
object mon;
string pname;

void reset(int arg){
  ::reset(arg);
  
  if(!present("guard",this_object())) {
    mon = clone_object("/d/Atreid/kiza/ubase/npc/vet_guard.c");
    move_object(mon,this_object());
  }
  
  if(arg) return;
  
  set_light(1);
  set_short("Baroso Hallway");
  set_long(
  "You are in one of the many hallways of Baroso\n\n"+
  "The hallway looks very new, with new plasteel walls, with the Hawk emblem \n"+
  "and paintings covering the walls.\n\n"+
  "The guard quarters to the east is protected by a guard.  BEWARE this guard\n"+
  "gets angry easily, also off to your west is the food gardens.\n\n");

  count = 0;
  add_exit("south","/d/Atreid/kiza/ubase/hall9.c");
  add_exit("north","/d/Atreid/kiza/ubase/hall11.c");
  add_exit("east","/d/Atreid/kiza/ubase/ghall1.c");
  add_exit("west","/d/Atreid/kiza/ubase/foodhall.c");
  add_item("emblem",
  "The hawks head, the recognized emblem of House Atreides.");
  add_item("paintings",
  "The portraits of many great Atreides fighters of\n"+
  "history, which line the walls in an attempt to hide the\n"+
  "cold plasteel walls they hang upon.");
  add_item("pictures","=paintings");
  add_item("picture","=paintings");
  add_item("painting","=paintings");
  add_item("plasteel",
  "This is the compound building material for structures, it\n"+
  "is light but extremely strong, and has been used for the\n"+
  "walls and supports for all of this base.");
}

init() {
::init();
 
  add_action("trap","e");
  add_action("trap","east");
}

int trap() {
  if(present("guard")) {
    if(count < 5) {
      say(
       "\nThe Guard says to "+TPQN+":\n"+
       "\"Where do you think you're going, this is a "+
       RED+"RESTRICTED AREA"+OFF+"\".\n\n"+
       "Leave immediately, before I teach you a lesson you do not wish "+
	   "to learn.\"\n");
      write(
       "The Guard says to you:\n"+
       "\"Where do you think you're going, this is a "+
       RED+"RESTRICTED AREA"+OFF+"\".\n\n"+
       "Leave immediately before I teach you a lesson you do not wish "+
	   "to learn.\"\n");

      if(pname==TPQN) {
        count++;
        pname = TPQN;
      }
      else {
        pname = TPQN;
        count=0;
      }
    }
    else {
      write(
       "The Guard pushes you and says: \"I warned you, you just don't learn "+
	   "do you?\".\n");
      say(
       "The Guard pushes "+TPQN+" and says: \"I warned you, you just don't "+
	   "learn do you?\".\n");
      mon->attack_object(this_player());
    }
  return 1;
  }
}
