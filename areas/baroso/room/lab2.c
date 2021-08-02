/* 
** Lab 2 - Food Tech Project
** by Kiza 21-Jul-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

#include "/d/Atreid/guild/include/defines.h"

int found_sarish;
int found_radine;

void reset(int arg){
  ::reset(arg);
  place_objects("/d/Atreid/kiza/ubase/npc/advanced_scientist.c", 3);
  if(arg) return;

  set_light(1);
  set_short("Atreides Food Tech Lab");
  set_long(
  "The aroma of cooking food assaults your nostrils as you walk into this lab.\n"
  "This is the Food Tech Lab, and there are a few scientists performing tests\n"
  "on prepared foods, and also preparing new dishes to keep the forces of \n"
  "House Atreides fit and healthy.\n\n");
  
  add_exit("north","/d/Atreid/kiza/ubase/hall5.c");
  
  
  add_item("food",
  "Unusual, but tasty looking food items in a variety of colors.");
  add_item("lab",
  "A clean white room, filled with equipment to prepare and test\n"+
  "food eventually to be used by the Atreides troops and citizens\n"+
  "too keep the troops and population healthy.");
  add_item("scientist",
  "Both of these scientists are dressed in white lab coats, \n"+
  "and seem extremely busy rushing between items of equipment\n"+
  "obviously preparing and testing food at the same time");
  add_item("scientists","=scientist");

}

init() {
::init();

  add_action("search","search");
}

search() {
  int rand_num;
 
  rand_num = random(3);

  if(!present("scientist",TO)) {
    if((rand_num == 0 && found_sarish == 0)
       && !(present("radine",TP) && present("sarish",TP))) {
         write(
"Rummaging through the food scraps on the benches you find a delicious\n"+
"looking meal with a label attached reading 'Sarish'.\n");
         transfer(clone_object("/d/Atreid/kiza/ubase/obj/sarish.c"),TP);
         found_sarish = 1;
       }
       else if((rand_num == 1 && found_radine == 0)
               && !(present("radine",TP) && present("sarish",TP))) {
         write(
"As you glance over the benches, you notice an leafy orange substance \n"+
"which has a label attached to it reading 'Spiced Radine'.\n");
         transfer(clone_object("/d/Atreid/kiza/ubase/obj/radine.c"),TP);
         found_radine = 1;
       }
       else {
         write("You find nothing special.\n");
       }
}
  return 1;
}

