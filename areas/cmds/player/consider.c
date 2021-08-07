&/player>
//
//  Consider command for DuneMUD.
//  Allows a player to view a mob's level and other information.
//
//  October 2020  -- Rewrite by Math
//                   * Changed visual style of graph.
//                   * Changed the reference mobs in the graph.
//                   * Added env-var support for custom graph colors.
//                   * Changed the visual presentation of stat/skill
    comparison.

#include <mudlib.h>
#include <ansi.h>
//#define DEBUG_ON
//#include <debug.h>
//#define DEBUG_TARGET "math"

#define NOANSI(x) remove_ansi(remove_ansi_tokens(x))

// GLOBAL VARIABLES

// as it says
mapping stat_adjective = ([ "str" : "strong",
                            "con" : "sturdy",
                            "int" : "clever",
                            "wis" : "wise",
                            "dex" : "nimble",
                            "qui" : "quick",
                         ]);

// the point of this mapping is that the numerical keys are easier to sort by.
mapping comparison_phrase = ([ -4 : "far less",
                               -3 : "much less",
                               -2 : "somewhat less",
                               -1 : "a bit less",
                                0 : "equally",
                                1 : "a bit more",
                                2 : "somewhat more",
                                3 : "much more",
                                4 : "far more",
                             ]);

// these are the reference mobs, chosen to give an even-ish spread from level
    1 to 700
// and also picked because they have distinctive (unique, in most cases) names.
string* reference_mobs = ({ "/d/Tleilax/hellish/newbie/mobs/masta",
                            "/d/Harko/flagg/complex/npc/trips",
                            "/d/Gesserit/aeon/complex/npcs/rmother",
                            "/d/Harko/sauron/base/admin/general",
                            "/d/Fremen/benedict/smugglers/npc/captain",
                            "/d/Harko/nuemonic/facility/npc/bodyguard",
                            "/d/Harko/sauron/tower/npc/lord",
                            "/d/Tleilax/domain/main_city/npc/ulifurti",
                            "/d/Tleilax/iolai/zoo/npc/grok",
                            "/d/Fremen/terro/facility/npc/ragnarok",
                            "/d/Tleilax/domain/main_city/npc/vrastec",
                            "/d/Imperial/niff/cymek/mobs/beowulf",
                            "/d/Warrior/remognothid/ship/hall2/murbella",
                          });

//the reference mob data is pulled from those objects, not hardcoded.
mapping monster_map = ([ :2 ]);
#define NAME  0
#define LEVEL 1

// FUNCTION PROTOTYPES
varargs void print_line     (string monster, string my_color, string
    enemy_color, string reference_color);
varargs void compare_skills (object monster, string*|string which_skills);
varargs void compare_stats  (object monster, string*|string which_stats);

// MUDLIB FUNCTIONS

void reset(string arg){
  if(arg) return;

  if(!mappingp(monster_map) || !sizeof (monster_map)){
    monster_map = ([:2 ]);
    foreach(string s : reference_mobs){
      object o = load_object(s);
      if(o)
        m_add(monster_map, s, NOANSI(o->short()), o->query_level());
    }
  }
}

mapping query_monster_map(){
  return copy(monster_map);
}

int help(){
  write("See 'help consider'.\n");
  return 1;
}

// MAIN COMMAND FUNCTION

int main(string arg){
  int quiet = sscanf(arg||"","-q %s",arg); // personally, i think this is
    rather clever.

  if(!arg || !sizeof(arg))
    return notify_fail("Usage: consider [-q] <monster>\n");

  quiet = quiet || (this_player()->query_env_var("quiet_consider") ? 1 : 0);

  object monster = present(arg, environment(this_player()));
  if(!monster || !living(monster))
    return notify_fail(sprintf("There is no '%s' here.\n",arg));

  string monster_name = monster->short();
  if (!monster_name)
    return notify_fail(sprintf("There is no '%s' here.\n",arg)); // so we
    can't use consider to detect invisibles

  int cooldown = this_player()->query_cooldown ("consider");
  if(cooldown)
    return notify_fail("Spamming this command hurts the mud. Wait a second.\n")
    ;

  if(!quiet){
    string custom_colors = this_player()->query_env_var("consider_colours");
    if(!this_player()->defined_env_var("ansi") || this_player()->query_no_ansi(
    ))
      custom_colors = "me:off;target:off;reference:off";

    mixed my_color, enemy_color, reference_color;
    if(stringp(custom_colors)){
      string* tmpclrs = explode(custom_colors,";");
      //DUMP_VAR(tmpclrs);
      my_color = filter(tmpclrs, (: $1 && $1[..1]=="me" :));
      my_color = sizeof(my_color) ?  my_color[0] : 0;
      my_color = stringp(my_color) ? my_color[3..] : 0;
      //DUMP_VAR(my_color);

      enemy_color = filter(tmpclrs, (: $1 && $1[..5]=="target" :));
      enemy_color = sizeof(enemy_color) ?  enemy_color[0] : 0;
      enemy_color = stringp(enemy_color) ? enemy_color[7..] : 0;
      //DUMP_VAR(enemy_color);

      reference_color = filter(tmpclrs, (: $1 && $1[..8]=="reference" :));
      reference_color = sizeof(reference_color) ?  reference_color[0] : 0;
      reference_color = stringp(reference_color) ? reference_color[10..] : 0;
      //DUMP_VAR(reference_color);
    }//end custom color logic

    printf("%|100'-'s\n","  COMPARATIVE GRAPH  ");
    // the player and the consider-target are added to the comparative graph
    mapping temp_monster_map = copy(monster_map);
    m_add(temp_monster_map, "you", NOANSI(this_player()->short() || "You"),
    this_player()->query_level());
    m_add(temp_monster_map, arg, NOANSI(monster_name), monster->query_level());
    foreach(string s : sort_array(m_indices(temp_monster_map), (: $3[$1,LEVEL]
    > $3[$2,LEVEL] :), temp_monster_map))
      print_line(s,my_color,enemy_color,reference_color);
    printf("%100'-'s\n","");
  }
  else
    print_line(arg);

  int output_flags = 0;
  float bonus = 0.25;

  //having various levels of skill in eval_opp gives bonus info.
  if(arg && this_player()->query_skills("Evaluate Opponent") > 100){
    printf("You gauge that you are...\n");
    switch(this_player()->query_skills("Evaluate Opponent")){
      case 1000 .. 99999:
        string *weapons = ({ });
        weapons += ({ this_player()->query_weapon1() ? this_player()->query_
    weapon1()->query_type() : 0 });
        weapons += ({ this_player()->query_weapon2() ? this_player()->query_
    weapon2()->query_type() : 0 });
        weapons += ({ monster->query_weapon1() ? monster->query_weapon1()->
    query_type() : 0 });
        weapons += ({ monster->query_weapon2() ? monster->query_weapon2()->
    query_type() : 0 });
        weapons -= ({ 0 });

        if (!sizeof (weapons) || !weapons)
            weapons += ({ "Hand to Hand" });

        compare_skills(monster, weapons),

        output_flags |= 0xF;
        bonus *= 1.5;

      case 750 .. 999:
        if (!(output_flags & 4))
        {
          string *weapons = ({ });
          weapons += ({ this_player()->query_weapon1() ? this_player()->query_
    weapon1()->query_type() : 0 });
          weapons += ({ this_player()->query_weapon2() ? this_player()->query_
    weapon2()->query_type() : 0 });
          weapons += ({ monster->query_weapon1() ? monster->query_weapon1()->
    query_type() : 0 });
          weapons += ({ monster->query_weapon2() ? monster->query_weapon2()->
    query_type() : 0 });
          weapons -= ({ 0 });

          if (!sizeof (weapons))
            weapons += ({ "Hand to Hand" });

          compare_skills(monster, pickone(weapons));
        }

        output_flags |= 4;
        bonus *= 1.5;

      case 500 .. 749:
        compare_skills(monster, ({ "Block", "Dodge", "Roll" }) );
        output_flags |= 2;
        bonus *= 1.5;

      case 250 .. 499:
        compare_stats (monster);
        output_flags |= 1;
        bonus *= 1.5;

      case 150 .. 249:
        if (!(output_flags & 2))
          compare_skills(monster, pickone( ({ "Block", "Dodge", "Roll"}) ));

        output_flags |= 2;
        bonus *= 1.5;

      case 101 .. 149:
        if (!(output_flags & 1))
        compare_stats(monster, pickone( ({ "str", "con", "int", "wis", "qui",
    "dex" }) )),
        output_flags |= 1;
        bonus *= 2;
    }
    printf("%30s... than %s.\n","",monster->short());
  }//end eval_opp logic

  // each main() call uses roughly 12k cycles, so let's throttle it a bit.
  if (!cooldown){
    this_player()->learn_something ("Evaluate Opponent", bonus, 0, 1);
    this_player()->add_cooldown ("consider", 2);
  }

  return 1;
}

// OTHER FUNCTIONS

//this returns the string which is the bar in the graph
string get_level_bars(int x){
  x = max(1,x);

  if(x<=500)
    return INV+("_"*max(1,x/10))+OFF;

  return INV+("_"*30)+"\e[27m//\e[7m"+("_"*(x/10-45))+"\e[0m";
}

//this prints each line of the graph, using custom colors if defined
varargs void print_line(string monster, string my_color, string enemy_color,
    string reference_color){
  if(!monster || monster == "you")
    printf("%s%-30.30s%s : %s%s%s (%d)\n",
            stringp(my_color) ? ansi_colour(my_color) : BOLD+GREEN,
            NOANSI(this_player()->short() || "You"),
            OFF,
            stringp(my_color) ? ansi_colour(my_color) : BOLD+GREEN,
            get_level_bars(this_player()->query_level()),
            OFF,
            max(1, this_player()->query_level()));

  if(member(monster_map, monster))
    printf("%-30.30s : %s%s%s (%d)\n",
           monster_map[monster,NAME],
           stringp(reference_color) ? ansi_colour(reference_color) : GREEN,
           get_level_bars(monster_map[monster,LEVEL]),
           OFF,
           max(1, monster_map[monster,LEVEL]));

  object o = present(monster, environment(this_player()));

  if (!o)
    return;

  printf("%s%-30.30s%s : %s%s%s (%d)\n",
         stringp(enemy_color) ? ansi_colour(enemy_color) : BOLD+RED,
         NOANSI(o->short()),
         OFF,
         stringp(enemy_color) ? ansi_colour(enemy_color) : BOLD+RED,
         get_level_bars(o->query_level()),
         OFF,
         max(1, o->query_level()));
}

//this generates the appropriate key for the comparison_phrase mapping
int comparison_modifier(int val1, int val2){
  int a = max(val1,val2);
  int b = min(val1,val2);
  int mod1;

  switch(percentage(b,a)){
    case  99 ..   100:  mod1 = 0; break;
    case  81 ..    98:  mod1 = 1;  break;
    case  51 ..    80:  mod1 = 2;  break;
    case  26 ..    50:  mod1 = 3;  break;
    case  0  ..    25:  mod1 = 4;  break;
  }

  return val1==a ? mod1 : -mod1;
}

//prints the line(s) which compare stat(s)
varargs void compare_stats(object monster, string*|string which_stats){
  if(stringp(which_stats))
    which_stats = ({which_stats});
  if(!pointerp(which_stats) ||!sizeof(which_stats))
    which_stats = ({"str","dex","con","qui","int","wis"});

  mapping retval = ([:1]);
  int modi;
  foreach(string s : which_stats){
    modi = comparison_modifier(call_other(this_player(),"query_"+s),
    call_other(monster,"query_"+s));
    if(retval[modi])
      retval[modi]+=({stat_adjective[s]});
    else
      m_add(retval,modi,({stat_adjective[s]}));
  }

  foreach(int comp : sort_array(m_indices(retval),#'<))
    printf("%15s%s %s\n",
      "",
      comparison_phrase[comp],
      _implode(retval[comp], ", ", sizeof(retval[comp])>2 ? ", and " : " and
    "));
}

//prints the line(s) which compare skill(s)
varargs void compare_skills(object monster, string*|string which_skills){
  if(stringp(which_skills))
    which_skills = ({which_skills});
  if(!pointerp(which_skills) ||!sizeof(which_skills))
    return;

  mapping retval = ([:1]);
  int modi;
  foreach(string s : which_skills){
    modi = comparison_modifier(this_player()->query_skills(s), monster->query_
    skills(s));
    if(retval[modi])
      retval[modi]+=({s});
    else
      m_add(retval,modi,({s}));
  }

  foreach(int comp : sort_array(m_indices(retval),#'<))
    printf("%15s%s skilled at %s\n",
      "",
      comparison_phrase[comp],
      _implode(retval[comp], ", ", sizeof(retval[comp])>2 ? ", and " : " and
    "));
}
