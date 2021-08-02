
#include <mudlib.h>
inherit MONSTER;
#include "/d/Fremen/bebop/sietch/include/defines.h"
object suit;
void reset (int arg) {
  ::reset(arg);
  if (!arg){
    set_level(40+random(10));
    set_hp(level*2000+random(level*150));
    improve_skill("Roll", level);
    set_name("fremen");
    set_short("Wise Fremen");
    set_race("fremen");
    set_long("\
  You see an old man, with very poor clothing,\n\
  as you notice the blue color in the eyes of\n\
  this man you understand that he's probably a\n\
  fremen. Scars on his body tell you that this\n\
  man must be experienced in many battles and\n\
  very wise. Perhaps he can teach you something.\n\
  Why not try to 'listen' to him?\n");
  }
}
void init() {
  ::init();
  if(!interactive(this_player())) return 1;
  tell_object(this_player(),"Fremen greets you as you enter the room.\n");
  load_chat(6, ({
   "Fremen worships Shai Hulud.\n",
   "Fremen whispers something.\n",
   "\
Fremen says : We have heard that the Harkonnen have \n\
our crysknife on their home planet. Find it and bring it back to us ! \n",
   "Fremen says : We had nearly located the crysknife when they moved it somewhere !\n",
   "Fremen says : We need your help warrior !\n",
   "Fremen says : We can't let them go with this !\n",
   "Fremen asks : Will you help us ?\n",
   "Fremen says : There exists a limit to the force, even the most powerful may apply.\n",
   "Fremen says : If you find the crysknife, just come here and 'say I have the crysknife'\n",
}));
  if(present("crysknife", this_object()))
   destruct(present("crysknife", this_object()));

  add_action("listen","listen");
}

int listen(string str){
  if (str == "to fremen" || str == "to wise"){
    write("Fremen thanks you and says: We need your help! Not long ago, this\n"
      +"sietch came under attack from the Harkonnen. Many Fremen young and old\n"
      +"lost their lives to that scum! During the mayhem, they pillaged a very\n"
      +"sacred and ancient crysknife from our sietch vault. This crysknife is\n"
      +"very important to us - and must be returned immediately!\n\n"
      +"The Wise Fremen clears his throat and says: We know it is hidden on their\n"
      +"home planet of Giedi Prime. Please, go there and find it! Once you do,\n"
      +"return to me and 'say I have the crysknife'.\n"
      +"Fremen bows to you.\n");

      return 1;
  }
  else return notify_fail("Listen to whom? Fremen maybe?\n");
}

int catch_tell(str) {
  string name;
  object ckf;
  if(sscanf(str, "%s says: I have the crysknife", name) ==1) {
   ckf = present_clone(OBJ_DIR + "quest_crys", this_player());
   if(!ckf) {
    write("Fremen says : No you don't !\n");
    return 1;
   }
  if(ckf->query_finder() != this_player()->query_real_name()) {
   write("Fremen asks : Where did you get this knife ?\n");
   write("Fremen says : You didn't found it yourself!\n");
   write(ckf->query_finder());
   transfer(ckf, "/room/null");
   destruct(ckf);
   return 1;
  }
 if(present("crysknife", this_object()))
    if(this_player()->query_level()<10){
    write("You are too young to finish this quest.\n");
    return 1;
  }

  /* Updated this condition to check if the quest is done
  rather than checking if it can be quest_set()  
  - Bebop 2021 */

  if(!this_player()->query_quests("stolen_knife")) {
    write("Fremen says: Finally someone has found it ! Thank you.\n"+
         "Fremen gives you some solaris and teaches you a bit.\n"+
         "He also gives you a very old still-suit.\n\n"+
         "You feel better at Sand Walking.\n"+
         "You feel more experienced.\n");

    /* This used to just improve_skill(), so I added
    this condition to make sure the player currently 
    has Sand Walking (otherwise improve does nothing) 
    - Bebop 2021 */

    if (this_player()->query_skills("Sand Walking") == -1){
      this_player()->learn_new_skill("Sand Walking");
      this_player()->improve_skill("Sand Walking", 24);
    }

    else this_player()->improve_skill("Sand Walking", 25);
   
    this_player()->add_exp(500000);
    this_player()->add_money(500000);

    // Added an explorer flag, obviously

    this_player()->add_explorer_flag (object_name(environment(this_object())));
    this_player()->set_quest("stolen_knife");
    transfer(ckf, "/room/void");
    destruct(ckf);
    suit = clone_object(OBJ_DIR + "quest_suit");
    move_object(suit, this_player());
    return 1;
   }
   else {
    write("Fremen says: You have helped us already!\n");
    
    /* Adding this explorer flag here allows players
       who've already completed the quest to get a
       retroactive explorer point. - Baldu- I mean, Bebop :) */

    this_player()->add_explorer_flag (object_name(environment(this_object())));
    return 1;
  }
 }
}
