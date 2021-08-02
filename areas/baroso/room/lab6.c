/* 
** Lab 6 - Anti-Biologics Project
** by Kiza 18-Jul-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg){
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/scientist.c", 2);
  if(arg) return;
  
  set_light(1);
  set_short("Anti-Biologics Lab");
  set_long(
  "This lab is split into two sections, a decontamination room, which you are\n"
  "presently standing which connects the entrance to the research room where\n"
  "contained experiments are performed, because it is all contained for\n"
  "safety's sake, you are unable to progress any further into this room.\n"
  "Through a observation window you can see two scientists wearing plastic\n"
  "suits in the research room busily performing tests and experiments.\n"
  "The main purpose for this lab is to conduct research into reversing the\n"
  "effects of Biological Weapons, which are commonly called Biologics.\n\n"
  "To your north is the main hallway of Baroso.\n\n");

  add_exit("north","/d/Atreid/kiza/ubase/hall7.c");

  add_item("scientist",
  "Two scientists wearing plastic contamination suits are busily working\n"+
  "on an experiment. You are unable to clearly make out exactly what they\n"+
  "are doing.");
  add_item("scientists","=scientist");
  add_item("window",
  "This window is all that protects you from being affected by the deadly\n"+
  "chemicals which the scientists are working on.");
  add_item("screen","=window");

}

