// Monster.c
// General NPC Library File
// For Dune Mud

#include <mudlib.h>
#include <daemons.h>
#include <levels.h>
#include <wizlevels.h>
#include "/obj/player/living.h"
#include <obj/alias.h>
#include <ansi.h>

#define DEBUG_ON
#define DEBUG_TARGET "orbital"
#include <debug.h>

#define MOVE_DAEMON       "obj/daemons/move_daemon"

string  short_desc,       // What the player sees
        long_desc,        // What we look like when you 'examine' us
        race,             // What race are we?
        guild,            // What guild are we?
        the_text,         // Used in catch_tell
        spell_mess1,      // Message to the room
        spell_mess2,      // Message to the player
       *non_combat_mess,  // Array of messages for the mob to use out of combat
       *combat_mess,      // Array of messages for the mob to use in combat
       *stunned_mess,     // Array of messages for the mob to use when stunned
       *talk_func,        // Array of function names
       *talk_match,       // Array of matching strings
       *talk_type,        // Array of types
       *spells,           // Our spells
        create_room;      // Where we originated

static int   fight_rounds;  // Store # of fight rounds
int     move_at_reset,    // Do we move every reset?
        aggressive,       // Are we aggressive?
        non_combat_chance,// Chance of chatting
        combat_chance,    // Chance of emoting
        stunned_chance,   // Chance of stunned chat
        spiced,           // Spice level.
        wait,             // Heartbeat bug fixer
        have_text,        // Do we have text?
        random_pick,      // Chance of picking stuff up
        stuned,           // Are we stunned?
        attack_speed,     // Our attack rate
        hunt_level,       // Level of players we hunt
        no_tank,          // Do we switch attackers?
        spell_chance,     // Chance of casting a spell
        busy_catch_tell,  // Used in catch_tell
        skinable,         // Is the npc skinnable? (I.E animals...)
        bonus_exp,        // We can assign bonus exp
        override_exp,     // We can override the experience function
       *spells_chance,    // Array of spell chances
        healing_rate;     // How many HB till we call auto_heal() ?

object  kill_ob,          // Who we are about to kill
        talk_ob,          // Who we are talking to
        pk_owner;         // Owner of this mob, for pking with aid of mobs

mixed   init_ob,          // Object to call on init()
        agr_guild,        // Guild(s) we are aggressive against
        gift_action,      // Action when a gift is received
        kill_action,      // Action when a kill is made
        death_mess,       // Message to the room upon death
        dead_ob;          // What we should do at death ?

static int auto_heal_count;

private int *hp_sp_table;

void test_match (string str);
void pick_any_obj ();
void set_hp (int n);
void set_sp (int n);
void set_wc (int n);
int  calc_xp ();
void check_old_hunt(string name,int time); // Ravien 19Dec2011 Used for
    walk_mapping'ing hunted players

int     query_fight_rounds()            { return fight_rounds;
                     }
int     query_aggressive ()             {  return aggressive;
                     }
int     query_bonus_exp ()              {  return bonus_exp;
                     }
int     query_healing_rate ()           {  return healing_rate;
                     }
int     query_override_exp ()           {  return override_exp;
                     }
int     query_skinable ()               {  return skinable;
                     }
int     query_spiced ()                 {  return spiced;
                     }
int     query_stuned ()                 {  return stuned;
                     }
int     query_stunned ()                {  return stuned;
                     }

mixed   query_dead_ob ()                {  return dead_ob;
                     }

object  query_pk_owner ()               {  return pk_owner;
                     }

status  can_put_and_get (string str)    {  return stringp (str);
                     }
mixed   query_agr_guild ()              {  return agr_guild;
                     }
string  query_create_room ()            {  return create_room;
                     }
mixed   query_death_message ()          {  return death_mess;
                     }
mixed   query_gift_action ()            {  return gift_action;
                     }
string *query_hitting_limbs ()          {  return hitting_limbs;
                     }
mixed   query_kill_action ()            {  return kill_action;
                     }
int     query_no_tank ()                {  return no_tank;
                     }
string  query_race ()                   {  return race ? lower_case (race) :
    "none";            }
string  query_real_name ()              {  return lower_case (name ||
    "something");             }
string *query_spells ()                 {  return spells;
                     }

void    remove_stuned ()                {  stuned = 0;
                     }
void    remove_stunned ()               {  stuned = 0;
                     }

void    set_al (int n)                  {  deprecate ("set_al", TO);
                     }
void    set_bonus_exp (int amt)         {  deprecate ("bonus_exp", TO);
    bonus_exp = amt / 250; }
void    set_chance (int c)              {  spell_chance = clip (c, 0, 100);
     calc_xp ();     }
void    set_dead (int arg)              {  dead = arg ? 1 : 0;
                     }
void    set_dead_ob (mixed ob)          {  dead_ob = ob;
                     }
void    set_death_mess (mixed m)        {  death_mess = m;
                     }
void    set_desp_move (int x)           {  TO->set_property ("DESP_MOVE", x);
                     }
void    set_ep (int n)                  {  experience = clip (n, 0, experience)
    ;                }
void    set_guild (string s)            {  guild = s;
                     }
void    set_gift_action (mixed act)     {  gift_action = act;
                     }
void    set_healing_rate (int r)        {  healing_rate = r;
                     }
void    set_hunt_level (int lev)        {  hunt_level = lev;
                     }
void    set_init_ob (object ob)         {  init_ob = ob;
                     }
void    set_kill_action (mixed act)     {  kill_action = act;
                     }
void    set_long (string s)             {  long_desc = s;
                     }
void    set_move_at_reset ()            {  move_at_reset = 1;
                     }
void    set_no_tank (int arg)           {  no_tank = arg && 1;
                     }
void    set_override_exp (int amt)      {  override_exp = amt;
                     }
void    set_pk_owner (object player)    {  pk_owner = player;
                     }
void    set_race (string s)             {  race = s;
                     }
void    set_random_pick (int r)         {  random_pick=r;
                     }
void    set_random_move (int r)         {  move_at_reset = r && 1;
                     }
void    set_short (string s)            {  short_desc = s;
                     }
int     set_skinable (int a)            {  skinable = a;
                     }
void    set_spell_mess1 (string m)      {  spell_mess1 = m;
                     }
void    set_spell_mess2 (string m)      {  spell_mess2 = m;
                     }
void    set_spell_dam (int d)           {  spell_dam = d; calc_xp ();
                     }
void    set_stuned (int x)              {  stuned = x;
                     }
void    set_stunned (int x)             {  stuned = x;
                     }
void    set_whimpy ()                   {  whimpy = 1;
                     }
void    set_wimpy (int w)               {  whimpy = w && 1;
                     }

void reset (status arg)
{
  ::reset (arg);
  if (arg)
  {
    if (move_at_reset)
      random_move ();
    return;
  }

  is_npc = 1;
  set_min_light_level (-1);
  enable_commands ();
  name_of_weapon = ({ 0,0,0 });
  name_of_weapon2 = ({ 0,0,0 });
  alias = ({ });

  combat_chance = 0;
  non_combat_chance = 0;
  stunned_chance = 0;
  combat_mess = ({ });
  non_combat_mess = ({ });
  stunned_mess = ({ });

  bonus_attack = 0;
  attack_speed = 1;
  hitting_limbs = DEFAULT_HITTINGS;
  create_room = 0;
  skills = 0;
  healing_rate = 0;
  auto_heal_count = 0;
  ud_cmd_path (1);
  gender = random(3);
  aim = "random";
  defen = random (4);
// to prevent monsters from ever listening to combat messages. -Methos & Mreh
    052007
  set_brief_combat(2);
//    catch("/obj/databases/mob_db/nu_mob_db"->catch_mob_object(this_object()))
    ;// hook into the database - Math 6/27/18
}

status id (string str)
        {  return member ( ({ name, race }), str) != -1 || (pointerp (alias) ?
    member (alias, str) >- 1 : 0);   }

varargs string short (int x)
{
  string str, *shp;
  mixed* hunters;
/*
  DEBUG("ENTER SHORT()\n");
  if (attacker_ob)
    DUMP_VAR(attacker_ob);
  DUMP_VAR(hunted);
  DUMP_VAR(this_object()->query_hunted());
  DUMP_VAR(this_object()->query_hunter());
*/
  if (!str = short_desc)
    return 0;
  if (x)
    return str;

  if (pointerp (shp = INVENTD->compute_shape ()) && shp [0])
    str = sprintf ("%s [%s]", str, shp [0]);

  hunters = this_object()->query_hunter();

  // IN COMBAT TAG (takes precedence)
  if (attacker_ob && ENV(attacker_ob) == E)
    str = sprintf("%s $OFF$BOLD-=$OFF $BLUE%s$OFF $BOLD=-$OFF", str,
    CAP(attacker_ob->QRN));
  // HUNTED TAG
  else if (sizeof (hunters -= ({ 0 })) && playerp (hunters [0]) && ENV
    (hunters [0]) != E)
    str = sprintf ("%s $OFF$BOLD-=$OFF $RED%s$OFF $BOLD=-$OFF", str, CAP
    (hunters [0]->QRN));

  return parse_ansi (str);
}

void long ()
{
  if (!long_desc)
    return;
  if (long_desc [<1] != '\n')
    long_desc = sprintf ("%s\n", long_desc);
  write (parse_ansi (long_desc));
}

int set_skill (string skill, int value)
{
  update_bonus ();
  learn_new_skill (skill, 1);
  skills [skill] = 0;
  return improve_skill (skill, clip (value, 1, 100 + level * 2));
}

mapping make_monster_limbs ()
{
  if (!MONSTER_MAX_HP_MAP)
    MONSTER_MAX_HP_MAP = MAX_HP_MAP;
  return make_limbs ();
}

void set_limbs (mixed limbo)
{
  if (mappingp (limbo))
    MONSTER_MAX_HP_MAP = limbo;
  else if (pointerp (limbo))
  {
    int* k = allocate (sizeof (limbo), 20);
  //int *k;
  //k = allocate (sizeof (limbo));
  //k = map (k, lambda ( ({'ind, 'val }), ({ #'+, 0, 'val })) ,20);
    MONSTER_MAX_HP_MAP = mkmapping (limbo, k);
  }

  limbs = make_monster_limbs ();
  hitting_limbs = m_indices (limbs);
}

int set_hitting_limbs (string *houba)
{
  if (sizeof (houba & m_indices (limbs)) != sizeof (houba))
//if (sizeof (filter (houba, lambda ( ({ 'val, 'limbs }), ({ #'member, 'limbs,
    'val}) ), limbs )) != sizeof (houba))
    return 0;
  hitting_limbs = houba;
  return 1;
}

void set_moving (int arg)
{
  if (arg < 1)
    return;
  if (arg)
    MOVE_DAEMON->add_monster (TO, arg);    // adds to the HB
  else
    MOVE_DAEMON->remove_monster (TO);
}

private void auto_heal ()
{
  hit_point += random (max_hp / (random (200 - random (75 + healing_rate * 2))
    + 1));
  spell_points += random (max_sp / (random (200 - random (75 + healing_rate *
    3)) + 1));

  if (hit_point > max_hp)
    hit_point = max_hp;

  if (spell_points > max_sp)
    spell_points = max_sp;
}

void heal_slowly ()
{
  if (!random (10))
    auto_heal ();
}

void heart_beat ()
{
  int c, x;
  object i;

  object *inv = filter (all_inventory (this_object ()),
                         (: ($1->query_worn () || $1->query_wielded ()) &&
                            $1->query_drain_info () != 0 &&
                            $1->query_drain_info () != ({ }) :));
  mixed *drain;

  if (sizeof (inv))
    foreach (object i : inv)
    {
      drain = i->query_drain_info ();
      if (random (100) <= drain [2])    // Drain triggered.
      {
        if (hit_point < drain [0] || spell_points < drain [1])
          i->set_last_drain (0);        // Drain failed.
        else
        {
          this_object ()->add_hp (-i->query_drain_info () [0]);
          this_object ()->add_cp (-i->query_drain_info () [1]);
          tell_object (this_object (), i->query_drain_info () [3]);
          i->set_last_drain (1);
        }
      }
    }

  stuned = max (stuned, 0);

  // Ravien 03Feb2015. Let's get rid of this mechanic for now until we do it
    right.
  if (0 && (no_tank || !pointerp (alt_attacker_ob)))
  {
    if (!random (20))
    {
      alt_attacker_ob -= ({ 0 });
      alt_attacker_ob = filter (alt_attacker_ob, (: $1 && objectp ($1) &&
    interactive ($1) && present ($1, environment ()) :));
      if (sizeof (alt_attacker_ob))
        i = pickone (alt_attacker_ob);
      else
        return;

      tell_object (i, sprintf ("%s switches and attacks you !!!\n", cap_name));
      tell_room (E, sprintf ("%s switches and attacks %s !!!\n", cap_name,
    i->QN), ({ i }));
      attack_object (i);
      i->attacked_by (TO);
      alt_attacker_ob -= ({ i });
    }
  }

  my_values = this_object()->get_combat_stats();

  if(attacker_ob)
      your_values = attacker_ob->get_combat_stats();

  if (!test_if_any_here ())
  {
    if (have_text && talk_ob)
    {
      have_text = 0;
      test_match (the_text);
    }
    else
    {
      if (!stuned)
        attack ();

      if (hit_point >= max_hp && spell_points >= max_sp)
        set_heart_beat (0);

      return;
    }
  }

  if (!stuned)
  {
    if (kill_ob && ENV (kill_ob) == E)
    {
      attack_object (kill_ob);
      kill_ob = 0;
      return;
    }
  }

  // Give mobs a chance to heal every round.  If they need it.
  if ((hit_point < max_hp || spell_points < max_sp) &&  !(healing_rate /
    (query_attack () ? 1 : 3)))
    auto_heal ();

  // This code needs to be fixed up, preferably by setting a variable of the
    room it belongs in,
  // and then only tracking within 2/3 rooms of it.  Teleporting mobs suck.
  if (hunt_level && attacker_ob)
  {
    if (attacker_ob && ENV (attacker_ob) != E && hunt_level && hunt_level <
    attacker_ob->QL)
    {
      if (ENV (attacker_ob) && !attacker_ob->query_linkdead ())
      {
        tell_room (E, sprintf ("%s runs to hunt %s.\n", cap_name, attacker_ob->
    QN));
        move_object (TO, ENV (attacker_ob));
        if (objectp (attacker_ob) && attacker_ob)
        {
          efun::tell_object (attacker_ob, sprintf ("%s arrives hunting you
    !\n", cap_name));
          tell_room (ENV (attacker_ob), sprintf ("%s arrives hunting %s\n",
                                                 cap_name, attacker_ob->QN),
                     ({ attacker_ob, TO }));
        }
      }
      else
      {
        if (stringp (create_room))
        {
          object room;
          room = find_object (create_room) || load_object (create_room);
          if (objectp (room))
            move_object (TO, room);
        }
      }
    }
  }

  // Rav 2018, let's track # of rounds fought
  if(attacker_ob && ENV(attacker_ob) == E) {
if(!(++fight_rounds%10000)) {
  string logline = sprintf("%s: %O fighting %O (%s) for %d", ctime(), TO,
    attacker_ob, attacker_ob->query_name(),fight_rounds);
  log_file("long_fight",logline);
}
  }

  if (attacker_ob && ENV (attacker_ob) == E && to_int (spell_chance) > random
    (100))
  {
    if (stringp (spell_mess2))
      tell_object (attacker_ob, spell_mess2);
    if (stringp (spell_mess1))
      tell_others (TO, spell_mess1, 0, attacker_ob);
    attacker_ob->hit_player (random (spell_dam), 0);
  }

  if (!stuned && attacker_ob)
  {
    int rate;

    for (c = 0, rate = max (0, attack_speed) + max (0, bonus_attack); c <
    rate; c ++)
    {
      if (!objectp (TO) || !attacker_ob || ENV (attacker_ob) != E ||
    get_eval_cost() < 100000)
        break;
      attack ();
    }
  }

  if (!objectp (TO))   // Iria: just in case we were destructed
  {
    set_heart_beat(0);
    return;
  }

  if (attacker_ob && whimpy && !stuned && percentage (hit_point, max_hp) <=
    whimpy && !random (50))
    run_away ();

  // Say or do stuff.
  string* mess_chats = 0;
  int mess_delay = 0;

  if (stuned) {
    mess_chats = stunned_mess;
    mess_delay = stunned_chance;
  }
  else if (query_attack () && present (query_attack (), environment (this_
    object ())))
  {
    if (!random (50))
      load_object ("/obj/cmds/skills/headbutt")->headbutt ();
    mess_chats = combat_mess;
    mess_delay = combat_chance;
  }
  else {
    mess_chats = non_combat_mess;
    mess_delay = non_combat_chance;
  }

  if (mess_delay && sizeof (mess_chats) && !random (mess_delay))
  {
    mixed rsay = pickone (mess_chats);
    rsay = closurep (rsay) ? funcall (rsay) : (pointerp (rsay) && sizeof
    (rsay) && closurep (rsay[0])) ? apply (rsay[0], rsay[1..]) : rsay;
    rsay = stringp (rsay) ? ({ rsay, "echo", }) : (pointerp (rsay) && (sizeof
    (rsay) > 1) && rsay);

    switch (this_object () && rsay && stringp (rsay[0]) && strlen (rsay[0]) &&
    rsay[1])
    {
      case 0      :  /* nothing. */
                                           break;
      case "call" :  apply (#'call_other, this_object (), rsay[0], rsay[2..]);
                                           break;
      case "clone":  !present(rsay[0]) && move_object(clone_object(rsay[0]),
    this_object());                          break;
      case "dest" :  present(rsay[0]) && destruct(present(rsay[0]));
                                           break;
      case "do"   :  command (rsay[0]);
                                           break;
      case "echo" :  tell_others (this_object (), rsay[0] +((rsay[0][<1] !=
    '\n') ? "\n" : ""));                      break;
      case "null" :  /* deliberately ignored. */
                                           break;
      case "say"  :  command ("say " +rsay[0]);
                                           break;
      case "tell" :
        if (query_attack () && present (query_attack (), environment ()))
          tell_object (query_attack (), rsay[0] +((rsay[0][<1] != '\n') ? "\n"
    : ""));
        break;
      case "emote":  /* no break. */
      default     :  tell_others (this_object (), query_name () +" " +rsay[0]
    +((rsay[0][<1] != '\n') ? "\n" : ""));  break;
    }

    if (!this_object ())
      return;
  }

  // Allow monsters to aid other livings -Stryder 031703
  if (!stuned)
  {
    if (aid_chance > 0 && random (10000) < aid_chance)
      do_aid ();
    if (random_pick > 0 && random (100) < random_pick)
      pick_any_obj ();
  }

  if (have_text && talk_ob)
  {
    have_text = 0;
    test_match (the_text);
  }

  if (!environment ())
    return;

  if (attacker_ob && ENV (attacker_ob) != E)
    wait ++;

  if (wait > 1)
  {
    if (!stuned)
     attack ();
    wait = 0;
  }

  // Benedict 8/11/03
  if (!TO)
    return;

  if (attacker_ob && TO->query_property ("DESP_MOVE") && percentage (hit_point,
     max_hp) < TO->query_property ("DESP_MOVE"))
  {
    efun::tell_object (attacker_ob, sprintf ("%s flees!\n", cap_name));
    tell_room (E, sprintf ("%s flees from %s!\n", cap_name, attacker_ob->query_
    name ()), ({ attacker_ob }));

    if (objectp (TO))
      TO->run_away ();

    if (objectp (TO))
      TO->set_property ("DESP_MOVE",0);
  }

  if (stuned > 0)
    stuned --;

  // Ravien 19Dec2011 We're still here, check if we're hunting someone.
  if(mappingp(hunted_names))
    walk_mapping(hunted_names,#'check_old_hunt);
}

void set_name (string n)
{
  set_living_name (name = n);
  short_desc = (cap_name = capitalize (n));
  long_desc = "You see nothing special.\n";
  set_aid (1, ({ name }) );
}

varargs void set_level (int l, int flag)
{
  string *a;
  int     b,
          c,
          base_exp,
          slevel,
         *default_ac;

  // added this to keep blueprints at a constant level - Math 2018
  if(!clonep(this_object()))
    flag = 1;

  // added this call to the scaler daemon - Math 2018
  if(load_object("/secure/daemons/scalerd"))
    l = "/secure/daemons/scalerd"->get_level(this_object(), "MOB", l);

  level = clip (l, 1, MAX_MOB_LEVEL);
  set_str (flag ? level : level * 4 / 5 + random (level * 2 / 5));
  set_int (flag ? level : level * 4 / 5 + random (level * 2 / 5));
  set_con (flag ? level : level * 4 / 5 + random (level * 2 / 5));
  set_dex (flag ? level : level * 4 / 5 + random (level * 2 / 5));
  set_wis (flag ? level : level * 4 / 5 + random (level * 2 / 5));
  set_qui (flag ? level : level * 4 / 5 + random (level * 2 / 5));
  set_hei (15 - random (11) + random (6));

  hit_point = max_hp;
  spell_points = max_sp;
  bonus_attack = level / (random (13) + 13);
  update_bonus ();

  if (!default_ac)
    default_ac = allocate (15);

  for (b = 0; b < sizeof (default_ac); b ++)
    default_ac [b] = flag ? level / 2 : level * 2 / 5 + random (level * 3 / 5);

  this_object ()->set_default_ac (default_ac);
  recalc_ac ();

  set_wc (3 + level / 3);
  limbs = make_monster_limbs();

  healing_rate = 0;

  // Intentional waterfalling.
  switch (level)
  {
    case 500 .. 1000:   healing_rate ++;
    case 350 ..  499:   healing_rate ++;
    case 250 ..  349:   healing_rate ++;
    case 225 ..  249:   healing_rate += 2;
    case 175 ..  224:   healing_rate += 2;
    case 150 ..  174:   healing_rate += 2;
    case 100 ..  149:   healing_rate += 5;
    case  75 ..   99:   healing_rate += 5;
    case  50 ..   74:   healing_rate += 5;
    case  25 ..   49:   healing_rate += 6;
    case   1 ..   24:   healing_rate += 10;
  }

  // This modifies monster skills to make them more comparable to high player
    skill levels.
  switch (level)
  {
    case   1 ..   24:   slevel = level *  2 / 5;        break;  //  40% of
    level
    case  25 ..   49:   slevel = level *  3 / 5;        break;  //  60% of
    level
    case  50 ..   74:   slevel = level *  3 / 4;        break;  //  75% of
    level
    case  75 ..  124:   slevel = level *  8 / 7;        break;  // 115% of
    level
    case 125 ..  174:   slevel = level *  8 / 5;        break;  // 160% of
    level
    case 175 ..  224:   slevel = level *  7 / 4;        break;  // 175% of
    level
    case 225 ..  249:   slevel = level *  9 / 4;        break;  // 225% of
    level
    case 250 ..  274:   slevel = level *  5 / 2;        break;  // 250% of
    level
    case 275 ..  299:   slevel = level * 10 / 3;        break;  // 333% of
    level
    case 300 ..  499:   slevel = level * 11 / 3;        break;  // 366% of
    level
    case 500 .. 1000:   slevel = level * 13 / 3;        break;  // 433% of
    level
  }

  slevel = clip (slevel * 3 / 4 + random (slevel / 4), slevel / 3, 2500);

  a = SKILLD->query_skills_of_class ("combat");
  foreach (string sk : a)
    set_skill (sk, max (10, (flag ? level : slevel * 4 / 5 + random (slevel *
    4 / 5))));

  a = SKILLD->query_skills_of_class ("defense");
  foreach (string sk : a)
    if (({ "random", "roll", "dodge", "block" })[defen] == sk)
      set_skill (sk, max (10, (flag ? level : slevel * 6 / 5 + random (slevel
    * 2 / 5))));
    else
      set_skill (sk, max (10, (flag ? level : slevel + random (slevel / 5))));

  experience = calc_xp ();
}

// Pretty sure these are already covered in living.c
void set_sp (int n)
{
  max_sp = n;
  spell_points = n;
  calc_xp ();
}

//void set_cp (int n) { set_sp (n); }

void set_hp (int n)
{
  hit_point = max_hp = n;
  limbs = make_monster_limbs ();
  calc_xp ();
}

int calc_xp ()
{
  int calc, MAXXP;

  MAXXP = 1500000000;

  if (hit_point > max_hp)
    hit_point = max_hp;

  if (override_exp > 0)
    return override_exp;

  else if (override_exp < 0)
    return 0;

  switch (level)
  {
    case   1 ..  24:    calc = level *  75 + (Str + Dex + Wis) * 2 + (Con +
    Int + Qui);                 //     75 - 2052
                        break;
    case  25 ..  49:    calc = level * 125 + (Str + Dex + Wis) * 5 + (Con +
    Int + Qui) * 3;             //   3605 - 7517
                        break;
    case  50 ..  74:    calc = level * 175 + (Str + Wis) * 11 + (Con + Int) *
    7 + (Dex + Qui) *  5;    //  10590 - 16998
                        break;
    case  75 ..  99:    calc = level * 215 + (Str + Wis) * 13 + (Con + Int) *
    11 + (Dex + Qui) *  7;    //  19845 - 28601
                        break;
    case 100 .. 149:    calc = level * 235 + (Str + Wis) * 17 + (Con + Int) *
    13 + (Dex + Qui) * 11;    //  30295 - 49611
                        break;
    case 150 .. 174:    calc = level * 255 + (Str + Wis) * 19 + (Con + Int) *
    17 + (Dex + Qui) * 13;    //  50265 - 64754
                        break;
    case 175 .. 224:    calc = level * 285 + (Str + Wis) * 23 + (Con + Int) *
    19 + (Dex + Qui) * 17;    //  66395 - 95464
                        break;
    case 225 .. 249:    calc = level * 305 + (Str + Wis) * 29 + (Con + Int) *
    23 + (Dex + Qui) * 19;    //  94185 - 118261
                        break;
    case 250 .. 349:    calc = level * 345 + (Str + Wis) * 31 + (Con + Int) *
    29 + (Dex + Qui) * 23;    // 119450 - 189793
                        break;
    case 350 .. 499:    calc = level * 385 + (Str + Wis) * 37 + (Con + Int) *
    31 + (Dex + Qui) * 29;    // 189070 - 308127
                        break;
    case 500 .. 999:    calc = level * 435 + (Str + Wis) * 41 + (Con + Int) *
    37 + (Dex + Qui) * 31;    // 295100 - 666977
                        break;
    default:            calc = level * 475 + (Str + Wis) * 43 + (Con + Int) *
    41 + (Dex + Qui) * 37;    // 668600 - ..? (But level 100000 is safe)
                        break;
  }

  calc += attack_speed * 5;
  calc += max_hp / 15000;
  return calc;
}

void set_wc (int n)
{
  if (n > weapon_class)
    weapon_class2 = weapon_class = n;
  calc_xp ();
}

varargs void set_ac (int n, int w)
{
  int i;

  if (intp (w))
  {
    if (w >= 0 && w < 15)
    {
      if (n > armour_class [w])
        default_ac [w] = n;
//    calc_xp ();
    }
  }
//else if (pointerp(n) && (sizeof(n) == 15) && (sizeof(filter(n, #'intp) ==
    15))
//  default_ac = map(n, #'max, 0);
  else
    for (i = 0; i < 15; i ++)
      default_ac [i] = random (n * 4 / 5) + n * 2 / 5;

  recalc_ac ();
  calc_xp ();
}

void set_aggressive (int a, mixed ar)
{
  aggressive = max (0, a);
  agr_guild = closurep (ar) ? ar : pointerp (ar) ? ar : 0;
}

void init_command (string cmd)
{
  // Use command () in reset ().
  deprecate ("init_command", this_object ());
  log_file ("INITCMD", sprintf("%s: init_command in %O\n", ctime (), TO));
  command (cmd);
}

// set_non_combat_chats (1/x chance per hb, ({ "message", "emote | say | echo"
    }) )
varargs void set_non_combat_chats (int delay, string *messages, int overwrite)
{
  if (delay)
    non_combat_chance = delay;
  else
    non_combat_chance = 30;

  if (overwrite)
  {
    if (messages)
      if (pointerp (messages))
        non_combat_mess = messages;
      else
        non_combat_mess = ({ messages, "echo" });
    else
    {
      non_combat_chance = 0;
      non_combat_mess = ({ });
    }
  }
  else
  {
    if (messages)
      if (pointerp (messages))
        non_combat_mess += messages;
      else
        non_combat_mess += ({ messages, "echo" });
  }
}

varargs void set_combat_chats (int delay, string *messages, int overwrite)
{
  if (delay)
    combat_chance = delay;
  else
    combat_chance = 30;

  if (overwrite)
  {
    if (messages)
      if (pointerp (messages))
        combat_mess = messages;
      else
        combat_mess = ({ messages, "echo" });
    else
    {
      combat_chance = 0;
      combat_mess = ({ });
    }
  }
  else
  {
    if (messages)
      if (pointerp (messages))
        combat_mess += messages;
      else
        combat_mess += ({ messages, "echo" });
  }
}

varargs void set_stunned_chats (int delay, string *messages, int overwrite)
{
  if (delay)
    stunned_chance = delay;
  else
    stunned_chance = 30;

  if (overwrite)
  {
    if (messages)
      if (pointerp (messages))
        stunned_mess = messages;
      else
        stunned_mess = ({ messages, "echo" });
    else
    {
      stunned_chance = 0;
      stunned_mess = ({ });
    }
  }
  else
  {
    if (messages)
      if (pointerp (messages))
        stunned_mess += messages;
      else
        stunned_mess += ({ messages, "echo" });
  }
}
// The next two functions fucking suck, because of how "chats" are handled.
// I'm deprecating them, and writing better functions.  -- Mreh
void load_chat(int chance, string *strs)
{
  deprecate ("load_chat", this_object ());
  set_non_combat_chats (100 / (chance ? chance : 1), strs);
}

void load_a_chat (int chance, string *strs)
{
  deprecate ("load_a_chat", this_object ());
  set_combat_chats (100 / (chance ? chance : 1), strs);
}

varargs mixed second_life (object corpse)
{
  if (!environment ());
  else if (stringp (death_mess))
    tell_others (this_object (), death_mess, 2, ({ this_object(), }));
  else if (pointerp (death_mess))
    foreach (mixed action : death_mess) {
      action          = (pointerp (action) && sizeof (action) && closurep
    (action[0])) ? apply (action[0], action[1..] +({ who_killed, corpse, })) :
    funcall (action, who_killed, corpse);

      mixed   message = 0;
      string  mclass  = pointerp (action) ? ((sizeof (action) > 1) && stringp
    (action[1]) && action[1]) : (stringp (action) && (action[..0] & "@/^!*:#\'&
    "));
      mixed*  args    = pointerp (action) ? action[2..] : ({ });

      action  = pointerp (action) ? (sizeof (action) && stringp (action[0]) &&
    action[0]) : (stringp (action) && action[strlen (mclass)..]);
      action  = strlen (action) && parse_string (action, this_object (),
    who_killed);

      switch (action && (strlen (mclass) ? mclass : "*")) {
      case "@" :  case "call" :  message = call_other (this_object (), action,
    who_killed, corpse, args);   break;
      case "/" :  case "clone":
        !present_clone(action, corpse || environment()) && move_object(clone_
    object(action), corpse || environment());
        break;
      case "^" :  case "dest" :
        present_clone(action, corpse || environment()) && destruct(present_
    clone(action, corpse || environment()));
        break;
      case "!" :  case "do"   :  command (action);
                                 break;
      case "*" :  case "echo" :  message = action;
                                 break;
      case ":" :  case "emote":  message = query_name () +" " +action;
                                 break;
      case "#" :  case "null" :  /* nothing. */
                                 break;
      case "\'":  case "say"  :  command ("say " +action);
                                 break;
      case "&" :  case "tell" :  tell_object (who_killed, action +((action[<1]
    != '\n') ? "\n" : ""));      break;
      }

      if (stringp (message) && strlen (message))
        tell_others (this_object (), message +((message[<1] != '\n') ? "\n" :
    ""));
    }

#if 0
  if (objectp (dead_ob))
    dead_ob = symbol_function ("monster_died", dead_ob);

  if (stringp (dead_ob))
    dead_ob = symbol_function (dead_ob, TO);

  if (closurep (dead_ob))
    return funcall (dead_ob, TO, who_killed, corpse);
#else
  foreach (mixed element : pointerp (dead_ob) ? dead_ob : ({ dead_ob, })) {
    if (objectp (element) || (stringp (element) && sscanf(element, "%~+s/%~+s")
    ))
      element->("monster_died")(this_object (), who_killed, corpse);
    else if (stringp (element))
      this_object ()->(element)(this_object (), who_killed, corpse);
    else if (closurep (element))
      funcall (element, this_object (), who_killed, corpse);
    else if (pointerp (element) && sizeof (element) && closurep (element[0]))
      apply (element[0], element[1..] + ({ who_killed, corpse, }));
  }
#endif

  return 0;
}

void pick_any_obj ()
{
  object *obs, ob;
  string obfile;

  if (!E || !TO)
    return;

  obs = filter (all_inventory (environment ()), (: objectp ($1) && !
    interactive ($1) :));

  if (!pointerp (obs) || !sizeof (obs))
    return;

  ob = pickone (obs);

  if (ob)
    obfile = sprintf ("%s (%s)", base_file (ob), object_name (ob));

  if (ob && ob->get () && ob->short ())
  {
    if (!ob)
    {
      log_file ("BAD_PICK", sprintf ("%s picked %s on %s\n", object_name (TO),
    obfile, ctime ()));
      return;
    }

    if (transfer (ob, TO))
    {
      tell_others (TO, sprintf ("%s foolishly tries to take %s, but fails.\n",
    TO->query_name (), ob->short ()));
      return;
    }

    if (!ob)
    {
      log_file ("BAD_PICK", sprintf ("%s picked %s on %s\n", object_name (TO),
    obfile, ctime ()));
      return;
    }

    tell_others (TO, sprintf ("%s takes %s%s.\n", capitalize (TO->query_name
    ()), ob->short () || "something", ANSI (normal) || ""));

    if (ob->query_weapon ())
      command (sprintf ("wield %s", ob->query_name ()));
    else if (ob->query_armour ())
      command (sprintf ("wear %s", ob->query_name ()));
  }
}

void init ()
{
  if (!stringp (create_room))
    create_room = object_name (E);
  if (stringp (init_ob) || objectp (init_ob))
    init_ob->monster_init (TO);
  else if (closurep (init_ob))
    funcall (init_ob, this_object());
#if 1
  else if (pointerp (init_ob))
    foreach (mixed action : init_ob) {
      action          = closurep (action) ? funcall (action) : (pointerp
    (action) && sizeof (action) && closurep (action[0])) ? apply (action[0],
    action[1..]) : action;

      mixed   message = 0;
      string  mclass  = pointerp (action) ? ((sizeof (action) > 1) && stringp
    (action[1]) && action[1]) : (stringp (action) && (action[..0] & "@/^!*:#\'&
    "));
      mixed*  args    = pointerp (action) ? action[2..] : ({ });

      action  = pointerp (action) ? (sizeof (action) && stringp (action[0]) &&
    action[0]) : (stringp (action) && action[strlen (mclass)..]);
      action  = strlen (action) && parse_string (action, this_object (),
    this_player());

      switch (action && (strlen (mclass) ? mclass : "*")) {
      case "@" :  case "call" :  message = call_other (this_object (), action,
    this_player(), args);          break;
      case "/" :  case "clone":  !present_clone(action) && move_object(clone_
    object(action), this_object());  break;
      case "^" :  case "dest" :  present_clone(action) && destruct(present_
    clone(action));                    break;
      case "!" :  case "do"   :  command (action);
                                   break;
      case "*" :  case "echo" :  message = action;
                                   break;
      case ":" :  case "emote":  message = query_name () +" " +action;
                                   break;
      case "#" :  case "null" :  /* nothing. */
                                   break;
      case "\'":  case "say"  :  command ("say " +action);
                                   break;
      case "&" :  case "tell" :  tell_object (this_player(), action +((action[<
    1] != '\n') ? "\n" : ""));     break;
      }

      if (stringp (message) && strlen (message))
        tell_others (this_object (), message +((message[<1] != '\n') ? "\n" :
    ""));
    }
#endif

  if (attacker_ob ||
     (pointerp (hunter) && sizeof (hunter)) ||
     (pointerp (hunted) && sizeof (hunted)))
    set_heart_beat (1);

  if (TP == TO)
    return;

  if (TP && playerp (TP) && !wizardp (TP) && can_see (TP))
  {
    set_heart_beat (1);
    if (aggressive && TP->QL >= aggressive)
    {
      if (closurep (agr_guild) && funcall (agr_guild, TP))
        kill_ob = TP;
      else if (pointerp (agr_guild) && sizeof (agr_guild) && closurep
    (agr_guild[0]))
        kill_ob = apply (agr_guild[0], agr_guild[1..] +({ this_player (), }))
    && this_player ();
      else if (!pointerp  (agr_guild) || !sizeof (agr_guild) ||
          !stringp (TP->query_guild ()) || TP->query_guild () == "none")
        kill_ob = TP;
      else if (member (agr_guild, lower_case (TP->query_guild ())) == -1)
        kill_ob = TP;
    }
    if (objectp (kill_ob))
      set_heart_beat (1);
  }
}

void set_attack_speed (int num)
{
  attack_speed = num > 6 ? 6 : num;
  set_attack_rate (attack_speed);
  calc_xp ();
}

void set_spell_handler (string *funs, int *chan)
{
  if (!pointerp (funs) || !pointerp (chan))
    return;
  spells = filter (funs, #'function_exists);
  spells_chance = chan;
}

void catch_tell (string str)
{
  set_heart_beat (1);

  if (busy_catch_tell)
    return;

  busy_catch_tell = 1;
  if (talk_ob)
  {
    if (have_text)
    {
      test_match (the_text);
      the_text = str;
    }
    else
    {
      the_text = str;
      have_text = 1;
    }
  }

  busy_catch_tell = 0;
}

set_match (ob, func, type, match)
{
  deprecate ("set_match", this_object ());

  object old;

  if (sizeof (func) != sizeof (type) || sizeof (match) != sizeof (type))
    return;

  talk_ob = ob;
  talk_func = func;
  talk_type = type;
  talk_match = match;
}

void test_match (string str)
{
  deprecate ("test_match", this_object ());

  // This function IS catch_tell.  Use that instead.
  string who, str1, type, match, func;
  int i = 0;

  while (i < sizeof (talk_match))
  {
    if (talk_type [i])
      type = talk_type [i];
    match = talk_match [i];

    if (!stringp (match))
      match = "";
    if (talk_func [i])
      func = talk_func [i];

    if (regexp ( ({ lower_case (str) }), sprintf (".*%s.*", lower_case
    (match))))
      return call_other (talk_ob, func, trimstr (str));
  }
}

/* Ravien 19Dec2011 Monsters now hunt players across logins. */

void check_old_hunt(string name,int time) {
  time--;

  if(time<=0 && stringp(name) && mappingp(hunted_names)) {
    hunted_names -= ([ name ]);
    return;
  }

  object quitter=find_player(name);

  if(playerp(quitter) && member(hunted,quitter) == -1) {
    tell_object(quitter, sprintf("$BOLDFrom afar, you feel the enmity of
    $RED%s!$OFF\n",CAP(query_name())));
    quitter->start_hunted(TO);
    hunted += ({ quitter });

        // Don't check here if our quitter is present, this should be called
    from init().
  }
} // end check_old_hunter

// Bubbs, 2016-03-05: Add monster support for gift recognition.
int catch_give (object giver, object item, int refused) {
  object  here    = objectp (item) && objectp (giver) && present (item) &&
    environment ();

  if (refused || !here || !present (giver, here))
    return refused;

  mixed   actions = 0;
  int     failure = 0;

  if (mappingp (gift_action)) {
    string  item_name = regreplace(load_name(item), "^/*|/+", "/", 1);
    foreach (mixed gift_key : gift_action)
//    if (!actions && ((item_name == gift_key) || (stringp (gift_key) &&
    item->id (gift_key)) || (closurep (gift_key) && funcall (gift_key, item,
    giver))))
//      actions = funcall (gift_action[gift_key], item, giver);
      actions ||= ((item_name == gift_key) || (stringp (gift_key) && item->id
    (gift_key)) || (closurep (gift_key) && funcall (gift_key, item, giver)))
    && funcall (gift_action[gift_key], item, giver);
    actions ||= funcall(gift_action[0], item, giver);
  }
  else
    actions = funcall (gift_action, item, giver);

  actions = pointerp (actions) ? actions : (stringp (actions) && strlen
    (actions) && explode (actions, "\n"));

  if (sizeof (actions)) {
    mapping tokens  = ([ ]);
    tokens["%ITEM"] = (string)(item->query_name ()) || "something";
    tokens["%ITSH"] = (string)(item->short ())      || "something";

    foreach (mixed action : actions) {
      action          = (pointerp (action) && sizeof (action) && closurep
    (action[0])) ? apply (action[0], action[1..] +({ item, giver, })) :
    funcall (action, item, giver);

      string  mclass  = pointerp (action) ? ((sizeof (action) > 1) && stringp
    (action[1]) && action[1]) : (stringp (action) && (action[..0] & "@/^!*:?#\'
    &"));
      mixed*  args    = pointerp (action) ? action[2..] : ({ });
      mixed   message = 0;

      action  = pointerp (action) ? (sizeof (action) && stringp (action[0]) &&
    action[0]) : (stringp (action) && action[strlen (mclass)..]);
      action  = strlen (action) && parse_string (implode (map (regexplode
    (action, "%ITEM|%ITSH"), tokens), ""), this_object (), giver);

      switch (action && (strlen (mclass) ? mclass : "*")) {
      case "@" :  case "call" :  message = call_other (this_object (), action,
    item, giver, args);      break;
      case "/" :  case "clone":  !present(action) && move_object(clone_object(
    action), this_object());  break;
      case "^" :  case "dest" :  present(action) && destruct(present(action));
                             break;
      case "!" :  case "do"   :  command (action);
                             break;
      case "*" :  case "echo" :  message = action;
                             break;
      case ":" :  case "emote":  message = query_name () +" " +action;
                             break;
      case "?" :  case "fail" :  failure = !notify_fail (action);
                             break;
      case "#" :  case "null" :  /* nothing. */
                             break;
      case "\'":  case "say"  :  command ("say " +action);
                             break;
      case "&" :  case "tell" :  tell_object (giver, action +((action[<1] !=
    '\n') ? "\n" : ""));       break;
      }

      if (stringp (message) && strlen (message))
        tell_others (this_object (), message +((message[<1] != '\n') ? "\n" :
    ""));
    }
  }

  return failure;
}

// Bubbs, 2016-03-05: Add monster support for kill recognition.
varargs public void do_kill (object victim, object corpse) {
  mixed   actions = funcall (kill_action, victim, corpse);

  actions = pointerp (actions) ? actions : (stringp (actions) && strlen
    (actions) && explode (actions, "\n"));

  foreach (mixed action : actions || ({ })) {
    action          = (pointerp (action) && sizeof (action) && closurep
    (action[0])) ? apply (action[0], action[1..] +({ victim, corpse, })) :
    funcall (action, victim, corpse);

    string  mclass  = pointerp (action) ? ((sizeof (action) > 1) && stringp
    (action[1]) && action[1]) : (stringp (action) && (action[..0] & "@/^!*:#\'&
    "));
    mixed*  args    = pointerp (action) ? action[2..] : ({ });
    mixed   message = 0;

    action  = pointerp (action) ? (sizeof (action) && stringp (action[0]) &&
    action[0]) : (stringp (action) && action[strlen (mclass)..]);
    action  = strlen (action) && parse_string (action, this_object (), victim);

    switch (action && (strlen (mclass) ? mclass : "*")) {
    case "@" :  case "call" :  message = call_other (this_object (), action,
    victim, corpse, args);         break;
    case "/" :  case "clone":  !present_clone(action) && move_object(clone_
    object(action), this_object());  break;
    case "^" :  case "dest" :  present_clone(action) && destruct(present_clone(
    action));                    break;
    case "!" :  case "do"   :  command (action);
                                 break;
    case "*" :  case "echo" :  message = action;
                                 break;
    case ":" :  case "emote":  message = query_name () +" " +action;
                                 break;
    case "#" :  case "null" :  /* nothing. */
                                 break;
    case "\'":  case "say"  :  command ("say " +action);
                                 break;
    case "&" :  case "tell" :  tell_object (victim, action +((action[<1] !=
    '\n') ? "\n" : ""));            break;
    }

    if (stringp (message) && strlen (message))
      tell_others (this_object (), message +((message[<1] != '\n') ? "\n" :
    ""));
  }
}

// Bubbs, 2019-08-23.
void show_stats(mixed arg) {
  ::show_stats(arg);

  if (create_room)
    printf("Create Room : %s\n", create_room);
  if (move_at_reset || no_tank || random_pick || skinable || whimpy)
    printf("%s%s%s%s%s\n",
      move_at_reset ? "Random-Walk, " : "", no_tank       ? "No-Tank, "     :
    "",
      random_pick   ? "Random-Pick, " : "", skinable      ? "Skinnable, "   :
    "",
      whimpy        ? "Wimpy, "       : "");
  if (bonus_exp || override_exp)
    printf("%s%s%s\n",
      bonus_exp     ? sprintf("Bonus Exp   : %d", bonus_exp)    : "",
      (bonus_exp && override_exp) ? ", " : "",
      override_exp  ? sprintf("Override Exp: %d", override_exp) : "");
  if (healing_rate)
    printf("Healing Rate: %d\n", healing_rate);
  if (attack_speed != 1)
    printf("Attack Speed: %d\n", attack_speed);
  if (hunt_level)
    printf("Hunt Level  : %d\n", hunt_level);
  if (aggressive)
    printf("Aggressive  : %#Q\n", agr_guild || 1);
  if (pointerp(init_ob))
    foreach (mixed action : init_ob)
      printf("Init Action : %#Q\n", action);
  else if (init_ob)
    printf("Init Ob     : %#Q\n", init_ob);
  if (non_combat_chance)
    printf("Normal Mess : Chance 1-in-%d heart-beats\n", non_combat_chance);
  if (pointerp(non_combat_mess))
    foreach (mixed mesg : non_combat_mess)
      printf("Normal Mess : %#Q\n", mesg);
  if (combat_chance)
    printf("Combat Mess : Chance 1-in-%d heart-beats\n", combat_chance);
  if (pointerp(combat_mess))
    foreach (mixed mesg : combat_mess)
      printf("Combat Mess : %#Q\n", mesg);
  if (stunned_chance)
    printf("Stunned Mess: Chance 1-in-%d heart-beats\n", stunned_chance);
  if (pointerp(stunned_mess))
    foreach (mixed mesg : stunned_mess)
      printf("Stunned Mess: %#Q\n", mesg);
  if (spell_chance || spell_dam)
    printf("Spell Chance: %Q%%    Spell Damage: random(%Q)\n", spell_chance,
    spell_dam);
  if (spell_mess1)
    printf("Spell Mess 1: %#Q\n", spell_mess1);
  if (spell_mess2)
    printf("Spell Mess 2: %#Q\n", spell_mess2);
  if (mappingp(gift_action) && gift_action[0])
    printf("Gift Action : %#Q\n" +
           "              %#Q\n", 0, gift_action[0]);
  else if (gift_action)
    printf("Gift Action : %#Q\n", gift_action);
  if (mappingp(gift_action))
    foreach (mixed gift_match : gift_action)
      if (gift_match && gift_action[gift_match])
        printf("Gift Action : %#Q\n" +
               "              %#Q\n", gift_match, gift_action[gift_match]);
  if (pointerp(kill_action))
    foreach (mixed action : kill_action)
      printf("Kill Action : %#Q\n", action);
  else if (kill_action)
    printf("Kill Action : %#Q\n", kill_action);
  if (pointerp(death_mess))
    foreach (mixed action : death_mess)
      printf("Death Action: %#Q\n", action);
  else if (death_mess)
    printf("Death Mess  : %#Q\n", death_mess);
  if (pointerp(dead_ob))
    foreach (mixed action : dead_ob)
      printf("Dead Action : %#Q\n", action);
  else if (dead_ob)
    printf("Dead Ob     : %#Q\n", dead_ob);
}
