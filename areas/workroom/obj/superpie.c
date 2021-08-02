inherit "/obj/food";
#include "/include/ansi.h"

#define TP this_player()
#define TO this_object()
#define QN query_name()

string flavor;

reset(arg) {
  ::reset();
  if(arg)return;
  switch(1+random(5)) {
    case 1: flavor = RED+"Strawberry"+OFF; break;
    case 2: flavor = YELLOW+"Banana"+OFF; break;
    case 3: flavor = GREEN+"Apple"+OFF; break;
    case 4: flavor = BOLD+"Whipped cream"+OFF; break;
    case 5: flavor = PURPLE+"Blackberry"+OFF; break;
  }
  set_name("pie");
  set_short("A SuperPie");
  set_long("This pie sure looks yummy.\n");
  set_strength(10);
  set_eater_mess("You gulp down a "+TO->short()+".\n");
  set_eating_mess(" gulps down a "+TO->short()+".\n");
}

init() {
  ::init();
  add_action("throw","throw");
}

throw(arg1) {
object victim;
string arg2,str;
int start_attack;
  victim = TP->query_attack();
  if(arg1 == "pie") {
    if(!victim) { write("At whom?\n"); return 1; }
    write("You throw a "+TO->short()+" at "+victim->query_name()+". MUAHAHAHA\n");
    say(TP->QN+" throws a "+TO->short()+" at "+victim->query_name()+"!\n", ({TP,victim}));
    tell_object(victim, TP->QN+" throws a "+TO->short()+" at you!\n");
    if(victim->query_npc()) victim->set_stuned(5);
                       else victim->add_hp(-random(10));
    destruct(TO);
    return 1;
  }
  if(sscanf(arg1,"pie at %s",str) == 0) return 0;
  if(!victim) victim = present(str,environment(TP));
  if(victim && !living(victim)) {
    write("You can't throw a pie at "+str+".\n");
    return 1;
  }
  if(!victim) victim = find_player(str);
  if(!victim || str != lower_case(victim->query_name())) {
    write("Who the hell are you talking about?\n");
    return 1;
  }
  if(!victim->query_npc() && ((!TP->query_pk() || victim->query_pk()) || !present(str))) {
    write("You throw a "+TO->short()+" at "+victim->query_name()+". MUAHAHAHA\n");
    say(TP->QN+" throws a "+TO->short()+" into the sky.\n", ({TP}));
    tell_object(victim, "A "+TO->short()+" hits you in the face!\n");
    tell_room(environment(victim), "A "+TO->short()+" hits "+victim->query_name()+" in the face!\n", ({TP, victim}));
    destruct(TO);
    return 1;
  }
  write("You throw a "+TO->short()+" at "+victim->query_name()+". MUAHAHAHA\n");
  say(TP->QN+" throws a "+TO->short()+" at "+victim->query_name()+"!\n", ({TP,victim}));
  tell_object(victim, TP->QN+" throws a "+TO->short()+" at you!\n");
  if(victim->query_npc()) victim->set_stuned(5);
                     else victim->add_hp(-random(10));
  if(TP != victim->query_attack()) {
    write(RED+victim->query_name()+" gets furious at you.\n"+OFF);
    victim->attack_object(TP);
  }
  destruct(TO);
  return 1;
}
