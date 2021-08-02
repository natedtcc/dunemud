// fremen.c - Fremen Sietch multimob
// Original file by Juris, updated by Bebop 2021

inherit "/obj/player/monster";
#include "/d/Fremen/bebop/sietch/include/defines.h"
object item, armour, weapon, boots;
int level_variance = random(100);

void reset (int arg) {
  ::reset (arg);
  if(!arg){
    set_name("fremen");
    set_level(BASE_MOB_LEVEL + level_variance);
    improve_skill("Roll", 1750);
    improve_skill("Dodge", 1750);
    improve_skill("Block", 1750);
    improve_skill("Crysknife", 1750);
    improve_skill("Knives and Daggers", 1750);
    improve_skill("Hand to Hand", 1750);
    add_money(1000 + random(700));
    set_race("fremen");

    switch(random(7)) {
      case 0 :
        set_alias("kid");
        set_short("Fremen kid");
        set_long("Young fremen.\n");
        set_gender(1);
        break;
      case 1 :
        set_alias("girl");
        set_short("Fremen girl");
        set_long("Nice fremen girl.\n");
        item = clone_object("/d/Fremen/bebop/sietch/obj/doll.c");
        set_gender(2);
        break;
      case 2 :
        set_alias("woman");
        set_short("Fremen woman");
        set_long("Old and very serious fremen woman.\n");
        set_gender(2);
        break;
      case 3 :
        set_alias("naib");
        set_short("Fremen naib");
        set_long("Very calm fremen naib.\n");
        set_gender(1);
        break;
      case 4 :
        set_alias("warrior");
        set_short("Fremen warrior");
        set_long("Very serious and well trained fremen guard.\n");
        set_gender(1);
        break;
      case 5 :
        set_alias("man");
        set_short("Old Fremen man");
        set_long("Very old man. Don't underestimate him.\n");
        item = clone_object("/d/Fremen/bebop/sietch/obj/pearl.c");
        set_gender(1);
        break;
      case 6 :
        set_alias("guard");
        set_short("Fremen guard");
        set_long("Well trained fremen guard.\n");
        set_gender(1);
        break;
      case 7 :
        set_alias("boy");
        set_short("Fremen boy");
        set_long("Small fremen boy.\n");
        item = clone_object("/d/Fremen/bebop/sietch/obj/pellet.c");
        set_gender(1);
        break;
      case 8 :
        set_short("Young fremen");
        set_long("Very young fremen with sea blue eyes.\n");
        set_gender(1);
        break;
    }
    set_dead_ob(this_object());
    armour = clone_object("/d/Fremen/bebop/sietch/obj/suit.c");
    weapon = clone_object("/d/Fremen/bebop/sietch/obj/crys.c");
    boots = clone_object("/d/Fremen/bebop/sietch/obj/dboots.c");
    
    // Set AC for all possible armor slots (0-14)

    for (int i=0; i<15; i++){
      set_ac(800+random(60), i);
    }
  }
  if (item) move_object(item, this_object());
  if (armour) move_object(armour, this_object());
  if (weapon) move_object(weapon, this_object());
  if (boots) move_object(boots, this_object());

  if (armour || weapon || boots) command("powerup");
  
}

monster_died() {
  switch(random(6)){
    case 0 : tell_object(this_player(),
    "You see a mist rise from the dead body.\n"); break;
    case 1 : tell_object(this_player(),
    "You see an illusion telling you that this death will be avenged.\n"); break;
    case 2 : tell_object(this_player(),
    "Dying fremen whispers to you : 'You are going to pay for this.'\n"); break;
    case 3 : tell_object(this_player(),
    "Dying fremen screams in agony : 'Long live the fighters.'\n"); break;
    case 4 : tell_object(this_player(),
    "Dying fremen says : 'Death is just the beginning.'\n");break;
    case 5 : break;
  }

}
