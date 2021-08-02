// area_utils.c -  Fremen Sietch
// Bebop 2021

// object LOGGER = clone_object("/d/Fremen/bebop/sietch/obj/logger.c");

// Define a base mob level

string PUNCH_OBJ = "/obj/cmds/npc/general/punch.c";
string KICK_OBJ = "/obj/cmds/npc/general/kick.c";
string THAI_OBJ = "/obj/cmds/npc/general/thaikick.c";
string HEAL_OBJ = "/obj/cmds/npc/general/heal.c";


int BASE_MOB_LEVEL = 675;
int LEVEL_VARIANCE = 30;
int BASE_SKILL_LEVEL = 800;
int AC_LEVEL = 1000;
int HP_MAX = 2000000;
string *MOB_SKILLS = {"Crysknife", "Dodge", "Block", "Roll"};

int BASE_BONUS = 130;
int POWERUP_BONUS = 180;
int MAX_BONUS = 250;
int BONUS_RATE = 2;

int MIN_SPECIAL_HIT = 2000;
int MAX_SPECIAL_HIT = 4000;
int MIN_HEAL = 10000;
int MAX_HEAL = 10000;
int NO_TANK_HIT = 1;
int CP_COST = 50;

// Valid args are "kick", "thaikick", "punch," and "heal".
void mreh_command(string arg){
  switch(arg){
    case ("kick"):
      load_object(KICK_OBJ)->kick(
      MAX_SPECIAL_HIT, 
      MIN_SPECIAL_HIT, 
      HIT_CP_COST, 
      NO_TANK_HIT
      );
    break;
    case ("punch"):
      load_object(PUNCH_OBJ)->punch(
      MAX_SPECIAL_HIT, 
      MIN_SPECIAL_HIT, 
      HIT_CP_COST, 
      NO_TANK_HIT
      );
    break;
    case ("thaikick"):
      load_object(THAI_OBJ)->kick(
      MAX_SPECIAL_HIT, 
      MIN_SPECIAL_HIT, 
      HIT_CP_COST, 
      NO_TANK_HIT
    );
    break;
    // case ("heal"):
    //   load_object(HEAL_OBJ)->heal(
    //   MAX_SPECIAL_HIT, 
    //   MIN_SPECIAL_HIT, 
    //   HIT_CP_COST, 
    //   NO_TANK_HIT
    //   );
    // break;
}

int generate_variance() {
  return random(LEVEL_VARIANCE);
}

// Initialize the mob as defined above
void initilize_mob(object mob){
  mob->set_level(BASE_MOB_LEVEL + generate_variance());
  mob->set_max_hp(HP_MAX);
  mob->add_hp(HP_MAX - this_object()->query_hp());
  mob->set_bonus_attack(BASE_BONUS);
  for (int i=0; i<15; i++){
    mob->set_ac(AC_LEVEL + random(60), i);
  }

  foreach (string skill: MOB_SKILLS){
    mob->set_skill(skill, BASE_SKILL_LEVEL);
  }
}
