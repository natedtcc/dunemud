/* 
** Food Gardens of Baroso
** by Kiza 4-Aug-1996
** cleaned up / upgraded by Bebop July 2018
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

#include <mudlib.h>

#define TOTALPICK 5 /* Max picks at a time per player */

int genguard();

int picked;
string name;
int guardatt;

void reset(int arg) {
  ::reset(arg);
  
  if(arg) return;
  
  guardatt = 0;
  picked = 0;
  set_light(1);
  
  short_desc = "Food Gardens of Baroso";
  set_long( 
  "You have entered the food gardens of Baroso, trays line the walls where\n"+
  "young seedlings grow. There is a great trellis with a multitude of vines\n"+
  "growing on it.  Each vine holding a different fruit, one with grapes,\n"+
  "another with big ripe yellow-orange fruit, creating a colourful pattern\n"+
  "along the trellis. Along the northen wall there are huge rows of hydroponic\n"+
  "bays with vegetable plants of every kind, enough to feed the base, and\n"+
  "suppliment food experiments conducted on Baroso. On the westen and eastern\n"+
  "walls there are massive jars of algae that bubble from time to time. There\n"+
  "is the ever present sound of water running in the background, the room is\n"+
  "lit with great white lights set into the ceiling so the whole room is bathed\n"+
"in pure white light with the green of the lush vegetation shinning through.\n\n"+
  "To the east entrance you see a guard watching over you carefully.\n\n");

  add_item("trellis",
"Intertwined on the trellis are long green vines, each bearing ripe purple\n"+
"grapes or large yellow-orange fruits, which look delicious in the artificial\n"+
"light shimmers against them.");
  add_item("grapes",
"These luscious purple grapes make your mouth water just by looking at them\n"+
"you wonder if you might be able to sneak a bunch without the guard noticing.");
  add_item("yellow fruit",
"These large round fruits look incredibly juicy, and delicious, you wonder\n"+
"if you might be able to pick a couple of these and take them with you.");
  add_item("yellow-orange fruit","=yellow fruit");
  add_item("orange fruit","=yellow fruit");
  add_item("large fruit","=yellow fruit");
  add_item("yellow-orange fruits","=yellow fruit");
  add_item("yellow fruits","=yellow fruit");
  add_item("orange fruits","=yellow fruit");
  add_item("large fruits","=yellow fruits");
  add_item("vines",
"These long vines are laden with huge bunches of grapes and large \n"+
"yellow-orange fruits which look just ripe for the picking.");
  add_item("jars",
"These huge plasteel jars contain huge quantities of water, were algae thrive\n"+
"as they help regulate the gas exchange in this room, there is two huge jars\n"+
"on the western and eastern walls.  From the top of each jar leads two huge\n"+
"filters which of which one leads back into this room, and the other out of\n"+
"room, possibly to the outside air, or to other rooms in the building.");
  add_item("algae",
"Through the clear plasteel walls of the jars, you can see tons of dark green\n"+
"algae thrive.");
  add_item("lights",
"You cover your eyes as you look up at the lights, which recreate the\n"+
"simulation of daylight, necessary for the plants growth this deep in the\n"+
"water.");
  add_item("light",
"An artificial sunlight is generated in this room by the long rows of lights\n"+
"along the ceiling, too bright to look at.");

  add_exit("west","/d/Atreid/kiza/ubase/hall2.c");
  add_exit("north","/d/Atreid/kiza/ubase/hall12.c");
  add_exit("east","/d/Atreid/kiza/ubase/hall10.c");
  
}

init() {
::init();

  add_action("pick","pick");
  add_action("search","search");
}

search() 
{
  write("You search around and find nothing of interest.\n");
say(TP->QN+" searches around the area, seemingly turning up nothing.\n");

  return 1;
}

pick(arg) {
  int randnum;
  object food;
  
  switch(arg) {
    case "fruit":
    case "fruits":
      if(picked < TOTALPICK && name != TP->query_real_name()) {
        write("You pick some fruit from the gardens.\n");
say(TP->QN+" picks some fruits from the garden.\n");
        food = clone_object("/obj/food");
        if((randnum = random(2)) == 1) {
          if((TP->add_weight(2))!=0) {
            food->set_name("chacko");
            food->set_short("A chacko");
            food->set_long("A large orange fruit which looks incredibly juicy.\n");
            food->set_strength(40);
            food->set_value(0);
            food->set_weight(2);
            food->set_eater_mess("You devour the soft flesh of this huge fruit.\n");
            food->set_eating_mess(" devours the soft flesh of a huge Chacko.\n");
            move_object(food,TP);
            name = TP->query_real_name();
            write("You select a large Chacko from the vines.\n");
            picked++;
          }
        }
        else if(randnum==0) {
          if((TP->add_weight(1))!=0) {
            food->set_name("grapes");
            food->set_short("A bunch of grapes");
            food->set_long("A lucious bunch of purple grapes.\n");
            food->set_strength(30);
            food->set_value(0);
            food->set_weight(1);
            food->set_eater_mess(
"You pop each suculent grape in your mouth, chewing each one carefully.\n");
            food->set_eating_mess(
" munches away on a bunch of purple grapes.\n");
            move_object(food,TP);
            name = TP->query_real_name();
            write("You select a bunch of grapes from the vines.\n");
            picked++;
          }
        }
        else {
          destruct(food);
        }
      }
      else {
        write(
"You are unable to pick anymore food from the gardens at the moment.\n");
        if(guardatt == 0)
          genguard();
      }
      break;
    case "vegetables":
    case "vegies":
    case "veg":
      if(picked < TOTALPICK && name != TP->query_real_name()) {
        write("You pick some vegetables from the dirt.\n");
say(TP->QN+" picks some vegetables from the dirt.\n");
        if((TP->add_weight(1))!=0) {
          food = clone_object("/obj/food");
          food->set_name("sarish");
          food->set_short("A small sarish");
          food->set_long(
"A small leafy vegetable, experimentally grown in the gardens of Baroso.\n");
          food->set_strength(45);
          food->set_value(0);
          food->set_weight(1);
          food->set_eater_mess(
"You tear away at the soft leaves of the sarish, each leaf filling you with "+
"a healthy vitality.\n");
          food->set_eating_mess(
" tears away at a small leafy vegetable which you can not identify.\n");
          move_object(food,TP);
          write("You carefully pluck a small sarish from the ground.\n");
          picked++;
        }
      }
      else {
        write(
"You are unable to pick anymore food from the gardens at the moment.\n");
        if(guardatt == 0)
          genguard();
      }
      break;
  }  
  return 1;
}

int genguard() {
  object mon,item,item;

  if(!present("guard",TO)&&guardatt==0) {
    write(
"An Atreides Marine Guard runs over to you accusing you of being a theif and\n"+
"attacks you.\n");
    say(
"An Atreides guard rushes over to "+TP->QN+", accuses "+TP->query_pronoun()+" of being a theif\n"+
"and attacks "+TP->query_pronoun()+".\n");
    mon = clone_object("/d/Atreid/kiza/ubase/npc/mguard.c");
    move_object(mon,TO);
    mon->attack_object(TP);
    guardatt = 1;
}
}
