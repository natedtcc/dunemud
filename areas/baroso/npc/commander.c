/* Atreidies Marine Commander */
/* MUST UPLOAD*/
/* upgraded by Bebop July 2018 */

inherit "/obj/player/monster";

#include "ansi.h"

void reset(int arg)
{
  object wep, arm;
  string *chat_str;
  ::reset(arg);
  if (arg)
    return;

  if (!chat_str)
  {
    chat_str = allocate(4);
    chat_str[0] = "\nThe Marine Commander BOOMS in an authoritive voice:\n" +
                  "\"The Atreides Marines are the ELITE\"\n";
    chat_str[1] = "\nThe Marine Commander snorts contemptuously at your" +
                  "presence.\n ";
    chat_str[2] = "\nThe Marine Commander looks at you with watchful eyes.\n";
    chat_str[3] = "\nThe Marine Commander BOOMS in an authoritive voice:\n" +
                  "\"Get out of here scum, only Marines are allowed in here!\"\n";
  }

  set_name("commander");
  set_alias(({"atreid", "atreides"}));
  set_level(370);
  set_hei(20);
  add_money(40000 + random(10000));
  set_skill("Long bladed weapons", 1200);

  set_short("Atreides $CYANMarine$OFF Commander");
  set_long(
      "This is the commander of the Atreides forces within Baroso, " +
      "he looks very\nexperienced in fighting, with a large scar on " +
      "the right side of his jawbone.\n");
  set_gender(1);
  set_race("Atreides");
  wep = clone_object("/d/Atreid/kiza/ubase/obj/hawkblade.c");
  wep->set_class(30);
  move_object(wep, this_object());
  command("wield " + wep->query_name());
  arm = clone_object("/d/Atreid/kiza/ubase/obj/muniform.c");
  arm->set_ac(31);
  move_object(arm, this_object());
  command("wear " + arm->query_name());
  load_chat(15, chat_str);
}
