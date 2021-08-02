// fremen.c - Fremen Sietch multimob
// Original file by Juris, defiled by Bebop 2021

inherit "/obj/player/monster";
#include "/d/Fremen/bebop/sietch/include/defines.h"

// Gear
object          item,
                armour,
                weapon,
                boots;

// Combat related vars
int             level_variance = random(5), 
                rage = 0,
                hp_max;

string *skill_list = ({"Dodge", "Block", "Roll", "Crysknife"});

void reset (int arg) {
  set_heart_beat(0);
  ::reset (arg);
  if(!arg){
    set_name("fremen");
    add_money(2000 + random(700));
    set_race("fremen");
    set_no_tank(1);

    // Buncha BS props
    set_property ("level_override");
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
        level_variance = random(30);
        break;
      case 3 :
        set_alias("naib");
        set_short("Fremen naib");
        set_long("Very calm fremen naib.\n");
        set_gender(1);
        level_variance = random(40);
        break;
      case 4 :
        set_alias("warrior");
        set_short("Fremen warrior");
        set_long("Very serious and well trained fremen guard.\n");
        set_gender(1);
        level_variance = 20 + random(50);
        break;
      case 5 :
        set_alias("man");
        set_short("Old Fremen man");
        set_long("Very old man. Don't underestimate him.\n");
        item = clone_object(OBJ_DIR + "pearl.c");
        set_gender(1);
        level_variance = random(40);
        break;
      case 6 :
        set_alias("guard");
        set_short("Fremen guard");
        set_long("Well trained fremen guard.\n");
        set_gender(1);
        level_variance = 20 + random(50);
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
    
    // Set the mob's level relative to variance
    set_level(BASE_MOB_LEVEL + level_variance);

    // Set base attack bonus
    set_bonus_attack(BASE_BONUS);

    // Improve each skill in the skill array to 2000
    foreach (string skill: skill_list){
      improve_skill(skill, (2000 - query_skills(skill)) );
    }
    
    // Safely achieve a maximum hp of 2-2.5m, based
    // on the level of the mob
    hp_max = query_level() * 3550;
    set_max_hp(hp_max);
    add_hp(hp_max - (query_hp()) );
    
    // Clone gear
    armour = clone_object(OBJ_DIR + "suit.c");
    weapon = clone_object(OBJ_DIR + "crys.c");
    boots = clone_object(OBJ_DIR + "dboots.c");
    
    // Set AC for all possible armor slots (0-14)
    for (int i=0; i<15; i++){
      set_ac(BASE_AC + random(60), i);
    }

    set_dead_ob(this_object());
  }

  // Place / wear gear
  if (item) move_object(item, this_object());
  if (armour) move_object(armour, this_object());
  if (weapon) move_object(weapon, this_object());
  if (boots) move_object(boots, this_object());

  if (armour || weapon || boots) command("powerup");
  
}

void monster_died() {
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
    either after 20 rounds of combat or if the mob's 
    hp falls below 75%, set_bonus_attack is bumped from
    100 to 150, and incremented by 2 every round after
    that. The max bonus 200. Any defines found here are
    found in /sietch/include/defines.h 
    EXCEPTIONS - int hp_max & rage are defined locally
  */

  // Increment bonus attack by 2 each round
  // (ONLY IF RAGE IS ACTIVE!)
  if (query_attack() && rage && (query_bonus_attack() < MAX_BONUS)){
    set_bonus_attack( query_bonus_attack() + 2 );
  }

  // If conditions are met, enrage
  if (query_attack() && !rage &&
    (query_fight_rounds() > 20 || query_hp() < to_int(hp_max * 0.75 ))){
      tell_object(query_attack(), 
        "The Fremen's eyes burn $BOLD$CYANblue$OFF as "
        +"they slip into a terrifying rage!"
      );
      set_bonus_attack(POWERUP_BONUS);
      rage = 1;
  }

  // Punch !!! - (1/3 chance of doing 1-4k damage)
  if (this_object () && !random (3) && query_attack()){
    load_object (MREH_PUNCH)->punch(4000, 1000, 50, 1);
  }

  // Thai kick !!! - (1/4 of doing 1-5k damage)
  if (this_object () && !random (4) && query_attack()){
    load_object (MREH_THAI)->kick(5000, 1000, 50, 1);
  }

  /* Heal the mob if combat is paused (faster heartbeat completion)
   Also decrement bonus attack until base has been reached.
   Used the basic formula hp_max / HEAL_MOD (200) for the heal rate,
   equating to roughly a roughly 200 HB timeout maximum */
  if (this_object () && !query_attack() 
    && this_object()->query_hp() != hp_max){
    add_hp(hp_max / HEAL_MOD);
    
    if (query_bonus_attack() > POWERUP_BONUS && rage){
      set_bonus_attack(query_bonus_attack() - 1);
    }
    
    else { 
      set_bonus_attack(BASE_BONUS);
      rage = 0;
    }
  }

}