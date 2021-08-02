/* Chef, carries Unique Cleaver of Good Heath and Strength
                 Updated by Bebop, July 2021 */

inherit "/obj/player/monster";

#define UNIQUED "/secure/daemons/uniqued"

//#include "gen.h"
#include <mudlib.h>

object weapon;
object armour;

reset(arg) {
  ::reset(arg);
  if(!arg){
    set_level(80+random(21));
    //  set_hp(level*100+random(level*500));
    // set_hp() should be not used in npc code
    // should be left to be set be set_level()
    // -Talon 05.040.2002
    adjust_str(10+random(5));
    adjust_wis(random(5));

    set_name("chef");
    set_race("Tleilax");
    set_alias("chef");
    set_short("Mean-looking chef");
    set_long("\
      Perhaps the buffest looking chef you've\n\
      ever seen, this one has muscles that\n\
      make watermelons look like mere seeds.\n\
      You should seriously reconsider attacking\n\
      this formidable giant.\n");
    set_al(0);
    set_gender(1);
    add_money(1000);

    // Added a couple few chats - Bebop 2021
    load_chat(10, ({
    "The chef cries out in aggravation: I need some spice!\n",
    "The chef mumbles something like: If I catch that person who's giving the bad spice...\n",
    "The chef goes: Now you will have to die!\n",
    "The chef goes: I will kill you easily!\n",
    "The chef says: Just 'say I have the spice' if you find it...\n",
    "The chef whispers: I've heard you can find the good stuff in the tower outside..\n",
    }));

    set_chance(30);
    set_spell_dam(100);
    set_spell_mess1("The chef fries his opponent's body!\n");
    set_spell_mess2("The chef fries your body!\n");
    improve_skill("Roll", random(level));
    improve_skill("Axes and Maces", random(level));
  }
  
  weapon = clone_object("/d/Tleilax/fate/obj/wcleaver.c");
  armour = clone_object("/d/Tleilax/fate/obj/apron.c");
  
  if (weapon) move_object(weapon, this_object());
  if (armor) move_object(armor, this_object());

  if(armour || weapon)
    command("powerup");
}

// Quest completion logic - updated by Bebop 2021
catch_tell(str) {
  string name;
  object spice;

  if(scanf(lower_case(str, "%s says: i have the spice.", name) == 1)) {
    spice = present("liquid spice", this_player());
    if(!spice) {
      write("Chef says: You liar! I will kill you for giving me the wrong spice!\n");
      this_player()->attacked_by(this_object());
      attack_object(this_player());
      return 1;
    }

    else {

    // Ridding of this in favor of querying a quest - Bebop 2021
    //
    // call_other("/d/Tleilax/fate/obj/quest_ob", "???");
    // if("/d/Tleilax/fate/obj/quest_ob"->query_names(this_player()->QRN+"_spice")) {
    //   write("Chef says: You've already done it!\n");
    //   return 1;
    // }
    // Desting quest item instead of leaving it on the mob - Bebop 2021
    //
    // if(present("liquid gin", this_object())) {
    //   write("Chef says: 'Thanks, but no thanks. You're a tad late.\n");
    //   return 1;
    // }

      if (!this_player()->query_quests("tainted_spice")){
        write("Chef says: 'Thank you! Now you will be rewarded\n"+
            "Chef takes the spice from you, beaming at you.\n"+
            "Chef looks at you and says: 'Great Job!'\n"+
            "Chef hands you his apron and cleaver and teaches how to use them.\n"+
            "You feel better at Axes and Maces.\n"+
            "You feel more experienced.\n");
        "/d/Tleilax/fate/obj/quest_ob"->add_name(this_player()->QRN+"_spice");
        this_player()->improve_skill("Axes and Maces", 75);
        this_player()->add_exp(500000);
        this_player()->add_money(500000);
        if(weapon) {
          command("unwield "+weapon->query_name());
          move_object(weapon, this_player());
        }
        if(armour)
          move_object(armour, this_player());
        destruct(spice);
        this_player()->add_explorer_flag(object_name(environment(this_object())));
      }

      else {
        write("Chef says: You've done this already!\nHe takes your spice anyway.\n");
        destruct(spice);
      
      }
    }
  }
}
