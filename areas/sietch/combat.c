
/*
**  obj/player/combat.c
**
**  Dune's combat system.
*/

#include <mudlib.h>
#include <ansi.h>

inherit "obj/player/skills";
inherit ATTRIBUTES;
inherit PROPERTIES;

#include <log.h>
#include <wizlevels.h>
#include <combat.h>
#include <guild_names.h>

#include "/obj/player/damage_listeners.h"

#define DEBUG_ON
#define DEBUG_TARGET "orbital"
#include <debug.h>

#define USER_SAVE(N)        sprintf ("/data/users/%c/%s", N[0], N)
#define WEAPM               "obj/player/weapon_messages"
#define CLAN_ENTER          "d/Harko/faction/area/floor1/enter"
#define STOPPER             "d/Harko/faction/obj/stopper" //Faction War changes...
#define DROP_CMD            "obj/cmds/living/drop"

//The new PK daemons
#define RANGE_DAEMON        "/secure/daemons/pksys/range_daemon"
#define HUNT_DAEMON         "/secure/daemons/pksys/hunt_daemon"

#define POLD                "/secure/daemons/politicsd"
#define SOCKETDAEMON        "/obj/daemons/socketdaemon"
#define PK_DEATH_DAEMON     "/secure/daemons/pkills"
#define MIN_PK_LEVEL        20
#define MAX_OPP             8
#define HEARER              "/players/arfang/listenner"
#define EXPD                "/secure/daemons/expd"
#define ALT_D               "/obj/daemons/player_alt"
#define NORMAL_ATTACK_MOD   100
#define MONSTER_HP_MOD       80

#define WEAPON_CLASS_OF_HANDS_OF_MONSTERS       min (3 + this_object ()->query_level () / 3, 20)
#define WEAPON_CLASS_OF_HANDS_OF_PLAYERS        min (this_object ()->query_skills ("Hand to Hand") / 25, 40)

//d100 combat system defines
#define DEFENSE_CAP 900
#define WC_CAP      100
//This is important to exclude 100% avoidance
#define DODGE_CAP    30
#define BLOCK_CAP    30
#define GUILD_CAP    38

nosave int MIN_BOT_LEVEL = 20;

//#define NO_PK
//#define LOG_PLAYER_HITS
//#define LOG_PK_BATTLES

nosave int MAXINT = 2000000000,
           MAX_SKILL_LEVEL = 2000;

nosave int attack_index,       // Index of attaacks
           bonus_attack,       // Our bonus attacks
           local_weight,       // How much are we carrying
           brief_combat,       // Are we in brief combat?
           brief_defense,      // Are we in brief defense?
           weapon_class,       // Our weapon class
           weapon_class2,      // Another weapon class
           spell_cost,         // Cost for spells (unused)
           spell_dam,          // Damage for spells (unused)
           bloodloss,          // Are we losing blood?
           which_wep,          // Which weapon are we using ?
           two_wep,            // Are we using 2 weapons?
           attack_bonus,       // The bonus we get with our stats
           know_bonus,         // The bonus we get with our stats
           comm_bonus,         // The bonus we get with our stats
           mani_bonus,         // The bonus we get with our stats
           perc_bonus,         // The bonus we get with our stats
           dext_bonus,         // The bonus we get with our stats
           disc_bonus,         // The bonus we get with our stats
           tot_shares,         // Our total shares (unused)
           second_attack_flag, // Just a flag
           chk_other_room,     // Another flag
           Str_adj,            // Amount of bonus str
           Int_adj,            // Amount of bonus int
           Con_adj,            // Amount of bonus con
           Dex_adj,            // Amount of bonus dex
           Wis_adj,            // Amount of bonus wis
           Qui_adj,            // Amount of bonus qui
           extra_attacks,      // How many extra attacks are being granted?  They are now spread out over the ten combat rounds.
           *extra_attack_rate, // Array of extra attacks (granted by guild powers and such)
           *attack_pattern,    // Pattern of attacks
           *armour_class,      // Our armour class for each limb
           *default_ac;        // For mobs, their base base AC, because recalc_ac () would otherwise zero it.

nosave string cap_name,        // Our name capitalized
              weapon_type,     // Type of weapon we are using
              spell_name,      // Name of spell (unused)
              aim,             // Where we are aiming
              corpse_bprint,   // Special corpse generation
              *aid_list,       // Who we will aid
              *att_mess,       // Array of attack messages
              *tmp_where,      //
              *tmp_where2,     //
              *my_hit_message; // Array of hit messages

static string *gooddodge = ({ "dodge$ $YOUR attack.",
                              "slide$ out of the way of $YOUR attack.",
                              "casually evade$ $YOUR attack." });
static string *baddodge  = ({ "almost dodge$ $YOUR attack.",
                              "fail$ to dodge $YOUR attack.", });

static string *goodroll  = ({ "roll$ and avoid$ $YOUR attack.",
                              "roll$ away from $YOUR attack, barely evading being hit.",
                              "roll$ out of the way of $YOUR attack." });
static string *badroll   = ({ "roll$ with $YOUR attack but still take$ the hit.",
                              "almost roll$ with the attack.", });

static string *goodblock = ({ "block$ $YOUR attack, not even flinching.",
                              "block$ $YOUR attack, grunting slightly by the force of the blow.",
                              "block$ $YOUR attack fearlessly." });
static string *badblock  = ({ "absorb$ the brunt of $YOUR assault.",
                              "grunt$, feeling the shock of $YOUR blow.", });

static object who_killed,        // Who was killed (for statistics)
              attacker_ob,       // Who we are attacking
              new_attacker,      // Who we're attacking now
              this_weapon,       // What weapon we are using
              *alt_attacker_ob,  // Array of alternate enemies
              *hunter,           // Who are our hunters
              *hunted,           // Who is hunted by us
              *armours = ({ });  // What armours we have

static mixed *name_of_weapon,    // Name of our first weapon
             *name_of_weapon2,   // Name of our second weapon
             *hitting_limbs;     // What limbs we have that can be aimed at

static mapping damage_shields = ([ 0: ([ :1 ]),
                                   1: ([ :1 ]),
                                   2: ([ :1 ]) ]);   // Damage shields soak damage

static mapping contributor_map,    // Varrack 072003
               HIT_MESS,           // Array of hit messages
               MONSTER_MAX_HP_MAP, // Array of monster hps
               hunted_names;       // Who are we hunting -Rav 19Dec2011

nosave  mapping my_values,         //d100 snapshot values
                your_values;

nosave int    *hp_sp_table;        // Array of HP/SP values for monsters - Iria

int money,           // How rich are we
    is_npc,          // Are we an npc?
    level,           // What is our level?
    experience,      // How much experience do we have?
    hit_point,       // How many hps do we have?
    max_hp,          // Maximum hit points
    max_sp,          // Maximum spell points (now command points)
    ghost,           // Are we a ghost?
    spell_points,    // Current spell points
    whimpy,          // Wimpy percentage
    dead,            // Are we dead?
    strong_hand,     // Our strong hand (0, 1, 2)
    Str,             // Strength
    Int,             // Intelligence
    Con,             // Constitution
    Dex,             // Dexterity
    Wis,             // Wisdom
    Qui;             // Quickness

string name,         // Our name
       with;         // What weapon were we using ?

nosave object *damage_mod,  // Objects that modify damage
              *attack_mod;  // Objects that modify attacks

mapping limbs;              // Our limbs

status no_corpse;           // do we leave a corpse?-- Salrylia, Sept 2000
status no_socket_drop;      // do we drop sockets on death?-- Math, March 2016

nosave mapping m_extra_attacks = ([ 0: 0; 0; 0; 0; 0; 0; 0; 0; 0; 0,
                                    1: 1; 0; 0; 0; 0; 0; 0; 0; 0; 0,
                                    2: 1; 0; 0; 0; 0; 1; 0; 0; 0; 0,
                                    3: 1; 0; 0; 1; 0; 0; 1; 0; 0; 0,
                                    4: 1; 0; 0; 1; 0; 0; 1; 0; 0; 1,
                                    5: 1; 0; 1; 0; 1; 0; 1; 0; 1; 0,
                                    6: 1; 0; 1; 0; 1; 1; 0; 1; 0; 1,
                                    7: 1; 0; 1; 1; 0; 1; 1; 0; 1; 1,
                                    8: 1; 1; 1; 0; 1; 1; 0; 1; 1; 1,
                                    9: 1; 1; 1; 1; 0; 1; 1; 1; 1; 1,
                                 ]);

// Prototyping
        int add_combat_message (string type, mapping map);
varargs int calculate_pk_exp (int flag);
        int calc_xp ();
        int can_kill_environment (object ob, int sil);           // Can we kill here?
        void calculate_extra_attack_rate ();                      // Using extra_attacks and m_extra_attacks, calculate the extra attacks array.
        int process_damage_mod (int dam, object source);         // Process damage modifiers
static  int process_armour (int dam, string wh, object source, int penetration);  // Armour reduces damage
static  int process_defense (int dam, object source);            // Do we roll/block/dodge?
static  int cap_defense (int chance, int level);                 // Cap our defense based on level
        void attack_with_weapon(mixed *weapon, object target, int fighting_style);
varargs int special_attack(object target, int dam_mod, object weapon, int no_block, int penetration);

mapping make_limbs ();
mapping get_combat_stats();
int get_combat_value(string str);

string query_hit_mess (string c_type,int num);
string plural (string arg);
string query_possessive ();

void damage_limb (string where, int amount, object source);         // For doing damage to specific limbs
varargs int learn_something (string skill, float bonus, int no_gain, int no_loss);
                                                                    // Chance to improve (or regress) skills
varargs int get_base_damage(int class_factor, int stat_factor, int skill_factor); //Gives base damage of attacks
varargs status lose_limb (string where, object source);

          void recalc_ac ();                                    // Reset our armour class  Varrack 011502
          void start_hunted (object who);                       // Start being hunted by someone
          void start_hunting (object who);                      // Start hunting someone
          void stop_hunted (object ob);                         // Stop being hunted by someone
          void stop_hunting (object who);                       // Stop hutning someone
static    void check_death (object source);                     // Did we die?
private   void display_limbs (string ind, int val, int me);     // Displays our limbs.
protected void init_hp_sp_table ();


/*
 * The following routines are defined for usage:
 * stop_fight           Stop a fight. Good for scroll of taming etc.
 * hit_player           Called when fighting.
 * attacked_by          Tells us who are attacking us.
 * attack               Should be called from heart_beat. Will maintain attack.
 * query_attack         To figure out which object we're focussing on.
 * attack_object        Called when starting to attack an object.
 * test_if_any_here     For monsters. Call this one if you suspect no enemy
 *                      is here any more.
 *                      Return 1 if anyone there, 0 if none.
 * reduce_hit_point     Reduce hit points, but not below 0.
 * transfer_all_to:     Transfer all objects to dest.
 */

int  compute_limb_map (int percen)      {  return max_hp * percen * 5 / 100;                    }
int  hit_this_room ()                   {  chk_other_room = 0;                                  }
int  hit_other_room ()                  {  chk_other_room = 1;                                  }
int  query_aid_chance ()                {  return aid_chance;                                   }
int *query_attack_rate1 ()              {  return attack_rate;                                  }
int  query_extra_attacks ()             {  return extra_attacks;                                }
int *query_extra_attack_rate ()         {  return extra_attack_rate;                            }
int  query_bonus_attack ()              {  return bonus_attack;                                 }
int  query_brief_combat ()              {  return brief_combat;                                 }
int  query_brief_defense()              {  return brief_defense;                                }
int  query_c_adj ()                     {  return C_adj;                                        }
int  query_dead ()                      {  return dead;                                         }
int  query_exp ()                       {  return experience;                                   }
int  query_ghost ()                     {  return ghost;                                        }
int  query_h_adj ()                     {  return H_adj;                                        }
int  query_hand ()                      {  return strong_hand;                                  }
int  query_hlevel ()                    {  return hlevel;                                       }
int  query_s_adj ()                     {  return C_adj;                                        }
int  query_slevel ()                    {  return slevel;                                       }
int  query_unarmed_wc ()                {  return is_npc ? WEAPON_CLASS_OF_HANDS_OF_MONSTERS :
                                                           WEAPON_CLASS_OF_HANDS_OF_PLAYERS;    }
int  reset_limbs ()                     {  limbs = make_limbs ();                               }
int  set_brief_combat (int i)           {  brief_combat = i;                                    }
int  set_brief_defense(int i)           {  brief_defense = i;                                   }
int  set_hand (int n)                   {  strong_hand = n;                                     }

mapping query_contributor_map ()        {  return contributor_map;                              }
mapping query_limbs ()                  {  return limbs;                                        }
mapping query_hunted_names ()           {  return hunted_names;                                 } // Ravien 19Dec2011

object *query_alt_attack ()             {  return alt_attacker_ob;                              }
object *query_armours ()                {  return armours;                                      }
object  query_attack ()                 {  return attacker_ob;                                  }
object *query_damage_mod ()             {  return damage_mod;                                   }
object* query_hunted ()                 {  return hunted = (hunted || ({ })) - ({ 0, });        }
object* query_hunter ()                 {  return hunter = (hunter || ({ })) - ({ 0, });        }

status query_modify_all_attacks ()      {  return modify_all_attacks;                           }
status query_no_corpse ()               {  return no_corpse || query_property ("no_corpse");    }
status query_no_socket_drop ()          {  return no_socket_drop;                               }

string *query_aid_list ()               {  return aid_list;                                     }
string  query_aim ()                    {  return aim;                                          }
string  query_combat_version ()         {  return "9.0";                                        }
string  query_name ()                   {  return cap_name;                                     }
//string  query_poss_name ()              {  return QN [<1] == 's' ? QN + "'" : QN + "'s";        }
string  query_poss_name ()              {  return regreplace (QN + "'s", "s's$", "s'", 0);      }
string  query_corpse_bprint ()          {  return corpse_bprint || CORPSE;                      }

void set_attacker (object ob)           {  attacker_ob = ob;                                    }
void block_all_attacks ()               {  modify_all_attacks = 1;                              }
void block_normal_attacks ()            {  modify_all_attacks = 0;                              }
void set_bonus_attack (int x)           {  bonus_attack = x;                                    }
void set_defen (int i)                  {  defen = clip (i, 0, 3);                              }
void set_my_hit_message (string *msg)   {  my_hit_message = msg;                                }
void set_no_corpse (status arg)         {  no_corpse = arg;                                     }
void set_corpse_bprint (string bprint)  {  corpse_bprint = bprint;                              }
void set_no_socket_drop (status arg)    {  no_socket_drop = arg;                                }

varargs void set_second_attack (int x)  {  second_attack_flag = x ? x : 1;                      }

/*
 *  At present, there are three classes of damage shield.
 *  0 - before defense is considered.
 *  1 - after defense is considered but before armour.
 *  2 - after armour has been considered but finally before damage is dealt.
 *  Each object is required to handle its own damage.
 */
void add_damage_shield (int class, object shield_object, string function_call)
{
  if (member (damage_shields, class))
  {
    if (damage_shields [class][shield_object])
      damage_shields [class][shield_object] += ({ function_call });
    else
      damage_shields [class] += ([ shield_object : ({ function_call }) ]);
  }
  else
    damage_shields += ([ class: ([ shield_object : ({ function_call }) ]) ]);
}

void remove_damage_shield (int class, object shield_object, string function_call)
{
  if (!damage_shields [class])
    return;

  if (damage_shields [class][shield_object])
    damage_shields [class][shield_object] -= ({ function_call });

  if (damage_shields [class][shield_object] == ({ }))
    damage_shields [class] -= ([ shield_object ]);
}

int process_shields (int class, int dam, string where, object source, int no_block)
{
  if(sizeof(damage_shields[class]))
  {
    mapping object_map = damage_shields[class];

    foreach(object shield_object : m_indices(object_map))
    {
      if(object_map[shield_object] == ({ }))
      {
        //damage_shields[class] -= ([ shield_object ]);
        m_delete(damage_shields[class], shield_object);
      }
      else
      {
        foreach(string funktion : object_map[shield_object])
        {
          //dam = shield_object->(funktion)(dam, where, source, no_block);
          dam = (int)call_other(shield_object, funktion, dam, where, source, no_block);
        }
      }
    }
  }

  return dam;
}

mapping query_damage_shields ()
{
  return damage_shields;
}

void print_limbs (int me)
{
  mixed limbstatus = ({ });
  string *limbmessages = ({ "missing",
                            "in an abominable state",
                            "in a very bad state",
                            "in a bad state",
                            "severely wounded",
                            "badly wounded",
                            "",
                            "wounded",
                            "",
                            "in perfect shape" });

  for (int i = 0; i <= 10; i ++)
    limbstatus += ({ ({ }) });

  foreach (string l, int i : limbs)
    limbstatus [min (i * 10 / (compute_limb_map (MAX_HP_MAP [l]) + 1), 10)] += ({ LOCA_CORR [l] });

  // Merge 5 and 6.  Merge 7 and 8.
  limbstatus [5] += limbstatus [6];
  limbstatus [6] = ({ });
  limbstatus [7] += limbstatus [8];
  limbstatus [8] = ({ });

  // Anything healthier than that does not get printed.
  for (int i = 0; i < sizeof (limbmessages) - 2; i ++)
    if (sizeof (limbstatus [i]))
      printf ("%-=78.78s\n",
              sprintf ("%s %s %s %s.",
                       this_player() && this_player() == this_object() ? "Your" : (capitalize(this_object()->query_possesive() || "Its")),
                       _implode (limbstatus [i], ", ", " and "),
    case "failed dodge":
      if (stringp (msg))
        baddodge += ({ msg });
      else if (pointerp (msg))
        baddodge += msg;
      break;

    case "roll":
    case "good roll":
      if (stringp (msg))
        goodroll += ({ msg });
      else if (pointerp (msg))
        goodroll += msg;
      break;
    case "bad roll":
    case "failed roll":
      if (stringp (msg))
        badroll += ({ msg });
      else if (pointerp (msg))
        badroll += msg;
      break;

    case "block":
    case "good block":
      if (stringp (msg))
        goodblock += ({ msg });
  }
}

mapping make_limbs ()
{
  int i;
  mapping limby;
  string tmp;

  limby = ([ ]);
  for (i = 0; i < sizeof (m_indices (is_npc ? MONSTER_MAX_HP_MAP : MAX_HP_MAP)); i ++)
  {
    tmp = m_indices (is_npc ? MONSTER_MAX_HP_MAP : MAX_HP_MAP)[i];
    if (limbs)
    {
      if (limbs [tmp] != 0 || is_npc)
        limby [tmp] = compute_limb_map (is_npc ? MONSTER_MAX_HP_MAP [tmp] : MAX_HP_MAP [tmp]);
      else
        limby [tmp] = 0;
    }
    else
      limby [tmp] = compute_limb_map (is_npc ? MONSTER_MAX_HP_MAP [tmp] : MAX_HP_MAP [tmp]);
  }
  return limby;
}

int heal_limbs (int healing)
{
  walk_mapping (limbs, lambda ( ({'ind, 'val, 'limbs, 'healing, 'limbo}),
    ({#'+=,
      ({#'[,
        'limbs,
        'ind}),
      ({#'?,
        ({#'&&,
          ({#'<,
        ({#'+,
          ({#'[, 'limbs, 'ind}),
                  'healing }) ,
        ({#'[, 'limbo, 'ind}) }),
          ({#'!=,
        ({#'[, 'limbs, 'ind}),
                0 }) }),
            'healing,
        ({#'?,
          ({#'==, ({#'[, 'limbs, 'ind}), 0}),
              0,
          ({#'-, ({#'[, 'limbo, 'ind}), ({#'[, 'limbs, 'ind}) }) }) })
        }) ), limbs , healing, make_limbs());
}

int add_limb (string limb)
{
  limbs [limb] = compute_limb_map (is_npc ? MONSTER_MAX_HP_MAP [limb] : MAX_HP_MAP [limb]);
  return 1;
}

// Updating the bonus
void update_bonus ()
{
  Str_total = Str + TO->query_str_adj ();
  Dex_total = Dex + TO->query_dex_adj ();
  Con_total = Con + TO->query_con_adj ();
  Int_total = Int + TO->query_int_adj ();
  Qui_total = Qui + TO->query_qui_adj ();
  Wis_total = Wis + TO->query_wis_adj ();

  // Ignore all of this! we have yet to find a use for this;) leaving it for uh?
  // ^ If we're not going to use this, let's comment it out? -Orb 2018

  attack_bonus = (Str_total * 2 + Int_total + Dex_total * 3) / 6;
  know_bonus   = Int_total;
  comm_bonus   = (Int_total + Wis_total) / 2;
  mani_bonus   = Dex_total;
  perc_bonus   = Int_total;
  dext_bonus   = Dex_total;
  disc_bonus   = Dex_total;

}

// This function will return a location basing on where we are aiming
// and how good we are with this weapon.
string find_where (string aimed, int modifier, object target)
{
  mapping tmp_limbs;
  int tmp, loop, irad;

  if (!objectp (target))
    return 0;

  tmp_limbs = copy (target->query_limbs ());            // Opponent's limbs (mapping)
  tmp_where = m_indices (tmp_limbs);                    // Array of limb names

  // Here we remove any missing limbs from the mapping
  for (loop = 0; loop < sizeof (tmp_where); loop ++)
    if (tmp_limbs [tmp_where [loop]] == 0)
      m_delete (tmp_limbs, tmp_where [loop]);

  tmp_where = m_indices (tmp_limbs);                    // re-initialize this after updating the tmp_limbs map
  if (stringp (aimed) && aimed && aimed == "random")    // If we are aiming at random, return a random limb.
  {
    if (pointerp (tmp_where))
    {
      if (!sizeof (tmp_where))
        return 0; // No limb present at this location
      irad = random (sizeof (tmp_where));
      if (!stringp (tmp_where [irad]))
        return 0; // No limb present at this location
      else
        return tmp_where [irad];
    }
    else
      return 0; // No limbs
  }

  if (member (tmp_limbs, aimed) == 0 || tmp_limbs [aimed] == 0)
  { // What if we don't have that limb anymore?
    printf ("Your aiming is not correct!\nYour opponent doesn't have a %s !!(aiming at random)\n",
             member (LOCA_CORR, aimed) ? LOCA_CORR [aimed] : aimed);
    aim = "random"; // Switch to random aiming
    return find_where (aim, modifier, target); // Redo the function
  }

  tmp = random (100); // Percentage

  if (tmp < modifier)
    return aimed; // modifier is weapon skill, the higher it is, the more accurate the hit

  tmp = random (10); // Re-assign tmp instead of using another integer

  if (LOCATIONS_MAP [aimed])
  {
    // We have another chance to hit where we are aiming
    if (tmp == 0 && member (tmp_where, LOCATIONS_MAP [aimed][0] != -1))
      return LOCATIONS_MAP [aimed][0];

    // Reduce tmp until we can reference it to the locations mapping
    while (!LOCATIONS_MAP [aimed][tmp])
      tmp --;

    if (LOCATIONS_MAP [aimed][tmp])
      if (member (tmp_where, LOCATIONS_MAP [aimed][tmp]) != -1)
        return LOCATIONS_MAP [aimed][tmp];
  }

  return tmp_where [random (sizeof (tmp_where))]; // When all else fails, return a random limb
}

varargs void stop_fight (mixed o)
{
  int i;

  if (o == this_object ())
  {
    if (attacker_ob == this_object ())
      attacker_ob = 0;

    alt_attacker_ob -= ({ this_object () });

    log_file ("BAD_STOP_FIGHT", sprintf ("previous_ob: %O this_object: %O arg: %O on %s\n",
                                          previous_object (), this_object (), o, ctime ()));
    return;
  }

  if (alt_attacker_ob && pointerp (alt_attacker_ob))
    alt_attacker_ob -= ({ 0 });

  if (!o)
  {
    if (objectp (attacker_ob) && living (attacker_ob))
      attacker_ob->stop_fight (TO);

    attacker_ob = 0;
    return;
  }

  if (o == "all")
  {
    if (objectp (attacker_ob))
      attacker_ob->stop_fight (TO);

    attacker_ob = 0;
    for (i = 0; i < sizeof (alt_attacker_ob); i ++)
      if (objectp (alt_attacker_ob [i]) && living (alt_attacker_ob [i]))
        alt_attacker_ob [i]->stop_fight (TO);

    alt_attacker_ob = ({ });
    return;
  }
  else if (objectp (o) && living (o) && o != this_object ())
  {
    if (attacker_ob != this_object () && o == attacker_ob)
    {
      attacker_ob = 0;
      o->stop_fight (TO);
    }

    if (member (alt_attacker_ob, o) != -1)
    {
      if (objectp(o) && living (o))
        o->stop_fight (TO);
      alt_attacker_ob -= ({ o });
    }
  }

  if (!attacker_ob && sizeof (alt_attacker_ob))
  {
    attacker_ob = alt_attacker_ob [0];
    alt_attacker_ob -= ({ attacker_ob });
  }
  return;
}

void transfer_all_to (object dest)
{
  object ob;
  foreach (ob : all_inventory ())
  {
    if (ob && ob->query_kept ())
      ob->set_kept (0);
    if (!ob->drop (1) && ob)
      transfer (ob, dest);
  }

  if (money)
  {
    ob = clone_object (MONEY);
    ob->set_money (money);
    move_object (ob, dest);
    money = 0;
  }

  local_weight = sum_array (all_inventory ()->QW);
}

// Returns an array of our armours covering the 'where' specified
object *find_armours (string where)
{
  object *ob,
         *tmp_armours;
  int x,
      tmp;

  if (!where || !stringp (where))
    return 0;

  tmp = (find_object ("/obj/player/aim_d") || load_object ("obj/player/aim_d"))->query_where(where);
  tmp_armours = ({ });
  ob = armours;

  if (!sizeof (ob))
    return 0;

  for (x = 0; x < sizeof (ob); x ++)
  {
    if (!ob [x] || !objectp (ob [x]))
      continue;
    if (ob [x]->query_ac (tmp))
      tmp_armours += ({ ob [x], });
  }

  return tmp_armours;
}

// flag == 0 or omitted : classical check
// flag == 1            : silent check
// flag == 2            : bypass pk flags check (but keep other checks)
varargs int is_in_range (object target, status flag)
{
  // Playerkilling system revamped by Mreh, Jan. 15, 2007.
  //
  // Playerkiller system revamped again - Paradox 06/16/10
  if (!pointerp (hunted))
    hunted = ({ });

  if (!pointerp (hunter))
    hunter = ({ });

  if (!objectp(target) || target == this_object())
    return 0;

  // If one of these is a mob...
  if (!playerp (target) || !playerp (TO))
  {
    if (!playerp (TO))
    {
      if (!target->query_pk_owner ())
        return 1;
      return TO->can_kill (target->query_pk_owner (), 1);
    }
    if (!TO->query_pk_owner ())
      return 1;
    return target->can_kill (TO->query_pk_owner (), 1);
  }

#ifdef NO_PK
   if (playerp (TO) && playerp (target))
   {
     if (!flag)
       write ("Playerkilling is currently disabled.\n");
     return 0;
   }
#endif

  if (flag != 2)
  {
    if (playerp (target) &&
        target->query_is_registered_bot () &&
        target->query_level () >= MIN_BOT_LEVEL &&
        (this_object ()->query_is_registered_bot () || this_object ()->defined_env_var ("attack_bots")))
        return 1; // Returning valid.  Bots should be attackable after level 20 regardless.
        //;       // No OP here, because it circumvents the regular PK checks.
    else
    {
      if (!TO->query_pk ())
      {
        if (!flag)
          printf ("You are not a playerkiller.\n");
        return 0;
      }

      if (!target->query_pk ())
      {
        if (!flag)
          printf ("%s isn't a playerkiller.\n", target->QN);
        return 0;

  if (TO->query_guild () == "none" || target->query_guild () == "none")
  {
    printf ("The unguilded may not engage in playerkilling.\n");
    return 0;
  }

  string attackerGuild = this_object()->query_guild();
  string defenderGuild = target->query_guild();

  //DUMP_VAR(TO);
  //DUMP_VAR(attackerGuild);
  //DUMP_VAR(defenderGuild);

  int *polstat = ({
    POLD->query_political_status (attackerGuild, defenderGuild),
    POLD->query_political_status (defenderGuild, attackerGuild),
  });

  //DUMP_VAR(polstat);

  //Matres can free-kill other matres. Jacurutu used to be considered here
  //but I have removed the check until they are re-implemented.
  if(attackerGuild == defenderGuild && attackerGuild == "matres")
    return 2;

  //If we're at a defensive pact or higher with the target's guild
  //and they are at neutral or higher towards us, and we've already fiddled
  //with the setting recently, prevent combat.
  if (polstat [0] > 4 &&
      polstat [1] > 3 &&
      POLD->query_next_change (attackerGuild, defenderGuild) > time () &&
      flag != 2)
  {
    if (flag)
      printf ("Diplomatic channels prevent aggression with that player at the moment.\n");
    return 0;
  }

  //If the guilds are at the same political position with each other
  //and the position is ALLIED
  if (polstat [0] == polstat [1] && polstat[0] == 5)
  {
    if(flag != 1)
      printf ("You must file your grievances through proper channels.\n");

    return 0;
  }

  //DUMP_VAR(RANGE_DAEMON->can_kill(this_object(), target));

  //Check if we can kill the target. This will include all considerations
  //of level difference, political differences, and hunting/vowing
  if(RANGE_DAEMON->can_kill(this_object(), target))
    return 1;

  if (!flag)
    write ("You are out of the pk range for this player.\n");

  return 0;
}

int can_kill (object ob, int sil)
{
  if (!pointerp (hunter))
    hunter = ({ });

  if (!pointerp (hunted))
    hunted = ({ });

  if (!objectp (ob))
    return 0;

  // Prevent attacking things that happen to be living,
  // but are not killable.  Varrack 101203
  if (living (ob))
    if (!function_exists ("hit_player", ob))
      return 0;

  if (TO == ob)
  {
    if (!sil)
      write ("Attack yourself ?\n");
    return 0;
  }

  if (!living (ob))     // Is it alive?
  {
    if (!sil)
      write ("You can only kill living beings !\n");
    return 0;
  }

  if (playerp (ob) && ob->query_linkdead () > 0)        // Player and LD
    return 0;

  if (!TO->can_see (ob))                                // Can we see it?
    return 0;

  if (ob->query_property ("no_fight") || TO->query_prop ("no_fight"))
  {
    if (!sil)
      tell_object (TO, "Fighting is not permitted here.\n");
    return 0;
  }

  if (playerp (ob) && playerp (TO) &&
     (ob->query_env_var ("no_pk") || TO->query_env_var ("no_pk")))
  {
    if (!sil)
      tell_object (TO, sprintf ("%s not allowed to participate in playerkilling.\n",
                                TO->query_env_var ("no_pk") ? "You are" : "That person is"));
    return 0;
  }

  // A player is not allowed to kill their bot alts.
  if ((playerp (ob) && ob->query_is_registered_bot () ||
       playerp (this_object ()) && this_object ()->query_is_registered_bot ()) &&
      ALT_D->is_related (ob->query_real_name (), this_object ()->query_real_name ()))
      return 0;

  switch (can_kill_environment (ob, sil))       // -Stryder 052203
  {                                             // can we kill in this place?
  // We can attack our alternate attackers and things which have attacked us but switched.
  if (member (TO->query_alt_attack (), ob) > -1 || member (ob->query_alt_attack (), TO) > -1)
    return 1;

  if (pointerp (ob->query_hunted ()))           // Are we hunted by ob,
    if (member (ob->query_hunted (), TO) != -1)
    {
      if (is_npc || ob->query_npc ())           // This should only count for player-player fights - Elmer 3/April/2002
        return 1;
    }

  if (!interactive (TO))
    return 1;

  if (interactive (ob))
  {
    if (guestp (ob))
    {
      if (!sil)
        write ("Don't attack our guests.\n");
      return 0;
    }

    if (wizardp (ob) && !TO->is_test_player () && !wizardp (TO))        // Varrack 012804
    {
      if (!sil)
  if (!objectp (ob))
    return;
  if (pointerp (damage_mod))
    damage_mod -= ({ ob });
}

object *query_damage_modifiers ()
{
  if (!damage_mod)
    return 0;

  if (!pointerp (damage_mod))
  {
    damage_mod = ({ });
    return 0;
  }

  return damage_mod;
}

//Had to keep this in for legacy objects using attack modifiers.
//Leondal

void attack_modifier (mixed ob)
{
  if (stringp (ob))
object *query_attack_modifiers ()
{
  if (!pointerp (attack_mod))
  {
    attack_mod = ({ });
    return 0;
  }

  return attack_mod;
}

varargs int calculate_pk_exp (int flag)
{
  int pk_exp, qlevel;

  if (this_object ()->query_is_registered_bot ())
    return 0;

  qlevel = this_object ()->query_level ();

  switch (qlevel)
  {
    case   1 ..   20:  return max (0,   5000 + qlevel *  3000);
    case  21 ..   50:  return max (0,   7500 + qlevel *  4250);
    case  51 ..   80:  return max (0,  10000 + qlevel *  5500);
    case  81 ..  120:  return max (0,  12500 + qlevel *  6750);
    case 121 ..  160:  return max (0,  15000 + qlevel *  8000);
    case 161 ..  200:  return max (0,  17500 + qlevel *  9250);
    case 201 ..  250:  return max (0,  20000 + qlevel * 10500);
    case 251 ..  300:  return max (0,  22500 + qlevel * 11750);
    case 301 .. 1000:  return max (0,  25000 + qlevel * 13000);
    default:           return max (0,   2500 + qlevel *  1750);
  }

  return pk_exp;
}

int get_gun_chance (int chance, int skill)
{
  if (!chance)
    return 0;

  switch (skill)
  {
    case    1 ..   25:  chance = chance * 99 / 100;     break;
    case   26 ..   50:  chance = chance * 91 / 100;     break;
    case   51 ..   75:  chance = chance * 78 / 100;     break;
    case   76 ..  100:  chance = chance * 66 / 100;     break;
    case  101 ..  150:  chance = chance * 55 / 100;     break;
    case  151 ..  225:  chance = chance * 45 / 100;     break;
    case  226 ..  300:  chance = chance * 36 / 100;     break;
    case  301 ..  400:  chance = chance * 28 / 100;     break;
    case  401 ..  600:  chance = chance * 21 / 100;     break;
    case  601 ..  900:  chance = chance * 15 / 100;     break;
    case  901 .. 1225:  chance = chance * 10 / 100;     break;
    case 1226 .. 1750:  chance = chance *  6 / 100;     break;
    case 1751 .. 1900:  chance = chance *  3 / 100;     break;
    default          :  chance = chance *  1 / 100;     break;
  }

  return chance;
}

varargs int get_spice_bonus (int chance, object source)
{
  if (function_exists ("query_spiced", TO) && function_exists ("query_max_spiced", TO))
    chance += ({ 0, 0, 2, 4, 7, 11, 18, 9, 3, -5, -15 })[clip (percentage (TO->query_spiced (), TO->query_max_spiced ()) / 10,
                                                               (TO->query_guild () == "Fremen" ? 2 : 0),
                                                               (TO->query_guild () == "Fremen" ? 8 : 10))];
  else if (function_exists ("query_spiced", TO))
    chance += ({ 0, 0, 2, 4, 7, 11, 18, 9, 3, -5, -15 })[max (percentage (TO->query_spiced (),
                                                                          level * 16 + Con * 2) / 10, 10)];

  if (source && function_exists ("query_spiced", source) && function_exists ("query_max_spiced", source))
    chance -= ({ 0, 0, 1, 3, 5, 7, 9, 5, 1, -3, -21 })[clip (percentage (source->query_spiced (), source->query_max_spiced ()) / 10,
                                                             (source->query_guild () == "Fremen" ? 2 : 0),
                                                             (source->query_guild () == "Fremen" ? 8 : 10))];
  else if (source && function_exists ("query_spiced", source))
    chance -= ({ 0, 0, 1, 3, 5, 7, 9, 5, 1, -3, -21 })[max (percentage (source->query_spiced (),
                                               source->query_level () * 16 + source->query_con () * 2) / 10, 10)];

  return chance;
}

// Gives a chance to increase or decrease the skill 'skill'
// This should only be for weapon and defense skills.
varargs int learn_something (string skill, float bonus, int no_gain, int no_loss)
{
  int brainpower, chance;
  float extra;

  if (!this_object () ||
      this_object ()->query_real_skill (skill) == -1 ||
      this_object ()->query_real_skill (skill) >= MAX_SKILL_LEVEL)
    return 0;

  if (!bonus)
    bonus = 0.0;
  brainpower = this_object ()->query_int () +
               this_object ()->query_wis () +
              (this_object ()->query_real_int () +
               this_object ()->query_real_wis ()) / 30;

  extra = this_object ()->query_enhanced_perception () || 0.0;
  bonus += clip (extra, 0.0, 20.0);

  switch (percentage (this_object ()->query_real_skill (skill), MAX_SKILL_LEVEL))
  {
    case  0 ..  5: chance =    1000;  break;
    case  6 .. 10: chance =    2000;  break;
    case 11 .. 20: chance =    4000;  break;
    case 21 .. 40: chance =    8000;  break;
    case 41 .. 60: chance =   16000;  break;
    case 61 .. 80: chance =   32000;  break;
    case 81 .. 90: chance =   64000;  break;
    case 91 .. 92: chance =  128000;  break;
    case 93 .. 94: chance =  256000;  break;
    case 95 .. 96: chance =  512000;  break;
    case 97 .. 98: chance = 1024000;  break;
    case       99: chance = 2048000;  break;
    default      : chance =      -1;  break;
  }

  if (chance == -1)
    return 0;

  extra = max (chance - to_int (brainpower * bonus), 0);

  if (!random (to_int (extra)) && !no_gain)
  {
    tell_object (this_object (),
                 sprintf ("%sYou just advanced %s!%s\n",
                          ANSI (bold white),
                          skill,
                          ANSI (normal)));
    this_object ()->improve_skill (skill, 1);
      return 1;
  }

  if (no_loss)
    return 0;

  switch (percentage (this_object ()->query_real_skill (skill), MAX_SKILL_LEVEL))
  {
    case  0 .. 20: chance = 2048000;  break;
    case 21 .. 40: chance = 1024000;  break;
    case 41 .. 60: chance =  512000;  break;
    case 61 .. 80: chance =  256000;  break;
  int protection;

  string type;

  float DAMAGE_REDUCTION;

  if (!source || !stringp(wh))
    return dam;

  armour_array_position = "/obj/player/aim_d"->query_where (wh);

  //Leondal added an armour penetration factor here - 04/2019
  raw_armour = armour_class[armour_array_position];

  if(penetration)
  {
      penetration = min(penetration, 50); //Max of 50% armor pen
      raw_armour -= ( ( raw_armour * penetration ) / 100 );
  }

  DAMAGE_REDUCTION = raw_armour / (armour_class [armour_array_position] + 125.0);

  if (member (LOCATIONS, wh) != -1)
    dam = to_int (ceil (dam * (1.0 - DAMAGE_REDUCTION)));

  type = source->query_this_weapon () ? source->query_this_weapon ()->query_damage_type () : "blunt";
  protection = min (75, TO->query_armour_protection (type, armour_array_position) / 2);   // Max 75% reduction from protection.
  DAMAGE_REDUCTION = 1.00 - protection / 100.0;

  dam = to_int (ceil (dam * DAMAGE_REDUCTION));

  // Trigger armour sockets.  Any return from them reduces damage.
  if (sizeof (armours))
  {
  for (j = 0; j < sizeof (armours); j ++)
  {
    if (armours [j])
    {
      k = allocate (15);
      k = call_other (armours [j], "armour_class");
      if(!pointerp(k) || !sizeof(k)) // added this check cause something was causing k==0. - Math, Feb 2021
        continue;
      for (i = 0; i < 15; i ++)
        armour_class [i] += k [i];
    }
    else
      armours -= ({ armours [j] });
  }

  // Because mobs have base armour.
  if (is_npc)
    for (i = 0; i < 15; i ++)
      armour_class [i] += default_ac [i];
}

int process_damage_mod (int dam, object source)
{
    if (!source)
        return dam;

    if (damage_mod)
    {
        //disabled for lag check
        //"/secure/daemons/guild_powers.c"->pre_hit (TO);
        map (damage_mod, lambda (({ 'ob, 'dam }), ({ #'=, 'dam, ({ #'?, ({ #'objectp, 'ob}), ({ #'call_other, 'ob, "modify_damage", 'dam, source}),'dam, }) })), &dam);
    }
    return dam;
}

/*
 * This function is called from other players when they want to make
 * damage to us. We return how much damage we received, which will
 * change the attackers score. This routine is probably called from
 * heart_beat() from another player.
 */
varargs int hit_player (int dam, string where, object source, int no_block, int penetration)
{
  int secondary_defense,  // Chance of defending against more than one attacker'
      i;

  // Defensive monitoring. -Methos 112605
  /* Ravien 19Dec2011 Check for MAXINT. */
  if(TO->query_property("pre_hit_player_damage")<2000000000)
    TO->set_property ("pre_hit_player_damage", TO->query_property ("pre_hit_player_damage") + dam);

  // If this player has no limbs, they should probably be dead, right?  - Mreh, 2007/01/18.
  while (!where && !stringp (where) && !member (limbs, where) && sizeof (limbs))
    where = find_where ("random", 0, TO);

  // Initializing alt_attacker_ob
  if (!alt_attacker_ob)
    alt_attacker_ob = ({ });

  if (!attacker_ob)
    set_heart_beat (1);

  if (attacker_ob == this_object ())
  {
      // Stop hitting yourself.
      attacker_ob = 0;
      dam = 0;
  }

  // Players can't do damage to themselves with hit_player () anymore.
  // This may affect some things (like Harko sacrifice), so let's log it.
  if (source == this_object ())
  {
    return 0;
  }

  // Implemented source so that damage could come indirectly from livings
  // and not bug everything up when hit_player() starting looking
  // for TP.  Varrack  030901
  if (!source)
    source = TP;

  if (dam > 2000 && playerp (source) && (source->QRN != "ike"))
  {
    log_file ("@BS_DAMAGE", sprintf ("%s did %d dam with %s cmd to %s.\n",
                                     source->QRN,
                                     dam,
                                     object_name (previous_object ()),
                                     object_name(TO)));
  }

#ifdef LOG_PLAYER_HITS
  if(source->QRN == LOGGED_PLAYER)
  {
    log_file ("@LOGGED_PLAYER_DAMAGE", sprintf ("%s did %d dam with %s cmd to %s on %s.\n",
                                                 source->QRN,
                                                 dam,
                                                 (string) object_name (previous_object ()),
                                                 object_name (TO),
                                                 ctime (time ())));
  }
#endif

  /* The following line replaces a previous check for TP && !TP->can_kill(TO,1) because source
                                        ctime (time ()),
                                        object_name (previous_object ())));

  // disabled 120107
  //"/secure/daemons/guild_powers.c"->pre_hit (TO);

  // Setting attacker_ob (now with source)
  if (!attacker_ob && source != TO)
    attacker_ob = source;

  // Updating alt_attacker_ob (now with source)
  else if (sizeof (alt_attacker_ob))
    if (member (alt_attacker_ob, source) == -1 && attacker_ob != source && source != TO && source)
      alt_attacker_ob += ({ source });

  // Removing all 0's from the list (attackers that died, etc) - Elmer
  alt_attacker_ob -= ({ 0 });
  alt_attacker_ob -= ({ attacker_ob });

  // Updating alt_attacker_ob for attackers who left the room - Elmer
  if (environment (TO))
    alt_attacker_ob = filter (alt_attacker_ob, #'present, environment (TO));

  // Updating attacker_ob for if he left the room - Elmer
  if (attacker_ob && environment (TO) && !present (attacker_ob, environment (TO)))
  {
    // Changing attacker_ob to source and removing source from the list
    attacker_ob = source;
    alt_attacker_ob -= ({ source });
  }

  /* Don't damage wizards too much ! */
  if (wizardp (TO) && dam * 2 >= hit_point)
    return 0;

  /* Or someone who is dead */
  if (dead)
    return 0;

  if (environment (TO)->is_axlotl_tank ())
    return 0;

  // d100 system makes class 0 unecessary - leaving in for now
  // This is where the class 0 damage shields kick in.
  dam = process_shields (0, dam, where, source, no_block);

  // This is where the class 1 damage shields kick in.
  dam && dam = process_shields (1, dam, where, source, no_block);

  // damage calculation and armour damages
  dam && dam = process_armour (dam, where, source, penetration);

  // This is where the class 2 damage shields kick in.
  dam && dam = process_shields (2, dam, where, source, no_block);

  //Disabled 120107
  //"/secure/daemons/guild_powers.c"->post_hit (TO);

  if (!TO)
  {
    tell_listeners (dam, object_name (previous_object ()), this_player ());
    return dam;
  }

  // Damage Monitoring. Methos 112605
  TO->set_property ("post_hit_player_damage", TO->query_property ("post_hit_player_damage") + abs (dam));

  if (dam <= 0)
    return 0;

  // Is this a major wound ?
  if (!limbs)
    limbs = make_limbs ();

  // Mapping holds player objects that have contributed to the damage of this object
  // Used later to divvy up exp.  Varrack 072003
    return 0;

#ifdef LOG_PK_BATTLES
  if (playerp (TO) && playerp (source))
    log_file ("@PK_BATTLE." + TO->QRN, sprintf ("%s did %d dam to me with %s cmd on %s.\n",
                                                 source->QRN,
                                                 dam,
                                                 object_name (previous_object ()),
                                                 ctime (time ())));
#endif

  if (this_object () && sizeof (this_object ()->query_armours ()))
    map_objects (this_object ()->query_armours (),
                 "post_process",
                 dam,
                 source);

  tell_listeners (dam, object_name (previous_object ()), this_player ());
  check_death (source);
  return dam;
}

// New exp distribution scheme.  Varrack 072003
void grant_exp (string k, int share_damage, int total_damage, int total_exp)
{
  float share_xp;
       total_damage,    // Varrack 072003
       value,
      *polstat,         // For changing political stances
       chance;

  object tsoul;         // For handling tlx Assassin. Ravien 02Sep05
  string *guilds;       // For looking up in POLD.

  guilds = allocate (2);
  polstat = allocate (2);

  int assassinated = 0;

  if (!source || !living (source))
    return;

  if (!objectp (source))
    return;

  if (playerp (source) && source->query_guild () == "Tleilax" &&
      playerp (TO)     &&     TO->query_guild () == "Tleilax")
    if ((tsoul = present ("tsoul", source)) && tsoul->query_gstatus () == "Assassin")
      assassinated = 1;

  // To handle player-owned mobs.
  if (source->query_npc () && source->query_pk_owner ())
    source = source->query_pk_owner ();

  if (hit_point <= 0 || dead)
  {
    object corpse;

    if (pointerp (hunter))
    {
      hunter -= ({ 0 });
      hunter->stop_hunting (TO);
    }

    if (pointerp (hunted))
    {
      hunted -= ({ 0 });
      hunted->stop_hunted (TO);
    }

    if (!this_object ()->query_death_message () ||
         this_object ()->query_death_message () == "")
    {
      tell_object (source, "You killed " + this_object ()->query_name () + ".\n");
      tell_others (TO, parse_ansi ("$BOLD$RED%N died.$OFF\n"), 3, source);
    }

    if (!is_npc)
    {
      tell_object (TO, "$BOLD$REDYou just died !$OFF\n");
      hit_point = 10;
      // If the room is not a normal combat room - Factions, for example...
      if (TO->query_property ("no_death") || (E && E->query_property ("no_death")))
      {
        string handler = TO->query_property ("no_death") ||
                         E->query_property ("no_death") ||
                         load_object (CLAN_ENTER);

        source->stop_hunting (TO);
        stop_hunted (source);
        if (present ("fact_ob"))
        {
          present ("fact_ob")->add_death (1);
          present ("fact_ob")->fact_shout (sprintf ("%s just killed %s", source->QN, CAP (name)));
          if (source && present ("fact_ob", source))
            present ("fact_ob", source)->add_kill (1);
        }

        string bprint = funcall(corpse_bprint);
        corpse = objectp(bprint) ? bprint : clone_object(stringp(corpse_bprint) ? corpse_bprint : CORPSE);
      //corpse = clone_object (CORPSE);
        corpse->set_player (1);
        corpse->set_level (level);
        corpse->set_gender (TO->query_gender ()); // temujin, march 13, 2006
        corpse->set_killer (playerp (source) ? source->QRN: (living (source) ? source->QN : "Unidentified"));
        corpse->set_guild (TO->query_guild ()); // temujin, march 13, 2006
        corpse->set_name (name); // triggers the heartbeat! should be last
        catch (transfer_all_to (corpse));
        move_object (corpse, environment (TO));
        corpse->recalc_carry ();

        move_object (TO, handler);
        if (handler && find_object (handler) && function_exists ("register_death", find_object (handler)))
          find_object (handler)->register_death (TO, source);

        move_object (clone_object (STOPPER), TO);
        return;
      }
    }

    /* Resume Normal Death here */
    dead = 1;

    // Its a player vs player battle
    pk_exp = (!is_npc && attacker_ob && playerp (source)) ? calculate_pk_exp () : 0;

    //pk death
    if (!is_npc && source)
    {
      if (!playerp (source))
        TO->add_killed ();
      else
        TO->add_pk_death ();

      if (source->query_npc ())
      {
        TO->set_nemesis (source->short (1));
        TO->set_nemesis_xp (source->query_exp ());
        tell_object (TO, sprintf ("Your Nemesis is now %s at %d experience.\n",
                                   source->QN, (int)source->query_exp ()));
      }
      else if (!assassinated)
      {
        TO->set_pk_nemesis (sprintf("%s [%s] at %d experience.",
                                    capitalize (source->query_real_name () || source->query_name ()),
                                    source->query_guild () || "none",
                                    source->calculate_pk_exp ()));
        tell_object (TO, sprintf ("Your pk nemesis is now %s\n",
                                  TO->query_pk_nemesis ()));
      }
      chance = experience / 5;
      experience -= chance;
    }

    if (living (source) && living (this_object ()) &&
        !this_object ()->query_construct () && !source->query_construct () &&
        !is_npc && !source->query_npc () &&
        !environment (this_object ())->query_is_arena ())
    {
      //Log a kill for the killer
      RANGE_DAEMON->add_kill(source);
      //Log the victim as a PK Martyr
      HUNT_DAEMON->add_martyr(this_object(), source);
      //End the victim's PK session
      RANGE_DAEMON->remove_session(this_object());

      // We don't want wizards or testers affecting politics.  Apparently.
      // We also don't want bots affecting politics on either side.
      if (testerp (this_object ()) || testerp (source) ||
          this_object ()->query_is_registered_bot () ||
          source->query_is_registered_bot ())
      {
      }
      else
      {
        // Politics change, if necessary.
        // Only changes Pact or Ally to War.  Does not affect Neutral or lower.
        guilds [0] = TO->query_guild () || "none";
        guilds [1] = source->query_guild () || "none";

        if (guilds [0] == "Fremen" && TO->query_sub_guild () != "none")
          guilds [0] = this_object ()->query_sub_guild ();
        if (guilds [1] == "Fremen" && source->query_sub_guild () != "none")
          guilds [1] = source->query_sub_guild ();

        guilds [0] = POLD->get_guild_name (guilds [0]);
        guilds [1] = POLD->get_guild_name (guilds [1]);
        polstat [0] = POLD->query_political_status (guilds [0], guilds [1]);

        if (polstat [0] > 3 || polstat [1] > 3)
        {
          if (polstat [1] <= 3)
            HEARER->listen (sprintf ("%s violates the trust of the %s and murders %s!\n",
                                     capitalize (source->query_real_name ()),
                                     POLD->get_full_guild (guilds [0]),
                               sprintf ("%s murdering %s.", source->query_real_name (), TO->query_real_name ()),
                               1);
        if (polstat [1] > 3)
          POLD->change_stance (guilds [1],
                               guilds [0],
                               1,
                               sprintf ("%s murdering %s.", source->query_real_name (), TO->query_real_name ()),
                               1);

        //Old hunt/vow logic replaced - Paradox 06/16/10
        string attackerGuild = source->query_guild();
        string defenderGuild = this_object()->query_guild();

        //The attacker has vowed revenge against TO's guild
        if (HUNT_DAEMON->is_vowed_against(source, defenderGuild))
        {
          HEARER->listen (sprintf ("%s carried out their vow of revenge against the %s.\n",
                capitalize(source->query_real_name()),
                POLD->get_full_guild(defenderGuild)));

          HUNT_DAEMON->remove_vow(source, defenderGuild);
        }
        //The attacker has vowed revenge against TO specifically
        else if(HUNT_DAEMON->is_vowed_against(source, this_object()))
        {
          HEARER->listen (sprintf ("%s carried out their vow of revenge against %s.\n",
                capitalize(source->query_real_name()),
                capitalize(this_object()->query_real_name())));

          HUNT_DAEMON->remove_vow(source, this_object());
        }

                                     pk_exp));
        source->set_pk_best_xp (pk_exp);
        source->set_pk_best_kill (capitalize (TO->QRN));
      }

      if (!assassinated && interactive (TO)) // no +pkills for the Assassin. -Ravien 02Sep05
      {
        source->add_pk_kill ();
        catch (PK_DEATH_DAEMON->add_kill (source, this_object ())); // Moved from player.c -Ravien 02Sep05
      }
    }

    if (playerp (source) && !playerp (TO)) // best monsterkill
    {
      if (TO->query_exp () > source->query_best_xp ())
      {
        tell_object (source, sprintf("This is your best kill yet! [%s at %d experience]\n",
                                     TO->short (1),
                                     TO->query_exp()));
        source->set_best_xp (TO->query_exp ());
        source->set_best_kill (TO->short (1));
      }
      source->add_kill ();
    }

    if (objectp (source))
      catch (place_objects ("/d/Events/Slot_Machine/token", random (3) + 1, corpse));

    corpse->set_name (name); // triggers the heartbeat! should be last
    catch (transfer_all_to (corpse));

    if (query_no_corpse ())
    {
      destruct (corpse);
      if (playerp (this_object ()))
        remove_property ("no_corpse");
    }
    else
      move_object (corpse, environment ());

    // Tell guild souls when a monster dies  Boss & Varrack  070801
    if (is_npc && playerp (who_killed) && who_killed->query_guild_soul ())
      catch ((who_killed->query_guild_soul ())->do_monster_kill (TO));

    // Tell a dead player's gsoul that they died.
    if (!is_npc && playerp (this_object ()))
      catch (this_object ()->query_guild_soul ()->process_death (who_killed, ""));

    // Tell things that care when they kill something.
    if (function_exists ("do_kill", who_killed))
      catch (who_killed->do_kill (TO, corpse));


  for (int i = 0; i < 10; i ++)
    extra_attack_rate [i] = extra_attacks / 10 + m_extra_attacks [extra_attacks % 10, i];
}

void set_attack_rate (int some)
{
  int i;

  if (!attack_rate)
    attack_rate = allocate (10);

  some = clip (some, 0, 10);
  for (i = 0; i < 10; i ++)
    attack_rate [i] = (i <= 9 - some) ? 1 : 2;
}

// Shows given combat messages to everyone with the given level of briefme
// standing in the room of this object.
// Ownmsg is shown to this object, attmsg to at, roommsg to everyone else.
// Returns amount of players that did not get the message and
// have a lower level of briefme.
// If flag is 1, the message is also given to lower briefme levels.
// - Elmer.
// Mreh, 2016-01-26
// Until a better solution can be found, misses are being broadcast at the highest brief level, with flag set, and ignoring peoples' gag settings.
varargs int spread_combat_message (string ownmsg, string attmsg, string roommsg, int brief_level, object at, int flag)
{
  object *players;
  int x, count;

  if (!this_object ())
      return 0;

  players = filter(users(), (:environment($1)==environment(TO):));
  // replaced below line with this, more optimal, one. - Math Oct 2020
  //  players = filter (all_inventory (environment (TO)), #'playerp);  // TP changed to TO.  Varrack 083003  '

  for (x = 0; x < sizeof (players); x ++)
  {
    if (players [x]->query_brief_combat () == brief_level ||
        (intp (players [x]->query_brief_combat ())) &&
        (flag && players [x]->query_brief_combat () < brief_level))
    {
      if (players [x] == TO)
      {
        if (!players [x]->query_npc ())
          tell_object (players [x], ownmsg, flag ? ({ }) : ({ "my", "damage", "my damage", "success" }) );
      }
      else
      {

void generate_combat_messages (int damage_total, mixed *weapon, string my_name, string its_name, object target, string where)
{
  string what, how, attack_colour, ownmsg, attmsg, roommsg;

  if (!this_object ())
    return;

  switch (damage_total)
  {
    case 1 .. 50:
      attack_colour = ANSI (green);
      break;
    case 51 .. 125:
      attack_colour = ANSI (cyan);
      break;
    case 126 .. 225:
      attack_colour = ANSI (brown);
      break;
    case 226 .. 500:
      attack_colour = ANSI (purple);
      break;
    default:
      attack_colour = ANSI (red);
  }

  // Brief messages.  For players that hate text.
  ownmsg = sprintf ("%sYou hit %s.$OFF\n",
                    attack_colour,
                    its_name);
  roommsg = sprintf ("%s%s hits %s.$OFF\n",
                     attack_colour,
                     my_name,
                     its_name);
  attmsg = sprintf ("%s%s hits you.$OFF\n",
                     attack_colour, my_name);

  spread_combat_message (ownmsg, attmsg, roommsg, 1, target);

  // Long-form messages from here on out.
  // This block exists so that guilds can define their own hit messages.
  if (this_object () && this_object ()->query_guild_soul () && this_object ()->query_guild_soul ()->query_combat_messages ())
  {
    mixed messages = this_object ()->query_guild_soul ()->query_combat_messages ();
    string message;

    if (!stringp (messages) && sizeof (messages))
      message = pickone (messages);
    else
      message = messages;

    ownmsg = subst (message, "$WHOP", attacker_ob->query_possessive ());
    ownmsg = subst (ownmsg, "$WHO", attacker_ob->QN);
    ownmsg = subst (ownmsg, "$MEP", "your");
    ownmsg = subst (ownmsg, "$ME", "You");
    ownmsg = subst (ownmsg, "$S", "");

    attmsg = subst (message, "$WHOP", "your");
    attmsg = subst (attmsg, "$WHO", "you");
    attmsg = subst (attmsg, "$MEP", TO->query_possessive ());
    attmsg = subst (attmsg, "$ME", TO->QN);
    attmsg = subst (attmsg, "$S", "s");

    roommsg = subst (message, "$WHOP", attacker_ob->query_possessive ());
    roommsg = subst (roommsg, "$WHO", attacker_ob->QN);
    roommsg = subst (roommsg, "$MEP", TO->query_possessive());
    roommsg = subst (roommsg, "$ME", TO->QN);
    roommsg = subst (roommsg, "$S", "s");

    spread_combat_message (ownmsg, attmsg, roommsg, 0, target);
    return;
  }

  // Now messages based on the weapon type.
  string *attack_message;
  string combat_type;
  string weapon_name;

  if (!weapon [0]) // If we aren't wielding anything, use your limbs.
  {
    weapon_name = pickone ( ({ "left hand", "left foot", "right hand", "right foot", "head" }) );

    if (strstr (weapon_name, "hand") > -1)
      combat_type = "HANDS";
    else if (strstr (weapon_name, "foot") > -1)
      combat_type = "LEGS";
    else
      combat_type = "HEAD";
  }
  else            // Check to see that there are hit messages for the weapon type, otherwise use default.
  {
    weapon_name = weapon [0]->query_name ();
    combat_type = query_hit_mess (weapon [2], 0) ? weapon[2] : "DEFAULT";
  }

  // Just in case, combat can be bland.
  if (!weapon_name || !stringp (weapon_name))
    weapon_name = "weapon";
  if (!combat_type || !stringp (combat_type))
    combat_type = "DEFAULT";

  // If the weapon has defined hit messages, we use those instead of the defaults.
  if (weapon [0] && weapon [0]->query_att_mess ())
  {
    attack_message = weapon [0]->query_att_mess ();
    what = attack_message [0];
    how = attack_message [1];

    switch (damage_total)
    {
      case 1 .. 20:
        what = attack_message [12];
        how = attack_message [13];
        break;
      default:
        what = attack_message [2];
        how = attack_message [3];
    }
  }
  else
  {
    what = query_hit_mess (combat_type, 0);
    how = query_hit_mess (combat_type, 1);

    switch (damage_total)
    {
      case 1 .. 20:
        what = query_hit_mess (combat_type, 16);
        how = query_hit_mess (combat_type, 17);
        break;
      case 21 .. 50:
        what = query_hit_mess (combat_type, 14);
        how = query_hit_mess (combat_type, 15);
        break;
      case 51 .. 80:
        what = query_hit_mess (combat_type, 12);
        how = query_hit_mess (combat_type, 13);
        break;
      case 81 .. 125:
        what = query_hit_mess (combat_type, 10);
        how = query_hit_mess (combat_type, 11);
        break;
      case 126 .. 175:
        what = query_hit_mess (combat_type, 8);
        how = query_hit_mess (combat_type, 9);
        break;
      case 176 .. 225:
        what = query_hit_mess (combat_type, 6);
        how = query_hit_mess (combat_type, 7);
        break;
      case 226 .. 500:
        what = query_hit_mess (combat_type, 4);
        how = query_hit_mess (combat_type, 5);
        break;
      default:
        what = query_hit_mess (combat_type, 2);
        how = query_hit_mess (combat_type, 3);
    }
  }

  // Just in case one of these things gets un-set or not-set, let's be basic.
  if (!attack_colour)
    attack_colour = ANSI (normal);
  if (!what || !stringp (what))
    what = "hurt";
  if (!its_name || !stringp (its_name))
    its_name = "it";
  if (!where || !stringp (where))
    where = "head";
  if (!how || !stringp (how))
    how = "hard";
  if (!weapon_name)
    weapon_name = "weapon";

  // Now that we've pieced messages together, let's tell everyone.
  // This is done differently than below because we're seeing "(s)printf(): incorrect argument type to %s." in this region.
  string message1 = sprintf ("%sYou %s %s's %s%s with your %s.$OFF\n", attack_colour, what, its_name, where, how, weapon_name);
  string message2 = sprintf ("%s%s %s your %s%s.$OFF\n", attack_colour, my_name, plural (what), where, how);
  string message3 = sprintf ("%s%s %s %s's %s%s.$OFF\n", attack_colour, my_name, plural (what), its_name, where, how);

  spread_combat_message (message1, message2, message3);
}

//Gets the base damage for abilities based on a specific stat and skill
//Leondal - 04/2019
varargs int get_base_damage(int class_factor, int stat_factor, int skill_factor)
{
    int damage_total;

    damage_total = to_int (ceil (class_factor *
                               (1 + max (1, skill_factor) / 100.0) *
                               pow (1.002875, stat_factor)));

    return damage_total;
}


void update_attack_rate ()
{
  //Give a bonus if fighting unarmed
  if(!wep1 && !wep2)
      quick_check = quick_check * 5 / 4; //25% more attacks

  set_attack_rate (quick_check);
}

void find_new_opponent ()
{
  if (attacker_ob)
    return;

  alt_attacker_ob -= ({ 0 });

  object *here = filter (alt_attacker_ob, (: environment ($1) == environment (this_object ()) :) );

  if (sizeof (here))
  {
    attacker_ob = pickone (here);
    alt_attacker_ob -= ({ attacker_ob });
  }

  if (!attacker_ob)
    return;

  if (attacker_ob && this_object ())
    return 0;

  if (!pointerp (hunted))
    hunted = ({ });
  if (!pointerp (hunter))
    hunter = ({ });

  if (!name_of_weapon)
    name_of_weapon = ({ 0, 0, 0 });   // ({ weapon object, weapon class, weapon type })  -- from stuff.c
  if (!name_of_weapon2)
    name_of_weapon2 = ({ 0, 0, 0 });  // ({ weapon object, weapon class, weapon type })  -- from stuff.c

  hunted -= ({ 0 });
  hunter -= ({ 0 });

  // If optional_attacker is set, we start attacking that thing with priority.
  if (optional_attacker)
  {
    if (attacker_ob)
      alt_attacker_ob += ({ attacker_ob });
    attacker_ob = optional_attacker;
  }

  if (!attacker_ob)
    find_new_opponent ();

  if (!attacker_ob) // We don't have anything to attack.  Short circuit here and skip a bunch of madness.
    return 0;

  if (environment (attacker_ob) != environment (this_object ()))
  {
    attacker_ob->start_hunting (this_object ());
    start_hunted (attacker_ob);

    attacker_ob = 0;
    find_new_opponent ();
  }

  if (!attacker_ob)
    return 0;

  its_name = attacker_ob->query_name () || "it";

  //This assigns our current style by what we're wielding
  if(name_of_weapon[0])
  {
      if(!name_of_weapon2[0])
          fighting_style = 1;
      if(name_of_weapon[0] == name_of_weapon2[0])
          fighting_style = 2;
      if(name_of_weapon2[0] && name_of_weapon[0] != name_of_weapon2[0])
          fighting_style = 3;
  }

  if (!this_object ())
    return 0;

  if(!sizeof(my_values))
      my_values = this_object()->get_combat_stats();
  if(sizeof(your_values))
      your_values = attacker_ob->get_combat_stats();

  if(get_eval_cost() < 100000)
      return 0;

  update_attack_rate ();

  //Leondal changed this next section to follow these rules - 07/2019
  //0 Unarmed style hits once or twice (depending on skill) with fists
  //1 1-handed style hits once with more precision
  //2 2-handed style hits once for more damage
  //3 2-weapon style hits twice with penalties
  switch(fighting_style)
  {
      case 0:
      attack_with_weapon( ({ 0, query_unarmed_wc(), "Hand to Hand" }), attacker_ob, 0);
      //If they happen to have a weapon in their offhand with none mainhand
      if(name_of_weapon2[0] && this_object() && attacker_ob)
          attack_with_weapon(name_of_weapon2, attacker_ob, 3); //Offhand hit gets penalty
      //If no offhand weapon, hit with other fist with no penalty. Minimum skill level required
      if(attacker_ob && this_object() && !name_of_weapon2[0] && this_object()->query_skills("Hand to Hand") > 500)
          attack_with_weapon( ({ 0, query_unarmed_wc(), "Hand to Hand" }), attacker_ob, 0);
      break;
      case 1..2:
      attack_with_weapon(name_of_weapon, attacker_ob, fighting_style);
      break;
      case 3:
      //If they are dual wielding, attempts hit with mainhand, then offhand, with penalty to both.
      attack_with_weapon(name_of_weapon, attacker_ob, fighting_style);
      this_object() && attacker_ob && attack_with_weapon(name_of_weapon2, attacker_ob, fighting_style);
      break;
  }

  // Again, we might die from a counter-attack before executing any more code.
  if (!this_object())
    return 0;

  if (!attacker_ob || attacker_ob->query_dead ()) // Enemy died.
  {
    if (this_object () && !is_npc && (this_object ()->query_death_message () == "" || this_object ()->query_death_message () == 0))
      tell_object (this_object (), sprintf ("%s died.\n", capitalize (its_name)));

    find_new_opponent ();
  }

  return objectp (attacker_ob);
}

int attack_object (object ob)
{
  if (!alt_attacker_ob)
    alt_attacker_ob = ({ });
  if (!(this_object()->can_kill (ob, 0)))
    return 0;

  set_heart_beat (1);           /* For monsters, start the heart beat */

  if (ob->query_npc ())
    ob->set_brief_combat (TO->query_briefingallmon ());

  if (is_npc)
    TO->set_brief_combat (ob->query_briefingallmon ());

  if (attacker_ob == ob)
  {
    TO->attack ();
    return 1;
  }

  if (member (alt_attacker_ob, ob) != -1)
  {
    alt_attacker_ob [member (alt_attacker_ob, ob)] = attacker_ob;
    attacker_ob = ob;
    TO->attack ();
    return 1;
  }

  if (member (alt_attacker_ob, ob) == -1)
    if (attacker_ob)
      alt_attacker_ob += ({ attacker_ob });

  //If the thing we are attacking is a player, and we're a bigger level than them
  //then we are 'antagonizing' them as far as the PK range system is concerned
  //and we must note that.
  if(playerp(ob) && ob->query_level() > this_object()->query_level())
  {
    //We also want to make sure we only add the entry if
    //we haven't already added it AND the room the players are fighting
    //in isn't free-kill (i.e. can_kill_environment returns 2). Otherwise
    //fighting someone in factions could get you killed later
    if(!RANGE_DAEMON->has_attacked(this_player(), ob) &&
       can_kill_environment(ob, 1) != 2)
      RANGE_DAEMON->add_antagonized(this_player(), ob);
  }

  attacker_ob = ob;
  attacker_ob->attacked_by (TO);
  if (TO)
    hunted_names -= ([ ob->QRN ]);
}

void start_hunted (object who)
{
  if (!objectp (who))
    return;

  if (!pointerp (hunter))
    hunter = ({ });

  if(!mappingp(hunted_names))
    hunted_names=([]);

  if (member (hunter, who) == -1)
  {
    if (playerp (TO))
      tell_object (TO, sprintf ("$REDYou are now hunted by %s.$OFF\n", who->QN));
    hunter +=({ who });
  }

  /* Ravien 19Dec2011 If we become hunted by a huntee, that takes precedence. */
  if (!playerp(TO) && playerp(who) && member (hunted_names,who->QRN))
    hunted_names -= ([ who->QRN ]);

  if (pointerp (hunted))
  if (member (hunter, ob) != -1)
  {
    if (playerp (TO))
      tell_object (TO, sprintf ("$GREEN%s is no longer hunting you.$OFF\n", ob->QN));
    hunter -= ({ ob });
  }
}

void set_hlevel(int i)
{
    int pre_max;

    if(!intp(i))
        return;

    hlevel = max(i, 8);
    init_hp_sp_table();
    i = Con + Con_adj;

    pre_max = (i > 108 ? (hp_sp_table [108] * (100 + level - 108) / 100) : hp_sp_table [i - 1]);

    if(is_npc)
        max_hp = ( pre_max * MONSTER_HP_MOD ) / 100;
    else
        max_hp = 48 + level + ( ( Con + Con_adj ) * ( 10 + hlevel + H_adj ) );

    if(function_exists("calc_xp"))
        calc_xp();
}


void set_slevel (int i)
{
  if (!intp (i))
    return;
  if (i <= 0)
    i = 8;
  slevel = i;

  init_hp_sp_table ();
  i = Int + Int_adj;

  if (is_npc)
    max_sp = (i > 108 ? (hp_sp_table [108] * (100 + level - 108) / 100) : hp_sp_table [i - 1]);
  else
  {
    if (!query_slevel ())
      max_sp = 42 + level + ((Int + Int_adj) * 8);
    else
      max_sp = 42 + level + ((Int + Int_adj) * (slevel + C_adj));
  }

  if (function_exists ("calc_xp"))
    calc_xp ();
}

void stat_log (object who, string stat, int pre, int post)
{
  log_file ("SET_STATS", sprintf ("%s tried setting %s(%d)'s %s from %d to %d at %s.\n",
                                  who->query_real_name () || object_name (who),
                                  TO->query_real_name () || object_name (TO),
                                  TO->query_level (),
                                  stat,
                                  pre,
                                  post,
                                  ctime ()));
}

int set_int (int i)
{
#if 0
  stat_log (this_interactive () || previous_object (), "int", Int, i);
#endif

  if (TP && TP != TO && !TO->is_test_player () && !slordp (TP) && wizardp (TP))
  {
    write("You are struck by a mental bolt of lighting from the game driver.\n");
    stat_log (TP, "int", Int, i);
    return 1;
  }

  Int = clip (i, 1, 1000);

  if (!is_npc)
  {
    if (!query_slevel ())
      max_sp = 42 + level + ((Int + Int_adj) * 8);
    else
      max_sp = 42 + level + ((Int + Int_adj) * slevel);
  }
  else
  {
    int x;
    init_hp_sp_table ();
    x = Int + Int_adj;
    max_sp = (x > 108 ? (hp_sp_table [108] * (100 + level - 108) / 100) : hp_sp_table [x - 1]);
    max_sp = max_sp * 4 / 5 + random (max_sp * 2 / 5);

    if (is_npc && function_exists ("calc_xp"))
      calc_xp ();
  }
}

int set_con (int i)
{
#if 0
  stat_log (this_interactive () || previous_object (), "int", Int, i);
#endif
  stat_log (this_interactive () || previous_object (), "dex", Dex, i);
#endif

  if (TP && TP != TO &&  !TO->is_test_player () && !slordp (TP) && wizardp (TP))
  {
    write("You are struck by a mental bolt of lighting from the game driver.\n");
    stat_log (TP, "dex", Dex, i);
    return 1;
  }

  Dex = clip (i, 1, 1000);

  if (is_npc && function_exists ("calc_xp"))
    calc_xp ();
}

int set_wis (int i)
{
#if 0
  stat_log (this_interactive () || previous_object (), "wis", Wis, i);
#endif

  if (TP && TP != TO && !TO->is_test_player () && !slordp (TP) && wizardp (TP))
  {
    write("You are struck by a mental bolt of lighting from the game driver.\n");
    stat_log (TP, "wis", Wis, i);
           c);
}

int aim_at(string str)
{
    object enemy;
    string *options;

    if (!str)
    {
        printf ("You are aiming at %s\n", ((aim == "random") ? "random" : (member (LOCA_CORR, aim) ? LOCA_CORR [aim] : aim)));
        return 1;
    }

    if(str == "random")
    {
        aim = "random";
        printf("%sYou are now aiming at : %srandom%s.\n", RED, CYAN, OFF);
        return 1;
    }

    if(objectp(attacker_ob))
        options = m_indices(attacker_ob->query_limbs());
    else
        options = m_indices(LOCATIONS_MAP);

    printf ("%s %s is %s.\n",me ? "Your" : CAP (possessive (TO)), member (LOCA_CORR, ind) ? LOCA_CORR [ind] : ind, tmp);
  }
}

// Plural functions for the hit messages :) DK
string plural (string arg)
{
  string modifier;
  if (!arg)
    return "";

  switch (arg [<1])
  {
    case 'h':   modifier = "es"; break;
    case 'x':   modifier = "es"; break;
    default:    modifier = "s";  break;
  }
  return sprintf ("%s%s", arg, modifier);
}

string query_hit_mess (string c_type, int num)
{
  if (!mappingp (HIT_MESS))
    HIT_MESS = copy (WEAPM->query_map ());
  return (num >= 0 && num < 18) ? HIT_MESS [c_type, num] : "";
}

int add_combat_message (string type, mapping map)
{
 /* This function allows new combat types and messages to
  * be added; by adding a current type, messages are overwritten
  * note: must add one at a time!
  *   -Stryder 071500
  */
  if (!mappingp (HIT_MESS))
    HIT_MESS = copy (WEAPM->query_map ());

  if (!type || !stringp (type))
    raise_error ("Bad argument 1 to add_combat_message.\n");

  if (!mappingp (map) || !sizeof (m_indices (map)))
    raise_error ("Bad argument 2 to add_combat_message.\n");

  if (get_type_info (map, 1) != 18)
    map = m_reallocate (map, 18);

  for (int x = 0; x < 18; x ++)
    if (!stringp (map [type, x]))
      map [type, x] = "";

  m_delete (HIT_MESS, type);
  HIT_MESS += map;
  return 1;
}

int remove_combat_message (string type)
{
 /* Removes combat messages that have been added and returns to
  * default; this function CANNOT be used to eliminate a default
  * combat type.
  *   -Stryder 071500
  */
  if (!type || !stringp (type))
  {
    raise_error("Bad argument 1 to remove_combat_message.\n");
    return 0;
  }

  if (!mappingp (HIT_MESS) || !member (HIT_MESS, type))
    return 0;

  m_delete (HIT_MESS, type);
  if (member (WEAPM->query_map (), type)) // it's a default type
  {
    if (get_type_info (HIT_MESS, 1) != 18)
      HIT_MESS = m_reallocate (HIT_MESS, 18);
        for (int x = 0; x < 18; x ++)
          HIT_MESS [type, x] = stringp (WEAPM->query_map ()[type, x]) ? WEAPM->query_map ()[type, x] : "";
  }

  return 1;
  if (sscanf (object_name (E), "d/Harko/faction/area/%s", tmp) == 1)
    return 1;

  if (!objectp (source))
    source = this_player () || this_object ();

  if (!living (source))
    return 1;

  if (pointerp (TO->query_property ("no_lose_limb")) && member (TO->query_property ("no_lose_limb"), where) != -1)
    return 1;

  if (limbs [where] == 0)
    return 1;

  limbs [where] = 0;
  if (where == "l_arm")
    limbs ["l_hand"] = 0;
  if (where == "r_arm")
    limbs ["r_hand"] = 0;
  if (where == "r_leg")
    limbs ["r_foot"] = 0;
  if (where == "l_leg")
    limbs ["l_foot"] = 0;

  if (!is_npc)
    {
      if (ob [i]->id (aid_list [x]))
      {
        tell_object (TO, "You jump in to aid " + ob [i]->QN + "!\n");
        tell_object (ob [i], QN + " jumps to your aid!\n");
        tell_room (E, QN + " jumps in to aid " + ob [i]->QN + "!\n", ({ TO, ob [i] }));
        attack_object (enemy);
        return 1;
      }
    }
  }
}

/*
  d100 Combat System

  -- Leondal -- 2020

  Removes player defense code completely. The outcome of the attack is determined by a d100
  roll on an attack table, much like what modern MMO games use. Each outcome gets a piece
  of a pie that can grow or shrink depending on combat bonuses/penalties. Also, certain
  outcomes can be "pushed off" the table if certain stats get large enough.

  For instance, if a player has a 5% Miss Chance, 30% dodge chance, 30% block chance, 35%
  chance, that means that the players is, essentially, uncrittable.

{
    mapping values;

    object *equipment;

    float dodge_chance,
          guild_defense_chance,
          block_chance,
          crit_chance,
          passive_reduction;

    int block_rating,
        defense_rating,
        attack_rating;

    //This section gets a snapshot of combat stat bonuses from equipment
    equipment = all_inventory(this_object());

    //Gotta narrow down the items we get stats from.
    equipment = filter(equipment, (: base_file($1) != "obj/junk" && base_file($1) != "obj/socketjunk" :));
    equipment = filter(equipment, (: !($1->query_is_socket()) :));
    equipment = filter(equipment, (: $1->query_worn() || $1->query_wielded() || $1->query_guild_soul() :));

    bonus_dodge_chance         = 0;
    bonus_block_chance         = 0;
        bonus_block_rating         = 0;
    bonus_passive_reduction    = 0;

    //This gets equipment bonuses to combat stats
    if(sizeof(equipment))
    {
        foreach(object ob : equipment)
        {
            add_bonus_block_rating(ob->query_bonus_block_rating());
            add_bonus_block_chance(ob->query_bonus_block_chance());
            add_bonus_dodge_chance(ob->query_bonus_dodge_chance());
            add_bonus_crit_chance(ob->query_bonus_crit_chance());
            add_bonus_crit_damage(ob->query_bonus_crit_damage());
            add_bonus_hit_chance(ob->query_bonus_hit_chance());
            add_bonus_defense_rating(ob->query_bonus_defense_rating());
            add_bonus_attack_rating(ob->query_bonus_attack_rating());
            add_bonus_guild_defense_chance(ob->query_bonus_guild_defense_chance());
            add_bonus_fast_healing(ob->query_bonus_fast_healing());
            add_bonus_fast_recovery(ob->query_bonus_fast_recovery());
            add_bonus_passive_reduction(ob->query_bonus_passive_reduction());
        }
    }

    dodge_chance = max(5.00, to_float(this_object()->query_skills("Dodge")) / 100) + to_float(this_object()->query_dex()) / 50;
    dodge_chance += min(10.00, this_object()->query_skills("Roll") / 200);
    dodge_chance += to_float(bonus_dodge_chance);
    block_chance = max(5.00, to_float(this_object()->query_skills("Block")) / 100);
    block_chance += min(10.00, this_object()->query_skills("Roll") / 200);
    block_chance += to_float(bonus_block_chance);
    guild_defense_chance = min(10.00 + to_float(bonus_guild_defense_chance), 30.00);
    guild_defense_chance += min(10.00, this_object()->query_skills("Roll") / 200);

    block_rating = 10 + this_object()->query_str() + this_object()->query_skills("Block") / 10;
    block_rating += bonus_block_rating;
    block_rating = max(block_rating, 0);

    crit_chance = 5.00 + to_float(this_object()->query_dex()) / 50;
    crit_chance += to_float(bonus_crit_chance);
    crit_chance = max(crit_chance, 0);

    //Defense rating will add to all defensive stats
    defense_rating = 10 + this_object()->query_level() + bonus_defense_rating;
    defense_rating = max(min(defense_rating, DEFENSE_CAP), 0);

    if(this_object()->query_guild_soul())
        guild_defense_chance += ( guild_defense_chance >= 20 ? to_float(defense_rating) / 100 : to_float(defense_rating) / 150 );
    else
        guild_defense_chance == 0;

    //Some diminishing returns on defense_rating
    dodge_chance += ( dodge_chance >= 20 ? to_float(defense_rating) / 100.00 : to_float(defense_rating) / 150.00 );
    block_chance += ( block_chance >= 20 ? to_float(defense_rating) / 100.00 : to_float(defense_rating) / 150.00 );
    passive_reduction = max(min(to_float(bonus_passive_reduction) + ( to_float(defense_rating) / 10 ), 90.00), 0.00);

    dodge_chance = to_float(max(min(dodge_chance, DODGE_CAP), 0));
    block_chance = to_float(max(min(block_chance, BLOCK_CAP), 0));
    guild_defense_chance = to_float(max(min(guild_defense_chance, GUILD_CAP), 0));

    //Attack rating adds a flat amount to normal attacks
    attack_rating = bonus_attack_rating;

    values = ([
               "dodge_chance"        : dodge_chance,
               "block_chance"        : block_chance,
               "crit_chance"         : crit_chance,
               "bonus_crit_damage"   : bonus_crit_damage,
               "block_rating"        : block_rating,
               "bonus_hit_chance"    : to_float(bonus_hit_chance),
               "defense_rating"      : defense_rating,
               "attack_rating"       : attack_rating,
               "guild_defense_chance": guild_defense_chance,
               "passive_reduction"   : passive_reduction,
               "fast_healing"        : fast_healing + bonus_fast_healing,
               "fast_recovery"       : fast_recovery + bonus_fast_recovery,
             ]);

    return values;
}

int get_combat_value(string str)
{
    if(!sizeof(my_values))
        my_values = get_combat_stats();

    if(!member(my_values, str))
        return 0;

    return copy(my_values)[str];
}

void attack_with_weapon(mixed *weapon, object target, int fighting_style)
{
    float dodge_chance,
          guild_defense_chance,
          block_chance,
          crit_chance,
          miss_chance,
          passive_reduction,
          roll;


    string my_name,
           its_name,
           weapon_skill_name;

    if(!target || !this_object() || !living(this_object()))
        return;

    if(!sizeof(my_values))
        my_values = this_object()->get_combat_stats();
    if(!sizeof(your_values))
        your_values = target->get_combat_stats();

    my_name = this_object ()->query_name ();
    its_name = objectp (target) ? target->query_name () : "it";
    weapon_skill_name = weapon[2];
    weapon_skill = this_object ()->query_skills (weapon[2]);

    miss_chance = 5.00 + to_float(target->query_level() - this_object()->query_level()) / 5;

    switch(fighting_style)
    {
      //1 1-handed weapon - Has more chance to hit
      case 1:
      miss_chance += 0.00;
      break;
      //1 2-handed weapon - Hits harder with each hit
      case 2:
      miss_chance += 3.00;
      break;
      //2 1-handed weapons - Has lower chance to hit (mitigated by Offhand Fight skill)
      case 3:
      miss_chance += max((20.00 - this_object()->query_skills("Offhand Fight") / 100), 0);
      break;
    }

    //If our level is more than twice our enemy's, we have a chance to hit through defenses
    if(this_object()->query_level() / 3 > target->query_level() && !random(5))
    {
        spread_combat_message (sprintf ("You crush %s.\n", its_name),
                               sprintf ("%s crushes you.\n", my_name),
                               sprintf ("%s crushes %s.\n", my_name, its_name),1, target, 0);
        //damage_total = target->hit_player(this_object()->query_str() + 1, "chest", this_object(), 1);
        target && target->add_hp(-this_object()->query_level() / 3);
        return;
    }

    miss_chance = max(miss_chance - to_float(bonus_hit_chance), 0);
    miss_chance = clip(miss_chance, 1.00, 99.00);

    if(sizeof(your_values))
    {
        dodge_chance = your_values["dodge_chance"];
        block_chance = your_values["block_chance"];
        guild_defense_chance  = your_values["guild_defense_chance"];
    }

        spread_combat_message (sprintf ("%s dodges your attack.\n", its_name),
                           sprintf ("You dodge %s's attack.\n", my_name),
                           sprintf ("%s dodges %s's attack.\n", its_name, my_name),2, target, 0);
        return;
    }

    //Hard cap on WC depending on level. This NEEDs to be controlled
    damage_total = min(weapon[1], WC_CAP);

    //Leondal 04-19-2019
    //Leondal 08-10-2019 - Dex stat affects some weapons instead of str
    if(objectp(weapon[0]) && (weapon_skill_name == "Lasers" ||
       weapon_skill_name == "Bullet weapons" ||
       weapon_skill_name == "Thrown weapons"))
       damage_total = get_base_damage(damage_total, this_object()->query_dex(), weapon_skill);
    else
       damage_total = get_base_damage(damage_total, this_object()->query_str(), weapon_skill);

    //2-handed weapons hit 50% harder
    if(fighting_style == 2)
        damage_total = damage_total * 3 / 2;

    //Add Weapon Hit functions
    if(weapon[0])
        damage_total += weapon[0]->hit(target);

  This is the function that should be used by guild commands that cause damage.
  This treats special attacks a little differently than normal attacks. This also
  allows us to use 'non-physical' attacks or offhand attacks, or even unarmed.

  Special attacks use a normal attack formula for base damage + the dam_mod
  Why? Because the problem in the past has been proper scaling for guild abilities.
  Some did far too much damage, others did far too little. Will set guidelines for
  Special ability damage
*/
varargs int special_attack(object target, int dam_mod, object weapon, int no_block, int penetration)
{
    float dodge_chance,
          passive_reduction,
          guild_defense_chance,
          block_chance,
          crit_chance,
          miss_chance,
          roll;

    int weapon_skill,
        damage_total,
        wc;

    string my_name,
           its_name,
           weapon_skill_name;

    //Soft cap on WC depending on level. This NEEDs to be controlled
    damage_total = min(wc, WC_CAP);

    //Leondal 04-19-2019
    //Leondal 08-10-2019 - Dex stat affects some weapons instead of str
    if(objectp(weapon) && (weapon_skill_name == "Lasers" ||
       weapon_skill_name == "Bullet weapons" ||
       weapon_skill_name == "Thrown weapons"))
       damage_total = get_base_damage(damage_total, this_object()->query_dex(), weapon_skill);
    else
       damage_total = get_base_damage(damage_total, this_object()->query_str(), weapon_skill);

    //Flat damage increase from attack_rating
    damage_total += my_values["attack_rating"];

    //Add the special attack modifier here (passed by ability)
    damage_total += dam_mod;

    if(no_block)
    {
        damage_total = target->hit_player(damage_total, 0, this_object(), no_block, penetration);
        return damage_total;
    }

    //DODGE

    //GUILD DEFENSE
    if(roll < miss_chance + dodge_chance + guild_defense_chance)
    {
        damage_total = target->process_damage_mod(damage_total, this_object());

        if(damage_total <= 0)
            return 0;
    }

    //BLOCK
    else if(roll < miss_chance + dodge_chance + block_chance + guild_defense_chance)
    {
        damage_total -= your_values["block_rating"];
        spread_combat_message (sprintf ("%s blocks your attack.\n", its_name),
                           sprintf ("You block %s's attack.\n", my_name),
                           sprintf ("%s blocks %s's attack.\n", its_name, my_name),1, target, 1);
        target->learn_something ("Block", 1.0, 0, 0);
        if(damage_total <= 0)
            return 0;
    }

    //CRIT
    else if(roll < miss_chance + dodge_chance + block_chance + guild_defense_chance + crit_chance)
    {
        spread_combat_message (sprintf ("You HIT %s [%sCritical%s]!\n", its_name, RED, OFF),
                           sprintf ("%s HITS you [%sCritical%s]!\n", my_name, RED, OFF),
                           sprintf ("%s HITS %s [%sCritical%s]!\n", my_name, its_name, RED, OFF),1, target, 1);
        damage_total *= 2;
        damage_total += bonus_crit_damage;
    }

    //Anything else is a normal hit.

    //PASSIVE REDUCTION
    //Defense Rating adds a passive % reduction

    if(damage_total <= 0 || !this_object() || !target)
        return 0;

    // Added so that "first strike" abilities will actually prevent people from quitting.
    // As it is, quit_wait will only kick in after a full combat round.
    // Math - June 21, 2021.
    if(playerp(target) && playerp(this_object()))
      target->set_quit_wait(1, this_object());

    //Chances to increase skill level
    if (this_object ())
      learn_something (weapon_skill_name, 1.0, 0, 0);
    if (target)
      target->learn_something (weapon_skill_name, 1.0, 0, 0);

    return damage_total;
}

//There was no heart_beat() here so Leondal added one separately for fast_healing, fast_recovery
void heart_beat()
{
    object *equipment,
           attacker;

    int total_healing,
        total_recovery;

    equipment = all_inventory(this_object());
    equipment = filter(equipment, (: $1->query_worn() || $1->query_wielded() :));

    //This gets equipment bonuses to combat stats
    if(sizeof(equipment))
    {
        foreach(object ob : equipment)
        {

            if (++attack_index > 9)
                attack_index = 0;
        }

        if (attacker_ob && bonus_attack)
        {
            //DEBUG(sprintf("bonus attacks\n"));
            //DUMP_VAR(bonus_attack);
            // Seeing if this has an effect on TLE's. Clipping to a max of 30.
            // Some players are over 40 per round.
            // - Orb 2018

            for (int i = 1; i < clip(bonus_attack, 0, 30); i ++)
            {
                if (!this_object ())
                    return;
                if(get_eval_cost() >= 100000)
                    this_object ()->attack ();
            }
        }
    }
}

/*
string *valid_status_effects()
*/


protected void init_hp_sp_table ()
{
  if (!pointerp (hp_sp_table))
    hp_sp_table = ({
          50,     60,     70,     80,     90,    100,    150,    200,    250,    300,     //   0 ..   9
         500,    700,    900,   1100,   1300,   1500,   1700,   1900,   2100,   2500,     //  10 ..  19
        3000,   3500,   4000,   4500,   5000,   5500,   6000,   6500,   7000,   7500,     //  20 ..  29
        8250,   9000,   9750,  10500,  11250,  12000,  13150,  14300,  15450,  16600,     //  30 ..  39
       17750,  18900,  20275,  21650,  23025,  24400,  25775,  27150,  28830,  30510,     //  40 ..  49
       32190,  33870,  35550,  37230,  39070,  40910,  42750,  44590,  46430,  48270,     //  50 ..  59
       50370,  52470,  54570,  56670,  58770,  60870,  60870,  63215,  65560,  67905,     //  60 ..  69
       70250,  72595,  74940,  77390,  79840,  82290,  84740,  87190,  89640,  92355,     //  70 ..  79
       95070,  97785, 100500, 103215, 105930, 108815, 111700, 114585, 117470, 120355,     //  80 ..  89
      123240, 126355, 129470, 132585, 135700, 138815, 141930, 145260, 148590, 151920,     //  90 ..  99
      155250, 161910, 167465, 173020, 178575, 184130, 189685, 202030, 214375              // 100 .. 108
                  });
}

object query_this_weapon ()     {  return this_weapon;  }
#include <living/run_away.h>
