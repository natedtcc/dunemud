/* 
** BIG BOY GABE
** Bebop July 2018
*/

inherit "/obj/player/monster";
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

object weapon, armor;
 
void reset (status arg)
{
  ::reset(arg);
  if (!arg) {
  
    /* Set level to the base mob level plus the product of
      the base mob level and the boss modifier
                (../sapho/include/area_utils.c)
    */

    set_level (to_int(BASE_MOB_LEVEL * BOSS_MODIFIER + BASE_MOB_LEVEL));
    set_name ("gabriel");
    set_race ("Imperial");
    set_alias ("gabriel", "kingpin");
    set_short ("Gabriel, the Red-Tar Sapho Kingpin");

    set_long (
      "This is the man who personally oversees the production and sale of tons\n"
      +"of red-tar sapho. Many view Gabriel as a morally bankrupt individual,\n"
      +"knowingly selling a product that is very addictive. To this, he simply\n"
      +"responds: \"Don\'t hate the player, hate the game\".\n"
      +"WARNING: BEWARE OF GABRIEL, HE IS A BIG BOY");

    set_gender (1);
    add_money (200000 + random (100000)); 
    improve_skill("Dodge", 1250);
    improve_skill("Roll", 1250);
    improve_skill("Block", 1250);
    improve_skill("Staffs and Oaks", 1250); 
    set_property ("no_backstab");
    set_property ("no_confuse");
    set_property ("no_enervate");
    set_property ("no_terror");
    set_property ("no_poison", 1);
    set_property ("no_voice", 1);
    set_property ("no_finish", 1);
    set_property ("no_enslave", 1);
    set_property ("no_disarm", 1);
    set_property ("no_disrupt", 1);
    set_property ("no_blind", 1);
    set_property ("no_instakill", 1);
    set_property ("no_steal", 1);
    set_property ("no_throw");
    
    set_non_combat_chats (12, ({
      ({ "Gabriel looks you over.", "echo" }),
      ({ "Gabriel goes: Gnome sayin'??", "echo" }),
      ({ "Gabriel says: Why you lookin' in my eyes? I ain't got no candy..", "echo" }),
      ({ "Gabriel asks: You heard that new Juice WRLD?","echo" }),
      ({ "Gabriel twirls his cane between his fingers.","echo" }),
      ({ "Gabriel asks: Wha'chu want, ya mark-ass trick?", "echo"}),
      ({ "Gabriel lowers his brows and says: Ain't seent chu arount..","echo" }),
      ({ "*DING!* Gabriel checks his DMs.","echo" }) }) );
    
    set_combat_chats (20, ({
      ({ "Gabriel says: You done goofed now, sonson!", "echo" }),
      ({ "Gabriel pulls out his 9mm and fires it at you randomly!", "echo" })
      }));
  
    armor = clone_object( OBJ_DIR + EQUIP + "suit.c");
    weapon = clone_object ( OBJ_DIR + EQUIP + "cane.c");
  }

  if (armor)
    move_object (armor, this_object ());

  if (weapon)
    move_object (weapon, this_object ());

  if (armor || weapon)
    command ("powerup");
}