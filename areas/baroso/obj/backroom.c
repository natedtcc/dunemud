
/* Comic Store Backroom - Secret HydroPod station */
/* by Kiza - 4-Jan-1999 */
/* explorer points added by Bebop July 2018 */
 

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg) {

  ::reset(arg);
  if(arg)return;

  set_light(1);
  set_short("Comic store back room.(UNDER CONSTRUCTION!)");
  set_long(
"This is a rather small backroom to Old Murphys comic store, around you\n"+
"is a variety of boxes of different sizes.\n"+
"\nThe western wall has been moved aside to reveal a single person transport\n"+
"pod.\n\nTHIS AREA IS CURRENTLY CLOSED!\n\n");

  add_item("boxes",
  "A variety of varying sized boxes, obviously used to ship the\n"+
  "comics to Old Muprh.");
  add_item("box","=boxes");
  add_item("pod",
  "A single man transport capsule, mounted on a launching rail.\n"+
  "The pod is on a slight angle, and you figure that it will\n"+
  "take you under the streets of Caladan.");

  add_exit("door","/d/Atreid/kiza/ubase/bentry");
  //add_exit("pod","/d/Atreid/kiza/ubase/obj/podloader");


}
