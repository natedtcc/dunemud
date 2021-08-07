// Testmob.c
// Potential mob for sapho pipe area
// - Bebop 2021

inherit "/obj/player/monster";

int is_mob_scaled = 0;

void reset(status arg)
{
  ::reset(arg);
  if (!arg) {
    set_name("tester");
    set_alias(({"tester", "test"}));
    set_race("none");
    set_gender(1);
    set_short("Scaling Tester");
    set_long("Just a lil' test mob.");
    set_level(1);
    set_property("level_override");
    add_money(500 + random(500));
  }

}


void set_scaled(){ is_mob_scaled = 1; }

int is_scaled(){ return is_mob_scaled; }

void scale_mob(float levelMod, float bonusMod){

  // Don't scale if already scaled (or for pesky wizards)
  if ( is_scaled() || wizardp(this_player())) { return; }

  // If the player is level 20 or under, scale ONLY to exact level 
  int playerLevel = this_player()->query_level();
  if (playerLevel <= 20){ 
    set_level(playerLevel);
    set_scaled();
    return;
  }

  // Scale the mob
  set_level(playerLevel + to_int(playerLevel * levelMod));
  set_bonus_attack(to_int(playerLevel * bonusMod));
  add_money(800 + (playerLevel * 8));
  set_scaled();
  //  foreach (string skill: skill_list){
  //    improve_skill(skill, (2000 - query_skills(skill)) );
  //  }

}

void init() {
  ::init();
  scale_mob(0.08, 0.22);
}

