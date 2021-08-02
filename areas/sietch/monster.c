
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
int     query_stunned ()                {  return stuned;                                       }

mixed   query_dead_ob ()                {  return dead_ob;                                      }

object  query_pk_owner ()               {  return pk_owner;                                     }

status  can_put_and_get (string str)    {  return stringp (str);                                }
mixed   query_agr_guild ()              {  return agr_guild;                                    }
string  query_create_room ()            {  return create_room;                                  }
mixed   query_death_message ()          {  return death_mess;                                   }
mixed   query_gift_action ()            {  return gift_action;                                  }
string *query_hitting_limbs ()          {  return hitting_limbs;                                }
mixed   query_kill_action ()            {  return kill_action;                                  }
int     query_no_tank ()                {  return no_tank;                                      }
string  query_race
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
  skills =
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

  // Ravien 03Feb2015. Let's get rid of this mechanic for now until we do it right.
  if (0 && (no_tank || !pointerp (alt_attacker_ob)))
  {
    if (!random (20))
    {
      alt_attacker_ob -= ({ 0 });
      alt_attacker_ob = filter (alt_attacker_ob, (: $1 && objectp ($1) && interactive ($1) && present ($1, environment ()) :));
      if (sizeof (alt_attacker_ob))
        i = pickone (alt_attacker_ob);
      else
        return;

      tell_object (i, sprintf ("%s switches and attacks you !!!\n", cap_name));
      tell_room (E, sprintf ("%s switches and attacks %s !!!\n", cap_name, i->QN), ({ i }));
      attack_object (i);
      i->attacked_by (TO);
      alt_attacker_ob -= ({ i });
    }
  }

      return;
    }
  }

  // Give mobs a chance to heal every round.  If they need it.
  if ((hit_point < max_hp || spell_points < max_sp) &&  !(healing_rate / (query_attack () ? 1 : 3)))
    auto_heal ();

  // This code needs to be fixed up, preferably by setting a variable of the room it belongs in,
  // and then only tracking within 2/3 rooms of it.  Teleporting mobs suck.
  if (hunt_level && attacker_ob)
  {
    if (attacker_ob && ENV (attacker_ob) != E && hunt_level && hunt_level < attacker_ob->QL)
    {
      if (ENV (attacker_ob) && !attacker_ob->query_linkdead ())
      {
        tell_room (E, sprintf ("%s runs to hunt %s.\n", cap_name, attacker_ob->QN));
        move_object (TO, ENV (attacker_ob));
        if (objectp (attacker_ob) && attacker_ob)
        {
          efun::tell_object (attacker_ob, sprintf ("%s arrives hunting you !\n", cap_name));
  }

  if (attacker_ob && ENV (attacker_ob) == E && to_int (spell_chance) > random (100))
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

    for (c = 0, rate = max (0, attack_speed) + max (0, bonus_attack); c < rate; c ++)
    {
      if (!objectp (TO) || !attacker_ob || ENV (attacker_ob) != E || get_eval_cost() < 100000)
        break;
      attack ();
    }
  }

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
    rsay = closurep (rsay) ? funcall (rsay) : (pointerp (rsay) && sizeof (rsay) && closurep (rsay[0])) ? apply (rsay[0], rsay[1..]) : rsay;
    rsay = stringp (rsay) ? ({ rsay, "echo", }) : (pointerp (rsay) && (sizeof (rsay) > 1) && rsay);

    switch (this_object () && rsay && stringp (rsay[0]) && strlen (rsay[0]) && rsay[1])
    {
      case 0      :  /* nothing. */                                                                                   break;
      case "call" :  apply (#'call_other', this_object (), rsay[0], rsay[2..]);                                        break;

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

+ Qui) * 3;             //   3605 - 7517
                        break;
    case  50 ..  74:    calc = level * 175 + (Str + Wis) * 11 + (Con + Int) *  7 + (Dex + Qui) *  5;    //  10590 - 16998
                        break;
    case  75 ..  99:    calc = level * 215 + (Str + Wis) * 13 + (Con + Int) * 11 + (Dex + Qui) *  7;    //  19845 - 28601
                        break;
    case 100 .. 149:    calc = level * 235 + (Str + Wis) * 17 + (Con + Int) * 13 + (Dex + Qui) * 11;    //  30295 - 49611
                        break;
    case 150 .. 174:    calc = level * 255 + (Str + Wis) * 19 + (Con + Int) * 17 + (Dex + Qui) * 13;    //  50265 - 64754
                        break;
    case 175 .. 224:    calc = level * 285 + (Str + Wis) * 23 + (Con + Int) * 19 + (Dex + Qui) * 17;    //  66395 - 95464
                        break;
    case 225 .. 249:    calc = level * 305 + (Str + Wis) * 29 + (Con + Int) * 23 + (Dex + Qui) * 19;    //  94185 - 118261
                        break;
    case 250 .. 349:    calc = level * 345 + (Str + Wis) * 31 + (Con + Int) * 29 + (Dex + Qui) * 23;    // 119450 - 189793
                        break;
    case 350 .. 499:    calc = level * 385 + (Str + Wis) * 37 + (Con + Int) * 31 + (Dex + Qui) * 29;    // 189070 - 308127

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
      action          = (pointerp (action) && sizeof (action) && closurep (action[0])) ? apply (action[0], action[1..] +({ who_killed, corpse, })) : funcall (action, who_killed, corpse);

      mixed   message = 0;
      string  mclass  = pointerp (action) ? ((sizeof (action) > 1) && stringp (action[1]) && action[1]) : (stringp (action) && (action[..0] & "@/^!*:#\'&"));
      mixed*  args    = pointerp (action) ? action[2..] : ({ });

      action  = pointerp (action) ? (sizeof (action) && stringp (action[0]) && action[0]) : (stringp (action) && action[strlen (mclass)..]);
      action  = strlen (action) && parse_string (action, this_object (), who_killed);

      switch (action && (strlen (mclass) ? mclass : "*")) {
      case "@" :  case "call" :  message = call_other (this_object (), action, who_killed, corpse, args);   break;
      case "/" :  case "clone":
        !present_clone(action, corpse || environment()) && move_object(clone_object(action), corpse || environment());
        break;
      case "^" :  case "dest" :
        present_clone(action, corpse || environment()) && destruct(present_clone(action, corpse || environment()));
        break;
      case "!" :  case "do"   :  command (action);                                                          break;
      case "*" :  case "echo" :  message = action;                                                          break;
      case ":" :  case "emote":  message = query_name () +" " +action;                                      break;
      case "#" :  case "null" :  /* nothing. */                                                             break;
      case "\'":  case "say"  :  command ("say " +action);                                                  break;

      if (stringp (message) && strlen (message))
        tell_others (this_object (), message +((message[<1] != '\n') ? "\n" : ""));
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
    if (objectp (element) || (stringp (element) && sscanf(element, "%~+s/%~+s")))
      element->("monster_died")(this_object (), who_killed, corpse);
    else if (stringp (element))
      this_object ()->(element)(this_object (), who_killed, corpse);
    else if (closurep (element))
      funcall (element, this_object (), who_killed, corpse);
    else if (pointerp (element) && sizeof (element) && closurep (element[0]))
      apply (element[0], element[1..] + ({ who_killed, corpse, }));
($1) :));

  if (!pointerp (obs) || !sizeof (obs))
    return;

  ob = pickone (obs);

  if (ob)
    obfile = sprintf ("%s (%s)", base_file (ob), object_name (ob));

  if (ob && ob->get () && ob->short ())
  {
    if (!ob)
    {
      log_file ("BAD_PICK", sprintf ("%s picked %s on %s\n", object_name (TO), obfile, ctime ()));
      return;
    }

    if (transfer (ob, TO))
    {
      tell_others (TO, sprintf ("%s foolishly tries to take %s, but fails.\n", TO->query_name (), ob->short ()));
      return;
    }

    tell_others (TO, sprintf ("%s takes %s%s.\n", capitalize (TO->query_name ()), ob->short () || "something", ANSI (normal) || ""));

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
      action          = closurep (action) ? funcall (action) : (pointerp (action) && sizeof (action) && closurep (action[0])) ? apply (action[0], action[1..]) : action;

      mixed   message = 0;
      string  mclass  = pointerp (action) ? ((sizeof (action) > 1) && stringp (action[1]) && action[1]) : (stringp (action) && (action[..0] & "@/^!*:#\'&"));
      mixed*  args    = pointerp (action) ? action[2..] : ({ });

      action  = pointerp (action) ? (sizeof (action) && stringp (action[0]) && action[0]) : (stringp (action) && action[strlen (mclass)..]);
      action  = strlen (action) && parse_string (action, this_object (), this_player());

      switch (action && (strlen (mclass) ? mclass : "*")) {
      case "@" :  case "call" :  message = call_other (this_object (), action, this_player(), args);          break;
      case "/" :  case "clone":  !present_clone(action) && move_object(clone_object(action), this_object());  break;
      case "^" :  case "dest" :  present_clone(action) && destruct(present_clone(action));                    break;
      case "!" :  case "do"   :  command (action);                                                            break;
      case "*" :  case "echo" :  message = action;                                                            break;
      case ":" :  case "emote":  message = query_name () +" " +action;                                        break;
      case "#" :  case "null" :  /* nothing. */                                                               break;
      case "\'":  case "say"  :  command ("say " +action);                                                    break;
      case "&" :  case "tell" :  tell_object (this_player(), action +((action[<1] != '\n') ? "\n" : ""));     break;
      }

      if (stringp (message) && strlen (message))
        tell_others (this_object (), message +((message[<1] != '\n') ? "\n" : ""));
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
      else if (pointerp (agr_guild) && sizeof (agr_guild) && closurep (agr_guild[0]))
        kill_ob = apply (agr_guild[0], agr_guild[1..] +({ this_player (), })) && this_player ();
      else if (!pointerp  (agr_guild) || !sizeof (agr_guild) ||
          !stringp (TP->query_guild ()) || TP->query_guild () == "none")
        kill_ob = TP;
      else if (member (agr_guild, lower_case (TP->query_guild ())) == -1)
        kill_ob = TP;
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
  spells = filter (funs, #'function_exists');
  spells_chance = chan;
}

void catch_tell (string str)
{
  set_heart_beat (1);

  if (busy_catch_tell)
    return;

  busy_catch_tell = 1;
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

    if (regexp ( ({ lower_case (str) }), sprintf (".*%s.*", lower_case (match))))
      return call_other (talk_ob, func, trimstr (str));
  }
}

/* Ravien 19Dec2011 Monsters now hunt players across logins. */

void check_old_hunt(string name,int time) {
  time--;

  if(time<=0 && stringp(name) && mappingp(hunted_names)) {
ction[1]) && action[1]) : (stringp (action) && (action[..0] & "@/^!*:?#\'&"));
      mixed*  args    = pointerp (action) ? action[2..] : ({ });
      mixed   message = 0;

      action  = pointerp (action) ? (sizeof (action) && stringp (action[0]) && action[0]) : (stringp (action) && action[strlen (mclass)..]);
      action  = strlen (action) && parse_string (implode (map (regexplode (action, "%ITEM|%ITSH"), tokens), ""), this_object (), giver);

      switch (action && (strlen (mclass) ? mclass : "*")) {
      case "@" :  case "call" :  message = call_other (this_object (), action, item, giver, args);      break;
      case "/" :  case "clone":  !present(action) && move_object(clone_object(action), this_object());  break;
      case "^" :  case "dest" :  present(action) && destruct(present(action));                          break;
      case "!" :  case "do"   :  command (action);                                                      break;
      case "*" :  case "echo" :  message = action;                                                      break;
      case ":" :  case "emote":  message = query_name () +" " +action;                                  break;
      case "?" :  case "fail" :  failure = !notify_fail (action);                                       break;
      case "#" :  case "null" :  /* nothing. */                                                         break;
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
    printf("Spell Chance: %Q%%    Spell Damage: random(%Q)\n", spell_chance, spell_dam);
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
