// fremen.c - Fremen Sietch multimob
// Original file by Juris, defiled by Bebop 2021

inherit "/obj/player/monster";
#include "/d/Fremen/bebop/sietch/include/defines.h"

// Gear
object item, armour, weapon, boots;

// Combat related vars
int rage = 0;

object debug = find_player("bebop");

void reset (int arg) {
  set_heart_beat(0);
  ::reset (arg);
  if(!arg){
    set_name("fremen");
    set_level(BASE_MOB_LEVEL);
    
    // Make sure max hp has been achieved
    set_max_hp(HP_MAX);
    add_hp(HP_MAX - (query_hp()) );

    improve_skill("Roll", 1000);
    improve_skill("Dodge", 1000);
    improve_skill("Block", 1000);
    improve_skill("Crysknife", 900);
    add_money(2000 + random(700));
    set_race("fremen");
    set_no_tank(1);

    // Buncha BS props
    set_property ("no_backstab");
    set_property ("no_confuse");
    set_property ("no_enervate");
    set_property ("no_terror");
    set_property ("no_voice", 1);
    set_property ("no_finish", 1);
    set_property ("no_enslave", 1);
    set_property ("no_disarm", 1);
    set_property ("no_disrupt", 1);
    set_property ("no_blind", 1);
    set_property ("no_instakill", 1);
    set_property ("no_throw");

    // Set base attack bonus
    set_bonus_attack(BASE_BONUS);

    /* I get the idea behind doing this, but if you want
      to create a slew of different mobs from one obj -AND-
      have level variance between the mobs, this is not the
      way. */

    // Spawn a random form of Fremen sietch dweller 
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
        item = clone_object(OBJ_DIR + "doll.c");
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
        item = clone_object(OBJ_DIR + "pearl.c");
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
        item = clone_object(OBJ_DIR + "pellet.c");
        set_gender(1);
        break;
      case 8 :
        set_short("Young fremen");
        set_long("Very young fremen with sea blue eyes.\n");
        set_gender(1);
        break;
    }
    set_dead_ob(this_object());
    
    // Clone gear
    armour = clone_object(OBJ_DIR + "suit.c");
    weapon = clone_object(OBJ_DIR + "crys.c");
    boots = clone_object(OBJ_DIR + "dboots.c");
    
    // Set AC for all possible armor slots (0-14)

    for (int i=0; i<15; i++){
      set_ac(1000+random(60), i);
    }
  }

  // Place gear
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

void heart_beat ()
{
  ::heart_beat();

  /* These mobs will get progressively stronger - 
    either after 25 rounds of combat or if the mob's 
    hp falls below 75%, set_bonus_attack is bumped to
    180, and incremented by 2 every round after that.
    The maximum
  */

  if (query_attack() && !rage &&
    (query_fight_rounds() > 20 || query_hp() < to_int(HP_MAX * 0.75 ))){
      tell_object(query_attack(), 
        "The Fremen's eyes burn $BOLD$CYANblue$OFF as "
        +"they slip into a terrifying rage!");
      set_bonus_attack(POWERUP_BONUS);
      rage = 1;
  }

  // Increment bonus attack by 2 each round
  if (query_attack() && rage && (query_bonus_attack() < MAX_BONUS)){
    set_bonus_attack( query_bonus_attack() + 2 );
  }

  // Punch !!! - does 2.5-5% damage (1/3 chance)
  if (this_object () && !random (2) && query_attack()){
    load_object ("/obj/cmds/npc/general/punch.c")->punch(4000, 2000, 50, 1);
      tell_object(debug, "Punch hit\n");
  }

  // Thai kick !!! - same as the punch, but 1/4 chance
  // and also much cooler 8)
  if (this_object () && !random (3) && query_attack()){
    load_object ("/obj/cmds/npc/general/thaikick.c")->kick(4000, 2000, 50, 1);
    tell_object(debug, "Kick hit\n");
  }

  // Heal the mob if combat is paused (faster heartbeat completion)
  // Also decrement bonus attack until base has been reached 
  if (this_object () && !query_attack()){
    add_hp(HP_MAX / 100);
    if (query_bonus_attack() > POWERUP_BONUS)
      set_bonus_attack(query_bonus_attack() - 2);
    else { 
      set_bonus_attack(BASE_BONUS);
      rage = 0;
    }
  }

  // Deboog
  if (query_attack())
    tell_object(debug, "Enemy HP: " + query_hp() 
         +"\nPlayer HP: " + query_attack()->query_hp()
         + "\nEnemy bonus: " + query_bonus_attack() + "\n");

}